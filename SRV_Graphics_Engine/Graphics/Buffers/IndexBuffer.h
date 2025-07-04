#pragma once

#pragma region Includes
#ifndef IdexBuffer_h__
#define IdexBuffer_h__

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

#include "../Device/GraphicsDevice.h"  
#pragma endregion



class IndexBuffer
{
private:
	IndexBuffer(const IndexBuffer& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT bufferSize = 0;

public:
	IndexBuffer() {}

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

	HRESULT Initialize(DWORD* data, UINT numIndices)
	{
		if (buffer.Get() != nullptr)
		{
			buffer.Reset();
		}

		bufferSize = numIndices;

		CD3D11_BUFFER_DESC indexBufferDesc{};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(DWORD) * numIndices;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData{};
		indexBufferData.pSysMem = data;

		HRESULT hr = SRVDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, buffer.GetAddressOf());

		return hr;
	}
};

#endif // !IdexBuffer_h__
