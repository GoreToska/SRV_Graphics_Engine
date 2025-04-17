#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <string>

class VertexShader
{
public:
	bool Initialize(std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements);
	ID3D11VertexShader* GetShader();
	ID3DBlob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

class PixelShader
{
public:
	bool Initialize(std::wstring shaderPath);
	ID3D11PixelShader* GetShader();
	ID3DBlob* GetBuffer();

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer;
};
