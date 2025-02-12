#include "Shaders.h"

#include "d3dcompiler.h"
#include "../../Utils/Logger.h"


// VERTEX SHADER

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device,
	std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), shaderBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, L"Failed to load shader: " + shaderPath);
		return false;
	}

	hr = device->CreateVertexShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		shader.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, L"Failed to create vertex shader: " + shaderPath);
		return false;
	}

	hr = device->CreateInputLayout(
		layoutDesc,
		numElements,
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		inputLayout.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create input layout.");
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return shaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return inputLayout.Get();
}

// PIXEL SHADER

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), shaderBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, L"Failed to load shader: " + shaderPath);
		return false;
	}

	hr = device->CreatePixelShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		shader.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, L"Failed to create vertex shader: " + shaderPath);
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return shaderBuffer.Get();
}
