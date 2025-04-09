#include "PrimitiveRenderComponent.h"
#include "../../../Engine/Engine.h"

ColorMeshComponent::ColorMeshComponent(GameObject* gameObject, std::vector<CVertex> vertexes, ShaderManager::ShaderType type, std::vector<DWORD> indexes)
	: IRenderComponent(gameObject, type), vertexes(vertexes), indexes(indexes)
{
	HRESULT hr = vertexBuffer.Initialize(&this->vertexes[0], this->vertexes.size());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create vertex buffer.");
	}

	if (indexes.size() > 0)
	{
		hr = indexBuffer.Initialize(&this->indexes[0], this->indexes.size());

		if (FAILED(hr))
		{
			Logger::LogError(hr, "Failed to create index buffer.");
		}
	}
}

void ColorMeshComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;
}

void ColorMeshComponent::Render()
{
	IRenderComponent::Render();

	if (indexes.size() > 0)
	{
		DeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		DeviceContext->DrawIndexed(indexes.size(), 0, 0);
	}
	else
	{
		DeviceContext->Draw(vertexes.size(), 0);
	}
}

void ColorMeshComponent::RenderForShadows()
{
	IRenderComponent::RenderForShadows();

	if (indexes.size() > 0)
	{
		DeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		DeviceContext->DrawIndexed(indexes.size(), 0, 0);
	}
	else
	{
		DeviceContext->Draw(vertexes.size(), 0);
	}
}

int ColorMeshComponent::GetVertexCount() const
{
	return vertexes.size();
}

void ColorMeshComponent::SetVertexBuffer()
{
	UINT stride = sizeof(CVertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}
