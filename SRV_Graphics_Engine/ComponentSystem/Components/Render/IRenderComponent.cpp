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

void IRenderComponent::Render(bool setShaders)
{
	SRVDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (setShaders)
	{
		SRVDeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(shaderType)->GetInputLayout());
		SRVDeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(shaderType)->GetShader(), NULL, 0);
		SRVDeviceContext->PSSetShader(ShaderManager::GetInstance().GetPS(shaderType)->GetShader(), NULL, 0);
	}

	SetVertexBufferContext();

	UpdateTransformBuffer(SRVEngine.GetGraphics().GetWorldMatrix(), SRVEngine.GetGraphics().GetCamera()->GetViewMatrix(), SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix());

	UpdateLightBuffer();

	UpdateCascadeShadowBuffer();
}

void IRenderComponent::RenderNoShader()
{
	SRVDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	SetVertexBufferContext();
	UpdateTransformBuffer(SRVEngine.GetGraphics().GetWorldMatrix(), SRVEngine.GetGraphics().GetCamera()->GetViewMatrix(), SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix());
	UpdateLightBuffer();
	UpdateCascadeShadowBuffer();
}

void IRenderComponent::RenderForShadows()
{
	SRVDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	SetVertexBufferContext();

	UpdateTransformBuffer(Matrix::Identity, Matrix::Identity, Matrix::Identity);
}

void IRenderComponent::UpdateLightBuffer()
{
	lightConstBuffer.GetData()->dynamicLightColor = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightColor();
	lightConstBuffer.GetData()->dynamicLightStrength = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightStrength();

	lightConstBuffer.GetData()->dynamicLightDirection =
		SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetForwardVector();


	if (lightConstBuffer.ApplyChanges())
		SRVDeviceContext->PSSetConstantBuffers(0, 1, lightConstBuffer.GetAddressOf());

	//lightMatrixBuffer.GetData()->lightView = DirectX::XMMatrixTranspose(SRVEngine.GetGraphics().GetAllLights()[0]->GetViewMatrix());

	//lightMatrixBuffer.GetData()->lightProjection = DirectX::XMMatrixTranspose(SRVEngine.GetGraphics().GetAllLights()[0]->GetProjectionMatrix()[0]);

	//if (lightMatrixBuffer.ApplyChanges())
	//	SRVDeviceContext->VSSetConstantBuffers(1, 1, lightMatrixBuffer.GetAddressOf());
}

void IRenderComponent::UpdateCascadeShadowBuffer()
{
	cascadeShadowsBuffer.GetData()->Distances =
		Vector4D(ShadowMapCalculator::shadowCascadeDistanceMultipliers[0],
			ShadowMapCalculator::shadowCascadeDistanceMultipliers[1],
			ShadowMapCalculator::shadowCascadeDistanceMultipliers[2],
			ShadowMapCalculator::shadowCascadeDistanceMultipliers[3]);

	std::vector<Matrix> viewProjections = SRVEngine.GetGraphics().GetAllLights()[0]->GetViewProjectionMatricies();

	for (int i = 0; i < ShadowMapCalculator::CascadeCount; ++i)
	{
		cascadeShadowsBuffer.GetData()->ViewProjectionMatrix[i] = SRVEngine.GetGraphics().GetAllLights()[0]->GetViewProjectionMatricies()[i].Transpose();
	}

	auto a = SRVEngine.GetGraphics().GetCamera()->GetPositionVector();

	// maybe here i need a world matrix vector
	cascadeShadowsBuffer.GetData()->CameraPosition = SRVEngine.GetGraphics().GetCamera()->GetPositionVector();

	if (cascadeShadowsBuffer.ApplyChanges())
	{
		SRVDeviceContext->PSSetConstantBuffers(1, 1, cascadeShadowsBuffer.GetAddressOf());
	}
}

void IRenderComponent::UpdateTransformBuffer(Matrix WorldMatrix, Matrix ViewMatrix, Matrix ProjectionMatrix)
{
	objectMatrixBuffer.GetData()->world = gameObject->GetTransform()->GetWorldMatrix().Transpose();;

	objectMatrixBuffer.GetData()->view = ViewMatrix.Transpose();
	objectMatrixBuffer.GetData()->projection = ProjectionMatrix.Transpose();
	objectMatrixBuffer.GetData()->inverseView = ViewMatrix.Invert().Transpose();
	objectMatrixBuffer.GetData()->inverseProjection = ProjectionMatrix.Invert().Transpose();

	if (objectMatrixBuffer.ApplyChanges())
		SRVDeviceContext->VSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());
}

GameObject* IRenderComponent::GetGameObject()
{
	return gameObject;
}
