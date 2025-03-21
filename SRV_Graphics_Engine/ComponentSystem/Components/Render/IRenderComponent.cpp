#include "IRenderComponent.h"
#include "../../../Graphics/Device/GraphicsDevice.h"
#include "../../../Engine/Engine.h"

IRenderComponent::IRenderComponent(GameObject* gameObject, ShaderManager::ShaderType type)
	: gameObject(gameObject), shaderType(type)
{
	HRESULT hr = constBuffer.Initialize();

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create const buffer.");
	}

	hr = lightConstBuffer.Initialize();

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create const light buffer.");
	}
}

void IRenderComponent::Render()
{
	DeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(shaderType)->GetInputLayout());
	DeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(shaderType)->GetShader(), NULL, 0);
	DeviceContext->PSSetShader(ShaderManager::GetInstance().GetPS(shaderType)->GetShader(), NULL, 0);

	DirectX::XMVECTOR orientation = gameObject->GetTransform()->GetOrientation();

#pragma region WorldProjectionMatrix
	constBuffer.GetData()->worldViewProjection = DirectX::XMMatrixScaling(
		gameObject->GetTransform()->GetScale().x,
		gameObject->GetTransform()->GetScale().y,
		gameObject->GetTransform()->GetScale().z);

	constBuffer.GetData()->worldViewProjection *= DirectX::XMMatrixRotationQuaternion(orientation);

	constBuffer.GetData()->worldViewProjection *= DirectX::XMMatrixTranslation(
		gameObject->GetTransform()->GetPosition().x,
		gameObject->GetTransform()->GetPosition().y,
		gameObject->GetTransform()->GetPosition().z);

	constBuffer.GetData()->worldViewProjection *= SRVEngine.GetGraphics().GetWorldMatrix();
	constBuffer.GetData()->worldViewProjection *= SRVEngine.GetGraphics().GetCamera()->GetViewMatrix();
	constBuffer.GetData()->worldViewProjection *= SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix();

	constBuffer.GetData()->worldViewProjection = DirectX::XMMatrixTranspose(constBuffer.GetData()->worldViewProjection);
#pragma endregion

#pragma region WorldMatrix

	constBuffer.GetData()->world = DirectX::XMMatrixScaling(
		gameObject->GetTransform()->GetScale().x,
		gameObject->GetTransform()->GetScale().y,
		gameObject->GetTransform()->GetScale().z);

	constBuffer.GetData()->world *= DirectX::XMMatrixRotationQuaternion(orientation);

	constBuffer.GetData()->world *= DirectX::XMMatrixTranslation(
		gameObject->GetTransform()->GetPosition().x,
		gameObject->GetTransform()->GetPosition().y,
		gameObject->GetTransform()->GetPosition().z);

	constBuffer.GetData()->world *= SRVEngine.GetGraphics().GetWorldMatrix();

	constBuffer.GetData()->world = DirectX::XMMatrixTranspose(constBuffer.GetData()->world);

#pragma endregion

	lightConstBuffer.GetData()->dynamicLightColor = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightColor();
	lightConstBuffer.GetData()->dynamicLightStrength = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightStrength();

	lightConstBuffer.GetData()->dynamicLightPosition =
	{ SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetPosition().x,
	SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetPosition().y,
	SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetPosition().z };

	lightConstBuffer.GetData()->dynamicLightAttenuation_const = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightAttenuationConst();
	lightConstBuffer.GetData()->dynamicLightAttenuation_linear = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightAttenuationLinear();
	lightConstBuffer.GetData()->dynamicLightAttenuation_exponent = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightAttenuationExponent();

	if (constBuffer.ApplyChanges())
		DeviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());

	if (lightConstBuffer.ApplyChanges())
		DeviceContext->PSSetConstantBuffers(0, 1, lightConstBuffer.GetAddressOf());
}

GameObject* IRenderComponent::GetGameObject()
{
	return gameObject;
}
