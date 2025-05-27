#include "LightComponent.h"
#include "../../../DataTypes/ModelData.h"
#include "ShadowMapCalculator.h"

#include <iostream>
#include "../../../Engine/Asserter.h"
#include "../../../Engine/Engine.h"
#include "LightAABB.h"

LightComponent::LightComponent(GameObject* gameObject, LightSourceType type)
	: MeshRendererComponent(ModelData("",
		L""), gameObject, ShaderManager::ShaderType::Texture)
{
	// Data\\Models\\Light\\PointLight\\PointLight.obj
	//Data\\Models\\Light\\PointLight\\PointLight.png
	lightConstBuffer.GetData()->lightColor = lightColor;
	lightConstBuffer.GetData()->lightDirection = Vector4D(gameObject->GetTransform()->GetForwardVector());
	lightConstBuffer.GetData()->lightStrength = lightStrength;
	
	CreateResources();

	sourceType = type;

	if (type == Point)
	{
		AABB box = getAABBForPointLight(*this);

		std::vector<Vector4D> vertices = getAABBVerticies(box);
		std::vector<DWORD> indicies = getAABBIndicies();

		PointSpotVertexBuffer.Initialize(vertices.data(), vertices.size());
		PointSpotIndexBuffer.Initialize(indicies.data(), indicies.size());
	}
	else if (type == Spot)
	{
		AABB box = getAABBForSpotLight(*this);

		std::vector<Vector4D> vertices = getAABBVerticies(box);
		std::vector<DWORD> indicies = getAABBIndicies();

		PointSpotVertexBuffer.Initialize(vertices.data(), vertices.size());
		PointSpotIndexBuffer.Initialize(indicies.data(), indicies.size());
	}
}

void LightComponent::Update(const float& deltaTime)
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

void LightComponent::Render(bool setShaders)
{
	if (!IsEnabled())
		return;

	MeshRendererComponent::Render(setShaders);
}

void LightComponent::SetShadowResources()
{
	SRVDeviceContext->OMSetRenderTargets(0, 0, nullptr);
	SRVDeviceContext->RSSetViewports(1, &shadowMapViewport);
	SRVDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	SRVDeviceContext->VSSetShaderResources(0, 1, shadowSRV.GetAddressOf());
	SRVDeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(ShaderManager::ShadowMap)->GetInputLayout());
	SRVDeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(ShaderManager::ShadowMap)->GetShader(), NULL, 0);
	SRVDeviceContext->GSSetShader(ShaderManager::GetInstance().GetGS(ShaderManager::ShadowMap)->GetShader(), NULL, 0);
	SRVDeviceContext->PSSetShader(NULL, NULL, 0);

	SRVDeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	SRVDeviceContext->OMSetRenderTargets(0, nullptr, depthStencilView.Get());
}

void LightComponent::SetShadowBuffer()
{
	viewProjectionMatricies.clear();
	viewProjectionMatricies.reserve(ShadowMapCalculator::CascadeCount);

	for (int cascade = 0; cascade < ShadowMapCalculator::CascadeCount; ++cascade)
	{
		float nearZ = 0;
		float farZ = 0;

		if (cascade == 0)
		{
			nearZ = SRVEngine.GetGraphics().GetCamera()->GetNearZ();
			farZ = SRVEngine.GetGraphics().GetCamera()->GetFarZ() *
				ShadowMapCalculator::shadowCascadeDistanceMultipliers[cascade];
		}
		else
		{
			nearZ = SRVEngine.GetGraphics().GetCamera()->GetFarZ() *
				ShadowMapCalculator::shadowCascadeDistanceMultipliers[cascade - 1];
			farZ = SRVEngine.GetGraphics().GetCamera()->GetFarZ() *
				ShadowMapCalculator::shadowCascadeDistanceMultipliers[cascade];
		}

		viewProjectionMatricies.push_back(
			ShadowMapCalculator::GetDirectionalLightViewProj(gameObject->GetTransform()->GetForwardVector(),
				nearZ, farZ));
	}

	for (int i = 0; i < ShadowMapCalculator::CascadeCount; ++i)
	{
		cascadeShadowsBuffer.GetData()->ViewProjectionMatrix[i] = viewProjectionMatricies[i].Transpose();
	}

	if (cascadeShadowsBuffer.ApplyChanges())
	{
		SRVDeviceContext->GSSetConstantBuffers(0, 1, cascadeShadowsBuffer.GetAddressOf());
	}
}

void LightComponent::RenderShadowPass(std::vector<IRenderComponent*>& renderObjects)
{
	if (sourceType != Directional)
		return;

	SetShadowBuffer();
	SetShadowResources();

	for (IRenderComponent* RC : renderObjects)
	{
		if (RC->GetGameObject() == gameObject) continue;

		objectMatrixBuffer.GetData()->world = RC->GetGameObject()->GetTransform()->GetWorldMatrix().Transpose();
		if (objectMatrixBuffer.ApplyChanges())
		{
			SRVDeviceContext->VSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());
		}

		RC->RenderForShadows();
	}

	SRVDeviceContext->ClearState();
	//SRVDeviceContext->OMSetRenderTargets(0, 0, nullptr);
	//SRVDeviceContext->GSSetShader(NULL, NULL, 0);
}

void LightComponent::SetLightColor(DirectX::XMFLOAT3& color)
{
	lightColor = color;
}

void LightComponent::SetLightColor(DirectX::XMFLOAT3 color)
{
	lightColor = color;
}

void LightComponent::SetLightStrength(float strength)
{
	lightStrength = strength;
}

void LightComponent::SetLightType(LightSourceType type)
{
	sourceType = type;
}

void LightComponent::SetLightAngle(float degrees)
{
	angle = DirectX::XMConvertToRadians(degrees);
}

void LightComponent::SetLightDistance(float value)
{
	distance = value;
}

DirectX::XMFLOAT3& LightComponent::GetLightColor()
{
	return lightColor;
}

float LightComponent::GetLightStrength()
{
	return lightStrength;
}

float LightComponent::GetLightAttenuationConst() const
{
	return attenuation_const;
}

float LightComponent::GetLightAttenuationLinear() const
{
	return attenuation_linear;
}

float LightComponent::GetLightAttenuationExponent() const
{
	return attenuation_exponent;
}

LightSourceType LightComponent::GetSourceType() const
{
	return sourceType;
}

Vector3D LightComponent::GetLightDirection() const
{
	return gameObject->GetTransform()->GetForwardVector();
}

Vector3D LightComponent::GetLightPosition() const
{
	return gameObject->GetTransform()->GetPosition();
}

float LightComponent::GetLightAngle() const
{
	return angle;
}

float LightComponent::GetLightDistance() const
{
	return distance;
}

ConstantBufferW<PS_LightParamsBuffer>& LightComponent::UpdateLightConstBuffer()
{
	lightConstBuffer.GetData()->angle = angle;
	lightConstBuffer.GetData()->lightColor = lightColor;
	lightConstBuffer.GetData()->lightDirection = Vector4D(GetLightDirection());
	lightConstBuffer.GetData()->lightPosition = GetLightPosition();
	lightConstBuffer.GetData()->lightStrength = lightStrength;
	lightConstBuffer.GetData()->sourceType = sourceType;
	lightConstBuffer.ApplyChanges();

	return lightConstBuffer;
}

ID3D11ShaderResourceView* LightComponent::GetShadowSRV()
{
	return shadowSRV.Get();
}

ID3D11ShaderResourceView* const* LightComponent::GetShadowSRVAddress()
{
	return shadowSRV.GetAddressOf();
}

std::vector<Matrix> LightComponent::GetViewProjectionMatricies()
{
	return viewProjectionMatricies;
}

DirectX::XMMATRIX LightComponent::GetWorldMatrix()
{
	return gameObject->GetTransform()->GetWorldMatrix();
}

VertexBuffer<Vector4D> LightComponent::GetVertexBufferPointSpot() const
{
	return PointSpotVertexBuffer;
}

IndexBuffer LightComponent::GetIndexBufferPointSpot() const
{
	return PointSpotIndexBuffer;
}

std::vector<UINT> LightComponent::GetStridesPointSpot() const
{
	return stridesPointSpot;
}

std::vector<UINT> LightComponent::GetOffsetsPointSpot() const
{
	return offsetsPointSpot;
}

void LightComponent::CreateResources()
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


	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice = 0;
	dsvDesc.Texture2DArray.FirstArraySlice = 0;
	dsvDesc.Texture2DArray.ArraySize = ShadowMapCalculator::CascadeCount;

	ThrowIfFailed(SRVDevice->CreateDepthStencilView(shadowmapTexture.Get(), &dsvDesc, depthStencilView.GetAddressOf()),
		"Failed to create depthStencilView");
}