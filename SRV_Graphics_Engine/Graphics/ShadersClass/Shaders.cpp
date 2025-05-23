#include "Shaders.h"

#include "d3dcompiler.h"
#include "../../Utils/Logger.h"
#include "../../Graphics/Device/GraphicsDevice.h"
#include "../../Engine/Asserter.h"

// VERTEX SHADER

bool VertexShader::Initialize(std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), shaderBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, L"Failed to load shader: " + shaderPath);
		return false;
	}

	hr = SRVDevice->CreateVertexShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		shader.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, L"Failed to create vertex shader: " + shaderPath);
		return false;
	}

	ThrowIfFailed(SRVDevice->CreateInputLayout(
		layoutDesc,
		numElements,
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		inputLayout.GetAddressOf()), "Failed to create input layout.");

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return shader.Get();
}

ID3DBlob* VertexShader::GetBuffer()
{
	return shaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return inputLayout.Get();
}

// PIXEL SHADER

bool PixelShader::Initialize(std::wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), shaderBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, L"Failed to load shader: " + shaderPath);
		return false;
	}

	hr = SRVDevice->CreatePixelShader(
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		NULL,
		shader.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, L"Failed to create pixel shader: " + shaderPath);
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return shader.Get();
}

ID3DBlob* PixelShader::GetBuffer()
{
	return shaderBuffer.Get();
}

// GEOMETRY SHADER
bool GeomertyShader::Initialize(std::wstring shaderPath)
{
	ThrowIfFailed(D3DReadFileToBlob(shaderPath.c_str(), shaderBuffer.GetAddressOf()), "Failed to load shader.");

	ThrowIfFailed(SRVDevice->CreateGeometryShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, shader.GetAddressOf()), 
		"Failed to create geometry shader.");

	return true;
}

ID3D11GeometryShader* GeomertyShader::GetShader()
{
	return shader.Get();
}

ID3DBlob* GeomertyShader::GetBuffer()
{
	return shaderBuffer.Get();
}
