#include "RenderComponent.h"

#include "../../Utils/Logger.h"
#include "../Device/GraphicsDevice.h"


RenderComponent::RenderComponent(std::vector<Vertex3D> vert, std::vector<DWORD> ind)
{
	this->vertices = vert;
	this->indexes = ind;

	HRESULT hr = vertexBuffer.Initialize(&vertices[0], vertices.size());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create vertex buffer.");
	}

	if (ind.size() > 0)
	{
		hr = indexBuffer.Initialize(&indexes[0], indexes.size());

		if (FAILED(hr))
		{
			Logger::LogError(hr, "Failed to create index buffer.");
		}
	}

	hr = constBuffer.Initialize();

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create constant buffer.");
	}
}

void RenderComponent::Render()
{
	UINT stride = sizeof(Vertex3D);
	UINT offset = 0;

	// vertex buffer
	DeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// index buffer
	if (indexes.size() > 0)
	{
		DeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		DeviceContext->DrawIndexed(indexes.size(), 0, 0);
	}
	else
	{
		DeviceContext->Draw(vertices.size(), 0);
	}

	// constant buffer
	constBuffer.GetData()->matrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	constBuffer.GetData()->matrix = DirectX::XMMatrixTranspose(constBuffer.GetData()->matrix);

	if (constBuffer.ApplyChanges())
		DeviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
}

void RenderComponent::SetPosition(const Vector3D& position)
{
	this->position = position;
}

void RenderComponent::MovePosition(const Vector3D& delta)
{
	this->position += delta;
}

Vector3D RenderComponent::GetPosition()
{
	return position;
}
