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

	viewMatrix = ShadowMapCalculator::GetViewMatrixDirectional(gameObject);
	projectionMatrix = ShadowMapCalculator::GetProjectionMatrix();

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

void DirectionalLightComponent::SetRenderTarget()
{
	SRVDeviceContext->OMSetRenderTargets(0, 0, nullptr);
	SRVDeviceContext->OMSetRenderTargets(0, nullptr, depthStencilView.Get());
	SRVDeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	SRVDeviceContext->RSSetViewports(1, &shadowMapViewport);

	SRVDeviceContext->VSSetShaderResources(0, 1, shadowSRV.GetAddressOf());
	SRVDeviceContext->PSSetShaderResources(0, 1, shadowSRV.GetAddressOf());

	SRVDeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(ShaderManager::ShadowMap)->GetInputLayout());
	SRVDeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(ShaderManager::ShadowMap)->GetShader(), NULL, 0);
	SRVDeviceContext->PSSetShader(NULL, NULL, 0);
	SRVDeviceContext->GSSetShader(ShaderManager::GetInstance().GetGS(ShaderManager::ShadowMap)->GetShader(), NULL, 0);
}

void DirectionalLightComponent::ClearRenderTarget()
{
	float color[4] = { 0,0,0,1 };
	//DeviceContext->ClearRenderTargetView(renderTargetView.Get(), color);
	SRVDeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectionalLightComponent::RenderShadowPass(std::vector<IRenderComponent*>& renderObjects)
{
	SetRenderTarget();

	objectMatrixBuffer.GetData()->world = DirectX::XMMatrixTranspose(gameObject->GetTransform()->GetWorldMatrix());
	objectMatrixBuffer.GetData()->view = DirectX::XMMatrixTranspose(viewMatrix);
	objectMatrixBuffer.GetData()->projection = DirectX::XMMatrixTranspose(projectionMatrix);

	if (objectMatrixBuffer.ApplyChanges())
		SRVDeviceContext->VSSetConstantBuffers(0, 1, objectMatrixBuffer.GetAddressOf());

	for (IRenderComponent* RC : renderObjects)
	{
		RC->RenderForShadows(gameObject->GetTransform()->GetWorldMatrix(),
			viewMatrix,
			projectionMatrix);
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

DirectX::XMMATRIX DirectionalLightComponent::GetProjectionMatrix()
{
	return projectionMatrix;
}

DirectX::XMMATRIX DirectionalLightComponent::GetViewMatrix()
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
	viewMatrix = ShadowMapCalculator::GetViewMatrixDirectional(gameObject);
	projectionMatrix = ShadowMapCalculator::GetProjectionMatrix();

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
	texDesc.ArraySize = 5;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	descDSV.Texture2DArray.MipSlice = 0;
	descDSV.Texture2DArray.FirstArraySlice = 0;
	descDSV.Texture2DArray.ArraySize = 5;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = 5;

#pragma endregion

	ThrowIfFailed(SRVDevice->CreateTexture2D(&texDesc, nullptr, shadowmapTexture.GetAddressOf()),
		"Failed to create shadowmapTexture.");

	ThrowIfFailed(SRVDevice->CreateDepthStencilView(shadowmapTexture.Get(), &descDSV, depthStencilView.GetAddressOf()),
		"Failed to create depthStencilView.");

	ThrowIfFailed(SRVDevice->CreateShaderResourceView(shadowmapTexture.Get(), &srvDesc, shadowSRV.GetAddressOf()),
		"Failed to create shadowSRV.");


	ThrowIfFailed(shadowMatrixBuffer.Initialize(), "Failed to create const buffer");

	shadowCascadeDistances.reserve(5);
	shadowCascadeDistances.push_back(1000.0f / 50.0f);
	shadowCascadeDistances.push_back(1000.0f / 25.0f);
	shadowCascadeDistances.push_back(1000.0f / 10.0f);
	shadowCascadeDistances.push_back(1000.0f / 2.0f);
}

Matrix DirectionalLightComponent::GetCascadeLightWVPMatrix(const float nearPlane, const float farPlane)
{
	const Matrix proj = Matrix::CreatePerspectiveFieldOfView(
		SRVEngine.GetGraphics().GetCamera()->GetFOV(),
		SRVEngine.GetGraphics().GetCamera()->GetAspectRatio(),
		nearPlane,
		farPlane);

	const std::vector<Vector4D> corners = SRVEngine.GetGraphics().GetCamera()->GetFrustumCornersWorldPosition(
		SRVEngine.GetGraphics().GetCamera()->GetViewMatrix(), proj);

	Vector3D center = Vector3D::Zero;
	for (const Vector4D& point : corners)
	{
		center.x += point.x;
		center.y += point.y;
		center.z += point.z;
	}

	center /= static_cast<float>(corners.size());

	const Matrix lightView = Matrix::CreateLookAt(center, center - gameObject->GetTransform()->GetForwardVector(), Vector3D::Up);

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const Vector4D& point : corners)
	{
		const Vector4D trf = Vector4D::Transform(point, lightView);
		minX = (std::min)(minX, trf.x);
		maxX = (std::max)(maxX, trf.x);
		minY = (std::min)(minY, trf.y);
		maxY = (std::max)(maxY, trf.y);
		minZ = (std::min)(minZ, trf.z);
		maxZ = (std::max)(maxZ, trf.z);
	}

	constexpr float zMult = 10.0f; // how much geometry to include from outside the view frustum
	minZ = (minZ < 0) ? minZ * zMult : minZ / zMult;
	maxZ = (maxZ < 0) ? maxZ / zMult : maxZ * zMult;

	const Matrix lightProjection = Matrix::CreateOrthographicOffCenter(minX, maxX, minY, maxY, minZ, maxZ);

	return lightView * lightProjection;
}

std::vector<Matrix> DirectionalLightComponent::GetLightSpaceMatrices()
{
	std::vector<Matrix> matrices = {};
	matrices.reserve(shadowCascadeDistances.size() + 1);

	for (size_t i = 0; i < shadowCascadeDistances.size() + 1; ++i)
	{
		if (i == 0)
		{
			matrices.push_back(GetCascadeLightWVPMatrix(SRVEngine.GetGraphics().GetCamera()->GetNearZ(),
				shadowCascadeDistances[i]));
		}
		else if (i < shadowCascadeDistances.size())
		{
			matrices.push_back(GetCascadeLightWVPMatrix(shadowCascadeDistances[i - 1], shadowCascadeDistances[i]));
		}
		else
		{
			matrices.push_back(GetCascadeLightWVPMatrix(shadowCascadeDistances[i - 1],
				SRVEngine.GetGraphics().GetCamera()->GetFarZ()));
		}
	}

	return matrices;
}
