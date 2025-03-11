#pragma once

#pragma region Includes
#ifndef VertexBuffer_h__
#define VertexBuffer_h__

#include <wrl/client.h>
#include <d3d11.h>
#include <memory>

#include "../Device/GraphicsDevice.h"  
#pragma endregion


template<class T>
class VertexBuffer
{

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	std::shared_ptr<UINT> stride;
	UINT bufferSize = 0;

public:
	VertexBuffer() {}

	VertexBuffer(const VertexBuffer<T>& rhs)
	{
		this->buffer = rhs.buffer;
		this->stride = rhs.stride;
		this->bufferSize = rhs.bufferSize;
	}

	VertexBuffer<T>& operator =(const VertexBuffer<T>& other)
	{
		this->buffer = other.buffer;
		this->stride = other.stride;
		this->bufferSize = other.bufferSize;
		return *this;
	}

	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();
	}

	UINT BufferSize() const
	{
		return bufferSize;
	}

	const UINT Stride() const
	{
		return *stride.get();
	}

	const UINT* StridePtr() const
	{
		return stride.get();
	}

	HRESULT Initialize(T* data, UINT numVertices)
	{
		if (buffer.Get() != nullptr)
		{
			buffer.Reset();
		}

		bufferSize = numVertices;
		stride = std::make_shared<UINT>(sizeof(T));

		CD3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData{};
		vertexBufferData.pSysMem = data;

		HRESULT hr = Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, buffer.GetAddressOf());

		return hr;
	}
};



#endif


