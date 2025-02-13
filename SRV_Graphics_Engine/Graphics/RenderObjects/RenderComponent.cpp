#include "RenderComponent.h"

#include "../../Utils/Logger.h"
#include "../Device/GraphicsDevice.h"


RenderComponent::RenderComponent(std::vector<Vertex3D> vertices)
{
	this->vertices = vertices;

	CD3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex3D) * vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData{};
	vertexBufferData.pSysMem = &vertices[0];

	HRESULT hr = Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create vertex buffer.");
	}
}


void RenderComponent::Render()
{
	UINT stride = sizeof(Vertex3D);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	DeviceContext->Draw(vertices.size(), 0);
}
