#include "PointLightComponent.h"
#include "../../../DataTypes/ModelData.h"
#include "ShadowMapCalculator.h"

#include <iostream>
#include "../../../Engine/Asserter.h"

PointLightComponent::PointLightComponent(GameObject* gameObject)
	: MeshRendererComponent(ModelData("Data\\Models\\Light\\PointLight\\PointLight.obj",
		L"Data\\Models\\Light\\PointLight\\PointLight.png"), gameObject, ShaderManager::ShaderType::Texture)
{
	lightConstBuffer.GetData()->dynamicLightColor = lightColor;
	lightConstBuffer.GetData()->dynamicLightPosition = gameObject->GetTransform()->GetPosition().ToXMFloat();
	lightConstBuffer.GetData()->dynamicLightStrength = lightStrength;

	CreateResources();
}

void PointLightComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;

	MeshRendererComponent::Update(deltaTime);

	lightConstBuffer.GetData()->dynamicLightColor = lightColor;
	lightConstBuffer.GetData()->dynamicLightPosition =
	{ gameObject->GetTransform()->GetPosition().x,
	gameObject->GetTransform()->GetPosition().y,
	gameObject->GetTransform()->GetPosition().z };
	lightConstBuffer.GetData()->dynamicLightStrength = lightStrength;
}

void PointLightComponent::Render()
{
	if (!IsEnabled())
		return;

	MeshRendererComponent::Render();
}

void PointLightComponent::SetLightColor(DirectX::XMFLOAT3& color)
{
	lightColor = color;
}

void PointLightComponent::SetLightColor(DirectX::XMFLOAT3 color)
{
	lightColor = color;
}

void PointLightComponent::SetLightStrength(float strength)
{
	lightStrength = strength;
}

DirectX::XMFLOAT3& PointLightComponent::GetLightColor()
{
	return lightColor;
}

float PointLightComponent::GetLightStrength()
{
	return lightStrength;
}

float PointLightComponent::GetLightAttenuationConst() const
{
	return attenuation_const;
}

float PointLightComponent::GetLightAttenuationLinear() const
{
	return attenuation_linear;
}

float PointLightComponent::GetLightAttenuationExponent() const
{
	return attenuation_exponent;
}

void PointLightComponent::CreateResources()
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

	D3D11_TEXTURE2D_DESC tdesc = {};
	tdesc.Width = ShadowMapCalculator::ShadowmapSize;
	tdesc.Height = ShadowMapCalculator::ShadowmapSize;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 6;
	tdesc.Format = DXGI_FORMAT_R32_TYPELESS;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;
	tdesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ThrowIfFailed(Device->CreateTexture2D(&tdesc, nullptr, shadowTextures.GetAddressOf()),
		"Failed to create texture.");


	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice = 0;
	dsvDesc.Texture2DArray.FirstArraySlice = 0;
	dsvDesc.Texture2DArray.ArraySize = 6;

	ThrowIfFailed(Device->CreateDepthStencilView(shadowTextures.Get(), &dsvDesc, depthStencilView.GetAddressOf()),
		"Failed to create depth stencil view.");


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = 1;
	srvDesc.TextureCube.MostDetailedMip = 0;

	ThrowIfFailed(Device->CreateShaderResourceView(shadowTextures.Get(), &srvDesc, shaderResourceView.GetAddressOf()),
		"Failed to create depth stencil view.");
}
