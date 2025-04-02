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

	UpdateTransformBuffer();

	UpdateLightBuffer();
}

void IRenderComponent::UpdateLightBuffer()
{
	lightConstBuffer.GetData()->dynamicLightColor = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightColor();
	lightConstBuffer.GetData()->dynamicLightStrength = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightStrength();

	lightConstBuffer.GetData()->dynamicLightPosition =
		SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetPosition().ToXMFloat();

	lightConstBuffer.GetData()->dynamicLightAttenuation_const = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightAttenuationConst();
	lightConstBuffer.GetData()->dynamicLightAttenuation_linear = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightAttenuationLinear();
	lightConstBuffer.GetData()->dynamicLightAttenuation_exponent = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightAttenuationExponent();

	if (lightConstBuffer.ApplyChanges())
		DeviceContext->PSSetConstantBuffers(0, 1, lightConstBuffer.GetAddressOf());
}

void IRenderComponent::UpdateTransformBuffer()
{
	DirectX::XMVECTOR orientation = gameObject->GetTransform()->GetOrientation();

	constBuffer.GetData()->world = DirectX::XMMatrixTranspose(DirectX::XMMatrixScalingFromVector(
		gameObject->GetTransform()->GetScale().ToXMVector())
		* DirectX::XMMatrixRotationQuaternion(orientation)
		* DirectX::XMMatrixTranslationFromVector(gameObject->GetTransform()->GetPosition().ToXMVector()) * SRVEngine.GetGraphics().GetWorldMatrix());

	constBuffer.GetData()->view = DirectX::XMMatrixTranspose(SRVEngine.GetGraphics().GetCamera()->GetViewMatrix());

	constBuffer.GetData()->projection = DirectX::XMMatrixTranspose(SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix());

	if (constBuffer.ApplyChanges())
		DeviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
}

GameObject* IRenderComponent::GetGameObject()
{
	return gameObject;
}
