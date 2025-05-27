#pragma once

#pragma region Includes
#ifndef ConstantBuffer_h__
#define ConstantBuffer_h__

#include<d3d11.h>
#include <wrl/client.h>

#include "../../Utils/Logger.h"
#include"../Device/GraphicsDevice.h"  
#pragma endregion



template<class T>
class ConstantBufferW
{
private:
	ConstantBufferW(const ConstantBufferW<T>& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	T data;

public:
	ConstantBufferW() {}

	void SetData(T data)
	{
		this->data = data;
	}

	T* GetData()
	{
		return &data;
	}

	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();
	}

	HRESULT Initialize()
	{
		if (buffer.Get() != nullptr)
		{
			buffer.Reset();
		}

		D3D11_BUFFER_DESC constBufferDesc{};
		constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constBufferDesc.MiscFlags = 0;
		// 16 byte alignment
		constBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - sizeof(T) % 16));
		constBufferDesc.StructureByteStride = 0;

		HRESULT hr = SRVDevice->CreateBuffer(&constBufferDesc, 0, buffer.GetAddressOf());

		return hr;
	}

	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		HRESULT hr = SRVDeviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

		if (FAILED(hr))
		{
			Logger::LogError(hr, "Failed to create constant buffer.");
			return false;
		}

		CopyMemory(mappedSubresource.pData, &data, sizeof(T));
		SRVDeviceContext->Unmap(buffer.Get(), 0);

		return true;
	}
};

template<class T>
class ConstantBufferRW
{
private:
	ConstantBufferRW(const ConstantBufferW<T>& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	T data;

public:
	ConstantBufferRW() {}

	void SetData(T data)
	{
		this->data = data;
	}

	T* GetData()
	{
		return &data;
	}

	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();
	}

	HRESULT Initialize(size_t numElements)
	{
		if (buffer.Get() != nullptr)
		{
			buffer.Reset();
		}

		D3D11_BUFFER_DESC constBufferDesc{};
		constBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;;
		constBufferDesc.CPUAccessFlags = 0;
		constBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		constBufferDesc.ByteWidth = sizeof(T) * numElements;
		constBufferDesc.StructureByteStride = sizeof(T);

		HRESULT hr = SRVDevice->CreateBuffer(&constBufferDesc, 0, buffer.GetAddressOf());

		return hr;
	}

	HRESULT Initialize(size_t numElements, T* data)
	{
		if (buffer.Get() != nullptr)
		{
			buffer.Reset();
		}

		D3D11_BUFFER_DESC constBufferDesc{};
		constBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;;
		constBufferDesc.CPUAccessFlags = 0;
		constBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		constBufferDesc.ByteWidth = sizeof(T) * numElements;
		constBufferDesc.StructureByteStride = sizeof(T);

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = data;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr = SRVDevice->CreateBuffer(&constBufferDesc, &subresourceData, buffer.GetAddressOf());

		return hr;
	}

	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE res = {};
		HRESULT hr = SRVDeviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);

		if (FAILED(hr))
		{
			Logger::LogError(hr, "Failed to create constant buffer.");
			return false;
		}

		CopyMemory(res.pData, &data, sizeof(T));
		SRVDeviceContext->Unmap(buffer.Get(), 0);


		return true;
	}
};


#endif // !ConstantBuffer_h__

