#include "Mesh.h"
#include "../../../Engine/Engine.h"


TextureMeshComponent::TextureMeshComponent(GameObject* gameObject, std::vector<TVertex> vertexes, ShaderManager::ShaderType type, std::vector<DWORD> indexes)
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

	hr = objectMatrixBuffer.Initialize();

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create constant buffer.");
	}

	hr = lightConstBuffer.Initialize();

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create constant light buffer.");
	}
}

TextureMeshComponent::TextureMeshComponent(GameObject* gameObject, std::vector<TVertex> vertexes, std::wstring texturePath, ShaderManager::ShaderType type, std::vector<DWORD> indexes)
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

	hr = objectMatrixBuffer.Initialize();

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create constant buffer.");
	}

	hr = lightConstBuffer.Initialize();

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create constant light buffer.");
	}

	ThrowIfFailed(DirectX::CreateWICTextureFromFile(SRVDevice, texturePath.c_str(), nullptr, texture.GetAddressOf()),
		"Failed to create texture");
}

TextureMeshComponent::TextureMeshComponent(const TextureMeshComponent& mesh)
	: IRenderComponent(mesh.gameObject, mesh.shaderType)
{
	this->indexBuffer = mesh.indexBuffer;
	this->objectMatrixBuffer = mesh.objectMatrixBuffer;
	this->lightConstBuffer = mesh.lightConstBuffer;
	this->vertexBuffer = mesh.vertexBuffer;
	this->vertexes = mesh.vertexes;
	this->indexes = mesh.indexes;
}

void TextureMeshComponent::Update(const float& deltaTime)
{
}

void TextureMeshComponent::Render()
{
	IRenderComponent::Render();

	if(texture.Get())
		SRVDeviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());

	if (indexes.size() > 0)
	{
		SRVDeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		SRVDeviceContext->DrawIndexed(indexes.size(), 0, 0);
	}
	else
	{
		SRVDeviceContext->Draw(vertexes.size(), 0);
	}
}

void TextureMeshComponent::RenderForShadows()
{
	IRenderComponent::RenderForShadows();

	if (indexes.size() > 0)
	{
		SRVDeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		SRVDeviceContext->DrawIndexed(indexes.size(), 0, 0);
	}
	else
	{
		SRVDeviceContext->Draw(vertexes.size(), 0);
	}
}

int TextureMeshComponent::GetVertexCount() const
{
	return vertexes.size();
}

void TextureMeshComponent::SetVertexBufferContext()
{
	UINT stride = sizeof(TVertex);
	UINT offset = 0;
	SRVDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}
