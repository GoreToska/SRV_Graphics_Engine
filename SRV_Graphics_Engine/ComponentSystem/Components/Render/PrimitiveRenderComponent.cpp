#include "PrimitiveRenderComponent.h"
#include "../../../Engine/Engine.h"

PrimitiveRenderComponent::PrimitiveRenderComponent(GameObject* gameObject, std::vector<CVertex> vertexes, ShaderManager::ShaderType type, std::vector<DWORD> indexes)
	: IRenderComponent(type), gameObject(gameObject), vertexes(vertexes), indexes(indexes)
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

	hr = constBuffer.Initialize();

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create constant buffer.");
	}

	hr = lightConstBuffer.Initialize();

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create const light buffer.");
	}

	lightConstBuffer.GetData()->ambientLightColor = DirectX::XMFLOAT3(1, 1, 1);
	lightConstBuffer.GetData()->ambientLightStrength = 1;
}

void PrimitiveRenderComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;
}

void PrimitiveRenderComponent::Render()
{
	IRenderComponent::Render();

	DirectX::XMVECTOR orientation = gameObject->GetTransform()->GetOrientation();

	constBuffer.GetData()->matrix = DirectX::XMMatrixScaling(
		gameObject->GetTransform()->GetScale().x,
		gameObject->GetTransform()->GetScale().y,
		gameObject->GetTransform()->GetScale().z);

	constBuffer.GetData()->matrix *= DirectX::XMMatrixRotationQuaternion(orientation);

	constBuffer.GetData()->matrix *= DirectX::XMMatrixTranslation(
		gameObject->GetTransform()->GetPosition().x,
		gameObject->GetTransform()->GetPosition().y,
		gameObject->GetTransform()->GetPosition().z);

	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetWorldMatrix();
	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetCamera()->GetViewMatrix();
	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix();

	constBuffer.GetData()->matrix = DirectX::XMMatrixTranspose(constBuffer.GetData()->matrix);

	if (constBuffer.ApplyChanges())
		DeviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());

	if (lightConstBuffer.ApplyChanges())
		DeviceContext->PSSetConstantBuffers(0, 1, lightConstBuffer.GetAddressOf());

	UINT stride = sizeof(CVertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

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
