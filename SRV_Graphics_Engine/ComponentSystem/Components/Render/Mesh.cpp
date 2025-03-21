#include "Mesh.h"
#include "../../../Engine/Engine.h"


Mesh::Mesh(GameObject* gameObject, std::vector<TVertex> vertexes, std::vector<DWORD> indexes) :
	gameObject(gameObject), vertexes(vertexes), indexes(indexes)
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
		Logger::LogError(hr, "Failed to create constant light buffer.");
	}

	lightConstBuffer.GetData()->ambientLightColor = DirectX::XMFLOAT3(1, 1, 1);
	lightConstBuffer.GetData()->ambientLightStrength = 1;
}

Mesh::Mesh(const Mesh& mesh)
{
	this->gameObject = mesh.gameObject;
	this->indexBuffer = mesh.indexBuffer;
	this->constBuffer = mesh.constBuffer;
	this->lightConstBuffer = mesh.lightConstBuffer;
	this->vertexBuffer = mesh.vertexBuffer;
	this->vertexes = mesh.vertexes;
	this->indexes = mesh.indexes;
}

void Mesh::Render()
{
	UINT stride = sizeof(TVertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	lightConstBuffer.GetData()->dynamicLightColor = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightColor();
	lightConstBuffer.GetData()->dynamicLightStrength = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightStrength();

	lightConstBuffer.GetData()->dynamicLightPosition =
	{ SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetPosition().x,
	SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetPosition().y,
	SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetGameObject()->GetTransform()->GetPosition().z };

	lightConstBuffer.GetData()->dynamicLightAttenuation_const = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightAttenuationConst();
	lightConstBuffer.GetData()->dynamicLightAttenuation_linear = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightAttenuationLinear();
	lightConstBuffer.GetData()->dynamicLightAttenuation_exponent = SRVEngine.GetInstance().GetGraphics().GetAllLights()[0]->GetLightAttenuationExponent();

	if (lightConstBuffer.ApplyChanges())
		DeviceContext->PSSetConstantBuffers(0, 1, lightConstBuffer.GetAddressOf());

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
