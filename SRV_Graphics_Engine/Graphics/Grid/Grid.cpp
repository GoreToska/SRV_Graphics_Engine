#include "Grid.h"
#include "../../Engine/Engine.h"

Grid::Grid()
{
	// Создание вершин для горизонтальных линий
	for (int i = 0; i <= gridLines; ++i) {
		float z = -gridSize / 2 + i * spacing;
		vertexes.push_back({ { -gridSize / 2, 0.0f, z }, WHITE }); // Левый конец
		vertexes.push_back({ { gridSize / 2, 0.0f, z }, WHITE }); // Правый конец
	}

	// Создание вершин для вертикальных линий
	for (int i = 0; i <= gridLines; ++i) {
		float x = -gridSize / 2 + i * spacing;
		vertexes.push_back({ { x, 0.0f, -gridSize / 2 }, WHITE }); // Нижний конец
		vertexes.push_back({ { x, 0.0f, gridSize / 2 }, WHITE }); // Верхний конец
	}

	// Создание индексов
	for (int i = 0; i < gridLines; ++i) {
		indexes.push_back(i * 2);     // Левый конец
		indexes.push_back(i * 2 + 1); // Правый конец
	}
	for (int i = gridLines + 1; i < vertexes.size(); i += 2) {
		indexes.push_back(i);         // Нижний конец
		indexes.push_back(i + 1);     // Верхний конец
	}

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
}

void Grid::Draw()
{
	DeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(ShaderManager::ShaderType::Color)->GetShader(), NULL, 0);
	DeviceContext->PSSetShader(ShaderManager::GetInstance().GetPS(ShaderManager::ShaderType::Color)->GetShader(), NULL, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	constBuffer.GetData()->matrix = DirectX::XMMatrixScaling(1, 1, 1);

	constBuffer.GetData()->matrix *= DirectX::XMMatrixRotationQuaternion({0,0,0});

	constBuffer.GetData()->matrix *= DirectX::XMMatrixTranslation(0, 1, 0);

	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetWorldMatrix();
	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetCamera()->GetViewMatrix();
	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix();

	constBuffer.GetData()->matrix = DirectX::XMMatrixTranspose(constBuffer.GetData()->matrix);

	if (constBuffer.ApplyChanges())
		DeviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());

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
