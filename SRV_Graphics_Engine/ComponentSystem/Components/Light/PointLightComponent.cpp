#include "PointLightComponent.h"
#include "../../../DataTypes/ModelData.h"
#include "ShadowMapCalculator.h"

#include <iostream>
#include "../../../Engine/Asserter.h"
#include "../../../Engine/Engine.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* gameObject)
	: MeshRendererComponent(ModelData("Data\\Models\\Light\\PointLight\\PointLight.obj",
		L"Data\\Models\\Light\\PointLight\\PointLight.png"), gameObject, ShaderManager::ShaderType::Texture)
{
	lightConstBuffer.GetData()->dynamicLightColor = lightColor;
	lightConstBuffer.GetData()->dynamicLightDirection = gameObject->GetTransform()->GetForwardVector().ToXMFloat();
	lightConstBuffer.GetData()->dynamicLightStrength = lightStrength;

	CreateResources();
}

void DirectionalLightComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;

	MeshRendererComponent::Update(deltaTime);

	// probably it not needed here
	lightConstBuffer.GetData()->dynamicLightColor = lightColor;
	lightConstBuffer.GetData()->dynamicLightDirection = gameObject->GetTransform()->GetForwardVector().ToXMFloat();
	lightConstBuffer.GetData()->dynamicLightStrength = lightStrength;
}

void DirectionalLightComponent::Render()
{
	if (!IsEnabled())
		return;

	MeshRendererComponent::Render();
}

void DirectionalLightComponent::SetRenderTarget()
{
	DeviceContext->OMSetRenderTargets(0, 0, nullptr);
	DeviceContext->OMSetRenderTargets(0, nullptr, depthStencilView.Get());
	DeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	DeviceContext->RSSetViewports(1, &shadowMapViewport);
	DeviceContext->VSSetShaderResources(0, 1, shadowSRV.GetAddressOf());
	DeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(shaderType)->GetInputLayout());
	DeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(shaderType)->GetShader(), NULL, 0);
	DeviceContext->PSSetShader(NULL, NULL, 0);
}

void DirectionalLightComponent::ClearRenderTarget()
{
	float color[4] = { 0,0,0,1 };
	//DeviceContext->ClearRenderTargetView(renderTargetView.Get(), color);
	DeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectionalLightComponent::RenderShadowPass(std::vector<IRenderComponent*>& renderObjects)
{
	SetRenderTarget();

	Vector3D orientation = gameObject->GetTransform()->GetOrientation();

	shadowMatrixBuffer.GetData()->world = DirectX::XMMatrixTranspose(DirectX::XMMatrixScalingFromVector(
		gameObject->GetTransform()->GetScale().ToXMVector())
		* DirectX::XMMatrixRotationQuaternion(orientation.ToXMVector())
		* DirectX::XMMatrixTranslationFromVector(gameObject->GetTransform()->GetPosition().ToXMVector())
		* SRVEngine.GetGraphics().GetWorldMatrix());

	shadowMatrixBuffer.GetData()->view = viewMatrix;
	shadowMatrixBuffer.GetData()->projection = DirectX::XMMatrixTranspose(projectionMatrix);

	if (shadowMatrixBuffer.ApplyChanges())
		DeviceContext->PSSetConstantBuffers(0, 1, shadowMatrixBuffer.GetAddressOf());

	for (IRenderComponent* RC : renderObjects)
	{
		// add indexes
		RC->RenderForShadows();
		//DeviceContext->Draw(RC->GetVertexCount(), 0);
	}

	DeviceContext->OMSetRenderTargets(0, 0, nullptr);
}

void DirectionalLightComponent::SetLightColor(DirectX::XMFLOAT3& color)
{
	lightColor = color;
}

void DirectionalLightComponent::SetLightColor(DirectX::XMFLOAT3 color)
{
	lightColor = color;
}

void DirectionalLightComponent::SetLightStrength(float strength)
{
	lightStrength = strength;
}

DirectX::XMFLOAT3& DirectionalLightComponent::GetLightColor()
{
	return lightColor;
}

float DirectionalLightComponent::GetLightStrength()
{
	return lightStrength;
}

float DirectionalLightComponent::GetLightAttenuationConst() const
{
	return attenuation_const;
}

float DirectionalLightComponent::GetLightAttenuationLinear() const
{
	return attenuation_linear;
}

float DirectionalLightComponent::GetLightAttenuationExponent() const
{
	return attenuation_exponent;
}

ID3D11ShaderResourceView* DirectionalLightComponent::GetShadowSRV()
{
	return shadowSRV.Get();
}

ID3D11ShaderResourceView* const* DirectionalLightComponent::GetShadowSRVAddress()
{
	return shadowSRV.GetAddressOf();
}

DirectX::XMMATRIX DirectionalLightComponent::GetProjectionMatrix()
{
	return projectionMatrix;
}

DirectX::XMMATRIX DirectionalLightComponent::GetViewMatrix()
{
	return viewMatrix;
}

void DirectionalLightComponent::CreateResources()
{
	DirectX::XMMATRIX matrices = ShadowMapCalculator::GetViewMatrixDirectional(gameObject);
	viewMatrix = ShadowMapCalculator::GetProjectionMatrix();

	shadowMapViewport.Width = ShadowMapCalculator::ShadowmapSize;
	shadowMapViewport.Height = ShadowMapCalculator::ShadowmapSize;
	shadowMapViewport.MinDepth = 0.0f;
	shadowMapViewport.MaxDepth = 1.0f;
	shadowMapViewport.TopLeftX = 0.0f;
	shadowMapViewport.TopLeftY = 0.0f;

#pragma region Descriptors
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = ShadowMapCalculator::ShadowmapSize;
	texDesc.Height = ShadowMapCalculator::ShadowmapSize;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
#pragma endregion

	ThrowIfFailed(Device->CreateTexture2D(&texDesc, nullptr, shadowmapTexture.GetAddressOf()),
		"Failed to create shadowmapTexture.");

	ThrowIfFailed(Device->CreateDepthStencilView(shadowmapTexture.Get(), &descDSV, depthStencilView.GetAddressOf()),
		"Failed to create depthStencilView.");

	ThrowIfFailed(Device->CreateShaderResourceView(shadowmapTexture.Get(), &srvDesc, shadowSRV.GetAddressOf()),
		"Failed to create shadowSRV.");


	ThrowIfFailed(shadowMatrixBuffer.Initialize(), "Failed to create const buffer");
}
