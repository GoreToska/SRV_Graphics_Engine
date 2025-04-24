#include "IRenderComponent.h"
#include "../../../Graphics/Device/GraphicsDevice.h"
#include "../../../Engine/Engine.h"

IRenderComponent::IRenderComponent(GameObject* gameObject, ShaderManager::ShaderType type)
	: gameObject(gameObject), shaderType(type)
{
	ThrowIfFailed(objectMatrixBuffer.Initialize(), "Failed to create const buffer.");
	ThrowIfFailed(lightMatrixBuffer.Initialize(), "Failed to create const buffer.");
	ThrowIfFailed(lightConstBuffer.Initialize(), "Failed to create const light buffer.");
	ThrowIfFailed(cascadeShadowsBuffer.Initialize(), "Failed to create cascade shadow buffer.");
}

void IRenderComponent::Render()
{
	SRVDeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(shaderType)->GetInputLayout());
	SRVDeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(shaderType)->GetShader(), NULL, 0);
	SRVDeviceContext->PSSetShader(ShaderManager::GetInstance().GetPS(shaderType)->GetShader(), NULL, 0);

	SetVertexBufferContext();

	UpdateTransformBuffer(SRVEngine.GetGraphics().GetWorldMatrix(), SRVEngine.GetGraphics().GetCamera()->GetViewMatrix(), SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix());

	UpdateLightBuffer();

	SRVDeviceContext->PSSetConstantBuffers(1, 1, cascadeShadowsBuffer.GetAddressOf());
}

void IRenderComponent::RenderForShadows(DirectX::XMMATRIX lightWorldMatrix, DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjectionMatrix)
{
	SetVertexBufferContext();

	UpdateTransformBuffer(lightWorldMatrix, lightViewMatrix, lightProjectionMatrix);

	UpdateCascadeShadowBuffer();
}

void IRenderComponent::UpdateLightBuffer()
{
	lightConstBuffer.GetData()->dynamicLightColor = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightColor();
	lightConstBuffer.GetData()->dynamicLightStrength = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightStrength();

	lightConstBuffer.GetData()->dynamicLightDirection =
		SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetForwardVector();

	lightConstBuffer.GetData()->inverseWorldView = DirectX::XMMatrixTranspose(
		Matrix::CreateScale(
			gameObject->GetTransform()->GetScale())
		* Matrix::CreateFromQuaternion(gameObject->GetTransform()->GetRotation()) * 
		SRVEngine.GetGraphics().GetCamera()->GetViewMatrix());

	// TODO: perform transpose in setter function

	if (lightConstBuffer.ApplyChanges())
		SRVDeviceContext->PSSetConstantBuffers(0, 1, lightConstBuffer.GetAddressOf());

	lightMatrixBuffer.GetData()->lightView = DirectX::XMMatrixTranspose(SRVEngine.GetGraphics().GetAllLights()[0]->GetViewMatrix());
	
	lightMatrixBuffer.GetData()->lightProjection = DirectX::XMMatrixTranspose(SRVEngine.GetGraphics().GetAllLights()[0]->GetProjectionMatrix());

	if (lightMatrixBuffer.ApplyChanges())
		SRVDeviceContext->VSSetConstantBuffers(1, 1, lightMatrixBuffer.GetAddressOf());
}

void IRenderComponent::UpdateCascadeShadowBuffer()
{
	cascadeShadowsBuffer.GetData()->Distances = SRVEngine.GetGraphics().GetAllLights()[0]->GetCascadeDistances();
	std::vector<Matrix> viewProjections = SRVEngine.GetGraphics().GetAllLights()[0]->GetLightSpaceMatrices();

	for (int i = 0; i < 5; ++i)
	{
		cascadeShadowsBuffer.GetData()->ViewProjectionMatrix[i] = viewProjections[i];
	}

	if (cascadeShadowsBuffer.ApplyChanges())
	{
		SRVDeviceContext->GSSetConstantBuffers(0, 1, cascadeShadowsBuffer.GetAddressOf());
		//SRVDeviceContext->PSSetConstantBuffers(1,1, cascadeShadowsBuffer.GetAddressOf());
	}
}

void IRenderComponent::UpdateTransformBuffer(DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix)
{
	objectMatrixBuffer.GetData()->world = DirectX::XMMatrixTranspose(gameObject->GetTransform()->GetWorldMatrix());

	objectMatrixBuffer.GetData()->view = DirectX::XMMatrixTranspose(ViewMatrix);
	objectMatrixBuffer.GetData()->projection = DirectX::XMMatrixTranspose(ProjectionMatrix);
	objectMatrixBuffer.GetData()->inverseWorldView = DirectX::XMMatrixTranspose(
		Matrix::CreateScale(gameObject->GetTransform()->GetScale()) 
		* Matrix::CreateFromQuaternion(gameObject->GetTransform()->GetRotation()) * ViewMatrix);

	if (objectMatrixBuffer.ApplyChanges())
		SRVDeviceContext->VSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());
}

GameObject* IRenderComponent::GetGameObject()
{
	return gameObject;
}
