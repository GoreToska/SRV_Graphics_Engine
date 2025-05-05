#pragma once

#include "../Render/MeshRendererComponent.h"
#include "../../GameObject.h"
#include "ShadowMapCalculator.h"

class DirectionalLightComponent : public MeshRendererComponent
{
public:
	DirectionalLightComponent(GameObject* gameObject);

	void Update(const float& deltaTime) override;
	void Render() override;

	void SetRenderTarget();
	void ClearRenderTarget();

	void RenderShadowPass(std::vector<IRenderComponent*>& renderObject);

	void SetLightColor(DirectX::XMFLOAT3& color);
	void SetLightColor(DirectX::XMFLOAT3 color);
	void SetLightStrength(float strength);
	DirectX::XMFLOAT3& GetLightColor();
	float GetLightStrength();
	float GetLightAttenuationConst() const;
	float GetLightAttenuationLinear() const;
	float GetLightAttenuationExponent() const;

	ID3D11ShaderResourceView* GetShadowSRV();
	ID3D11ShaderResourceView* const* GetShadowSRVAddress();

	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetWorldMatrix();

	Vector4D GetCascadeDistances() const;
	std::vector<Matrix> GetLightSpaceMatrices();

private:
	void CreateResources();

	Matrix GetCascadeLightWVPMatrix(const float nearPlane, const float farPlane);

	DirectX::XMFLOAT3 lightColor = DirectX::XMFLOAT3(1, 0, 0);
	float lightStrength = 1.0f;

	float attenuation_const = 1.0f;
	float attenuation_linear = 0.1f;
	float attenuation_exponent = 0.1f;


	// -- shadows --
	Microsoft::WRL::ComPtr<ID3D11Texture2D> shadowmapTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTextures;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> depthStencilViews[ShadowMapCalculator::CascadeCount];
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> shadowSRV;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> renderTargetView;

	DirectX::XMMATRIX projectionMatrix = {};
	DirectX::XMMATRIX viewMatrix = {};
	D3D11_VIEWPORT shadowMapViewport = {};

	ConstantBuffer<VS_ObjectMatrixBuffer> shadowMatrixBuffer;
	// -- shadows --

	std::vector<float> shadowCascadeDistances = {};
};