#include "IRenderComponent.h"
#include "../../../Graphics/Device/GraphicsDevice.h"
#include "../../../Engine/Engine.h"

IRenderComponent::IRenderComponent(GameObject* gameObject, ShaderManager::ShaderType type)
	: gameObject(gameObject), shaderType(type)
{
	ThrowIfFailed(constBuffer.Initialize(), "Failed to create const buffer.");

	ThrowIfFailed(lightConstBuffer.Initialize(), "Failed to create const light buffer.");
}

void IRenderComponent::Render()
{
	DeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(shaderType)->GetInputLayout());
	DeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(shaderType)->GetShader(), NULL, 0);
	DeviceContext->PSSetShader(ShaderManager::GetInstance().GetPS(shaderType)->GetShader(), NULL, 0);

	SetVertexBufferContext();

	UpdateTransformBuffer(SRVEngine.GetGraphics().GetWorldMatrix(), SRVEngine.GetGraphics().GetCamera()->GetViewMatrix(), SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix());

	UpdateLightBuffer();
}

void IRenderComponent::RenderForShadows()
{
	SetVertexBufferContext();

	UpdateTransformBuffer(SRVEngine.GetGraphics().GetAllLights()[0]->GetWorldMatrix(),
		SRVEngine.GetGraphics().GetAllLights()[0]->GetViewMatrix(),
		SRVEngine.GetGraphics().GetAllLights()[0]->GetProjectionMatrix());

	UpdateLightBuffer();
}

void IRenderComponent::UpdateLightBuffer()
{
	lightConstBuffer.GetData()->dynamicLightColor = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightColor();
	lightConstBuffer.GetData()->dynamicLightStrength = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightStrength();

	lightConstBuffer.GetData()->dynamicLightDirection =
		SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetForwardVector().ToXMFloat();

	GameObject* light = SRVEngine.GetGraphics().GetAllLights()[0]->GetGameObject();

	lightConstBuffer.GetData()->lightWorld = DirectX::XMMatrixTranspose(DirectX::XMMatrixScalingFromVector(
		light->GetTransform()->GetScale().ToXMVector())
		* DirectX::XMMatrixRotationQuaternion(light->GetTransform()->GetOrientation().ToXMVector())
		* DirectX::XMMatrixTranslationFromVector(light->GetTransform()->GetPosition().ToXMVector())
		* SRVEngine.GetGraphics().GetWorldMatrix());

	lightConstBuffer.GetData()->lightView = SRVEngine.GetGraphics().GetAllLights()[0]->GetViewMatrix();
	lightConstBuffer.GetData()->lightProjection = SRVEngine.GetGraphics().GetAllLights()[0]->GetProjectionMatrix();

	if (lightConstBuffer.ApplyChanges())
		DeviceContext->PSSetConstantBuffers(0, 1, lightConstBuffer.GetAddressOf());
}

void IRenderComponent::UpdateTransformBuffer(DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix)
{
	constBuffer.GetData()->world = DirectX::XMMatrixTranspose(DirectX::XMMatrixScalingFromVector(
		gameObject->GetTransform()->GetScale().ToXMVector())
		* DirectX::XMMatrixRotationQuaternion(gameObject->GetTransform()->GetOrientation().ToXMVector())
		* DirectX::XMMatrixTranslationFromVector(gameObject->GetTransform()->GetPosition().ToXMVector()) * WorldMatrix);

	constBuffer.GetData()->view = DirectX::XMMatrixTranspose(ViewMatrix);

	constBuffer.GetData()->projection = DirectX::XMMatrixTranspose(ProjectionMatrix);

	if (constBuffer.ApplyChanges())
		DeviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
}

GameObject* IRenderComponent::GetGameObject()
{
	return gameObject;
}
