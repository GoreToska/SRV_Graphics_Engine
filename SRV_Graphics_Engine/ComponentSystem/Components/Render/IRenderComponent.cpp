#include "IRenderComponent.h"
#include "../../../Graphics/Device/GraphicsDevice.h"
#include "../../../Engine/Engine.h"

IRenderComponent::IRenderComponent(GameObject* gameObject, ShaderManager::ShaderType type)
	: gameObject(gameObject), shaderType(type)
{
	ThrowIfFailed(objectMatrixBuffer.Initialize(), "Failed to create const buffer.");
	ThrowIfFailed(lightMatrixBuffer.Initialize(), "Failed to create const buffer.");
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

void IRenderComponent::RenderForShadows(DirectX::XMMATRIX lightWorldMatrix, DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjectionMatrix)
{
	SetVertexBufferContext();

	UpdateTransformBuffer(lightWorldMatrix, lightViewMatrix, lightProjectionMatrix);
}

void IRenderComponent::UpdateLightBuffer()
{
	lightConstBuffer.GetData()->dynamicLightColor = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightColor();
	lightConstBuffer.GetData()->dynamicLightStrength = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightStrength();

	lightConstBuffer.GetData()->dynamicLightDirection =
		SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetForwardVector().ToXMFloat();
	

	// TODO: perform transpose in setter function

	if (lightConstBuffer.ApplyChanges())
		DeviceContext->PSSetConstantBuffers(0, 1, lightConstBuffer.GetAddressOf());

	lightMatrixBuffer.GetData()->lightView = DirectX::XMMatrixTranspose(SRVEngine.GetGraphics().GetAllLights()[0]->GetViewMatrix());
	lightMatrixBuffer.GetData()->lightProjection = DirectX::XMMatrixTranspose(SRVEngine.GetGraphics().GetAllLights()[0]->GetProjectionMatrix());

	if (lightMatrixBuffer.ApplyChanges())
		DeviceContext->VSSetConstantBuffers(1, 1, lightMatrixBuffer.GetAddressOf());
}

void IRenderComponent::UpdateTransformBuffer(DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix)
{
	// TODO: perform transpose in setter function
	objectMatrixBuffer.GetData()->world = DirectX::XMMatrixTranspose(DirectX::XMMatrixScalingFromVector(
		gameObject->GetTransform()->GetScale().ToXMVector())
		* DirectX::XMMatrixRotationQuaternion(gameObject->GetTransform()->GetOrientation().ToXMVector())
		* DirectX::XMMatrixTranslationFromVector(gameObject->GetTransform()->GetPosition().ToXMVector())
		);

	objectMatrixBuffer.GetData()->view = DirectX::XMMatrixTranspose(ViewMatrix);
	objectMatrixBuffer.GetData()->projection = DirectX::XMMatrixTranspose(ProjectionMatrix);

	if (objectMatrixBuffer.ApplyChanges())
		DeviceContext->VSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());
}

GameObject* IRenderComponent::GetGameObject()
{
	return gameObject;
}
