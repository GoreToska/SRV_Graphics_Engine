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
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	T data;

public:
	ConstantBuffer() {}

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


#endif // !ConstantBuffer_h__

