#include "RenderComponent.h"

#include "../../Utils/Logger.h"
#include "../../Graphics/Device/GraphicsDevice.h"
#include "../../Engine/Engine.h"

RenderComponent::RenderComponent(TransformComponent* transform, std::vector<Vertex3D> vert, std::vector<DWORD> ind)
{
	this->vertices = vert;
	this->indexes = ind;
	this->transform = transform;

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

void RenderComponent::Update(const float& deltaTime)
{
	// TODO: what to do here?
}

void RenderComponent::Render()
{
	UINT stride = sizeof(Vertex3D);
	UINT offset = 0;

	// constant buffer
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	constBuffer.GetData()->matrix = DirectX::XMMatrixTranslation(
		transform->GetPosition().x,
		transform->GetPosition().y,
		transform->GetPosition().z);

	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetWorldMatrix();
	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetCamera()->GetViewMatrix();
	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix();

	constBuffer.GetData()->matrix = DirectX::XMMatrixTranspose(constBuffer.GetData()->matrix);

	if (constBuffer.ApplyChanges())
		DeviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());

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
}
