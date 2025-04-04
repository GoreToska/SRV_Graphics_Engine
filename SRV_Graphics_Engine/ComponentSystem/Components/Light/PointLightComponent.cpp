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
	// если cube shadow map то надо 6?
	DeviceContext->OMSetRenderTargets(6, renderTargetView.GetAddressOf(), depthStencilView.Get());
	DeviceContext->RSSetViewports(1, &shadowMapViewport);
}

void DirectionalLightComponent::ClearRenderTarget()
{
	float color[4] = { 0,0,0,1 };
	DeviceContext->ClearRenderTargetView(renderTargetView.Get(), color);
	DeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectionalLightComponent::RenderShadowPass(IRenderComponent* renderObject)
{
	Vector3D orientation = gameObject->GetTransform()->GetOrientation();

	shadowMatrixBuffer.GetData()->world = DirectX::XMMatrixTranspose(DirectX::XMMatrixScalingFromVector(
		gameObject->GetTransform()->GetScale().ToXMVector())
		* DirectX::XMMatrixRotationQuaternion(orientation.ToXMVector())
		* DirectX::XMMatrixTranslationFromVector(gameObject->GetTransform()->GetPosition().ToXMVector())
		* SRVEngine.GetGraphics().GetWorldMatrix());

	shadowMatrixBuffer.GetData()->view = viewMatrix;

	for (size_t i = 0; i < 6; ++i)
	{
		shadowMatrixBuffer.GetData()->projection = DirectX::XMMatrixTranspose(projectionMatrix[i]);

		if (shadowMatrixBuffer.ApplyChanges())
			DeviceContext->PSSetConstantBuffers(0, 1, shadowMatrixBuffer.GetAddressOf());

		DeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(shaderType)->GetInputLayout());
		DeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(shaderType)->GetShader(), NULL, 0);
		DeviceContext->PSSetShader(ShaderManager::GetInstance().GetPS(shaderType)->GetShader(), NULL, 0);

		// add indexes
		DeviceContext->Draw(renderObject->GetVertexCount(), 0);
	}
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

void DirectionalLightComponent::CreateResources()
{
	DirectX::XMMATRIX* matrices = ShadowMapCalculator::GetPerspectiveMatricesCube(gameObject);
	std::copy(matrices, matrices + 6, projectionMatrix);

	viewMatrix = ShadowMapCalculator::GetProjectionMatrix();

	shadowMapViewport.Width = ShadowMapCalculator::ShadowmapSize;
	shadowMapViewport.Height = ShadowMapCalculator::ShadowmapSize;
	shadowMapViewport.MinDepth = 0.0f;
	shadowMapViewport.MaxDepth = 1.0f;
	shadowMapViewport.TopLeftX = 0.0f;
	shadowMapViewport.TopLeftY = 0.0f;

	D3D11_TEXTURE2D_DESC renderTargetdesc = {};
	renderTargetdesc.Width = ShadowMapCalculator::ShadowmapSize;
	renderTargetdesc.Height = ShadowMapCalculator::ShadowmapSize;
	renderTargetdesc.MipLevels = 1;
	renderTargetdesc.ArraySize = 6;
	renderTargetdesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetdesc.SampleDesc.Count = 1;
	renderTargetdesc.SampleDesc.Quality = 0;
	renderTargetdesc.Usage = D3D11_USAGE_DEFAULT;
	renderTargetdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	renderTargetdesc.CPUAccessFlags = 0;
	renderTargetdesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ThrowIfFailed(Device->CreateTexture2D(&renderTargetdesc, nullptr, renderTargetTextures.GetAddressOf()),
		"Failed to create texture.");


	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	renderTargetViewDesc.Format = renderTargetdesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	ThrowIfFailed(Device->CreateRenderTargetView(renderTargetTextures.Get(),
		&renderTargetViewDesc, renderTargetView.GetAddressOf()), "Failed to create render target view.");


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = renderTargetdesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = 1;
	srvDesc.TextureCube.MostDetailedMip = 0;

	ThrowIfFailed(Device->CreateShaderResourceView(renderTargetTextures.Get(), &srvDesc, shaderResourceView.GetAddressOf()),
		"Failed to create depth stencil view.");


	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	depthBufferDesc.Width = ShadowMapCalculator::ShadowmapSize;
	depthBufferDesc.Height = ShadowMapCalculator::ShadowmapSize;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 6;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	ThrowIfFailed(Device->CreateTexture2D(& depthBufferDesc, NULL, depthStencilTextures.GetAddressOf()),
		"Failed to create depth texture.");

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = depthBufferDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice = 0;
	dsvDesc.Texture2DArray.FirstArraySlice = 0;
	dsvDesc.Texture2DArray.ArraySize = 6;

	ThrowIfFailed(Device->CreateDepthStencilView(depthStencilTextures.Get(), &dsvDesc, depthStencilView.GetAddressOf()),
		"Failed to create depth stencil view.");

	ThrowIfFailed(shadowMatrixBuffer.Initialize(),"Failed to create const buffer");
}
