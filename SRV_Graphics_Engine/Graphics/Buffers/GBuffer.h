#pragma once
#include "../../DataTypes/Vector3D.h"

#include <d3d11.h>
#include <wrl/client.h>


class GBuffer
{
public:
	GBuffer();
	
	void SetRenderTargets() const;
	void PSBindResourceViews(int startIndex) const;
	void PSClearResourceViews(int startIndex) const;
	void ClearRenderTargets() const;

private:

	void Initialize();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> normal;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> diffuse;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> specular;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> specularSRV;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> depthRTV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> normalRTV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> diffuseRTV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> specularRTV;
};

