#include "PointLightComponent.h"
#include "../../../DataTypes/ModelData.h"
#include "ShadowMapCalculator.h"

#include <iostream>
#include "../../../Engine/Asserter.h"
#include "../../../Engine/Engine.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* gameObject)
	: MeshRendererComponent(ModelData("",
		L""), gameObject, ShaderManager::ShaderType::Texture)
{
	// Data\\Models\\Light\\PointLight\\PointLight.obj
	//Data\\Models\\Light\\PointLight\\PointLight.png
	lightConstBuffer.GetData()->dynamicLightColor = lightColor;
	lightConstBuffer.GetData()->dynamicLightDirection = gameObject->GetTransform()->GetForwardVector();
	lightConstBuffer.GetData()->dynamicLightStrength = lightStrength;

	CreateResources();
}

void DirectionalLightComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;

	//viewMatrix = ShadowMapCalculator::GetViewMatrixDirectional(gameObject);
	//projectionMatrix = ShadowMapCalculator::GetProjectionMatrix();

	MeshRendererComponent::Update(deltaTime);


	// probably it not needed here
	/*lightConstBuffer.GetData()->dynamicLightColor = lightColor;
	lightConstBuffer.GetData()->dynamicLightDirection = gameObject->GetTransform()->GetForwardVector();
	lightConstBuffer.GetData()->dynamicLightStrength = lightStrength;*/
}

void DirectionalLightComponent::Render()
{
	if (!IsEnabled())
		return;

	MeshRendererComponent::Render();
}

void DirectionalLightComponent::SetShadowResources(size_t cascade_index)
{
	SRVDeviceContext->OMSetRenderTargets(0, 0, nullptr);
	SRVDeviceContext->OMSetRenderTargets(0, nullptr, depthStencilViews[cascade_index].Get());
	SRVDeviceContext->ClearDepthStencilView(depthStencilViews[cascade_index].Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	SRVDeviceContext->RSSetViewports(1, &shadowMapViewport);

	SRVDeviceContext->VSSetShaderResources(0, 1, shadowSRV.GetAddressOf());
	SRVDeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(ShaderManager::ShadowMap)->GetInputLayout());
	SRVDeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(ShaderManager::ShadowMap)->GetShader(), NULL, 0);
	SRVDeviceContext->GSSetShader(ShaderManager::GetInstance().GetGS(ShaderManager::ShadowMap)->GetShader(), NULL, 0);
	SRVDeviceContext->PSSetShader(NULL, NULL, 0);

	// calculate for each cascade (with nearZ and farZ)
	ShadowMapCalculator::GetDirectionalLightMatrices(gameObject, projectionMatrix, viewMatrix);
}

void DirectionalLightComponent::SetShadowBuffer(size_t cascade_index)
{
	objectMatrixBuffer.GetData()->view = DirectX::XMMatrixTranspose(viewMatrix);
	objectMatrixBuffer.GetData()->projection = DirectX::XMMatrixTranspose(projectionMatrix[cascade_index]);

	if (objectMatrixBuffer.ApplyChanges())
	{
		SRVDeviceContext->VSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());
		//SRVDeviceContext->GSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());
	}

	for (int i = 0; i < ShadowMapCalculator::CascadeCount; ++i)
	{
		cascadeShadowsBuffer.GetData()->ViewProjectionMatrix[i] = (viewMatrix * projectionMatrix[i]).Transpose();
	}

	if (cascadeShadowsBuffer.ApplyChanges())
	{
		SRVDeviceContext->GSSetConstantBuffers(0, 1, cascadeShadowsBuffer.GetAddressOf());
	}
}

void DirectionalLightComponent::RenderShadowPass(std::vector<IRenderComponent*>& renderObjects)
{
	for (size_t i = 0; i < ShadowMapCalculator::CascadeCount; ++i)
	{
		SetShadowResources(i);
		SetShadowBuffer(i);

		for (IRenderComponent* RC : renderObjects)
		{
			if (RC->GetGameObject() == gameObject) continue;
			objectMatrixBuffer.GetData()->world = RC->GetGameObject()->GetTransform()->GetWorldMatrix().Transpose();
			if (objectMatrixBuffer.ApplyChanges())
			{
				SRVDeviceContext->VSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());
			}

			RC->RenderForShadows(gameObject->GetTransform()->GetWorldMatrix(),
				viewMatrix,
				projectionMatrix[i]);
		}
	}

	SRVDeviceContext->OMSetRenderTargets(0, 0, nullptr);
	SRVDeviceContext->GSSetShader(NULL, NULL, 0);
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

std::vector<Matrix> DirectionalLightComponent::GetProjectionMatrix()
{
	return projectionMatrix;
}

Matrix DirectionalLightComponent::GetViewMatrix()
{
	return viewMatrix;
}

DirectX::XMMATRIX DirectionalLightComponent::GetWorldMatrix()
{
	return gameObject->GetTransform()->GetWorldMatrix();
}

Vector4D DirectionalLightComponent::GetCascadeDistances() const
{
	return Vector4D(
		shadowCascadeDistances[0],
		shadowCascadeDistances[1],
		shadowCascadeDistances[2],
		shadowCascadeDistances[3]);
}

void DirectionalLightComponent::CreateResources()
{
	shadowMapViewport.Width = ShadowMapCalculator::ShadowmapSize;
	shadowMapViewport.Height = ShadowMapCalculator::ShadowmapSize;
	shadowMapViewport.MinDepth = 0.0f;
	shadowMapViewport.MaxDepth = 1.0f;
	shadowMapViewport.TopLeftX = 0.0f;
	shadowMapViewport.TopLeftY = 0.0f;

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = ShadowMapCalculator::ShadowmapSize;
	texDesc.Height = ShadowMapCalculator::ShadowmapSize;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = ShadowMapCalculator::CascadeCount;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.ArraySize = ShadowMapCalculator::CascadeCount;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.MostDetailedMip = 0;

	ThrowIfFailed(SRVDevice->CreateTexture2D(&texDesc, nullptr, shadowmapTexture.GetAddressOf()),
		"Failed to create shadowmapTexture.");

	ThrowIfFailed(SRVDevice->CreateShaderResourceView(shadowmapTexture.Get(), &srvDesc, shadowSRV.GetAddressOf()),
		"Failed to create shadowSRV.");


	for (size_t i = 0; i < ShadowMapCalculator::CascadeCount; ++i)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		dsvDesc.Texture2DArray.MipSlice = 0;
		dsvDesc.Texture2DArray.FirstArraySlice = i;
		dsvDesc.Texture2DArray.ArraySize = 1;

		ThrowIfFailed(SRVDevice->CreateDepthStencilView(shadowmapTexture.Get(), &dsvDesc, depthStencilViews[i].GetAddressOf()),
			"Failed to create depthStencilView: " + i);
	}

	shadowCascadeDistances.push_back(1000.0f / 50.0f);
	shadowCascadeDistances.push_back(1000.0f / 25.0f);
	shadowCascadeDistances.push_back(1000.0f / 10.0f);
	shadowCascadeDistances.push_back(1000.0f / 2.0f);
}