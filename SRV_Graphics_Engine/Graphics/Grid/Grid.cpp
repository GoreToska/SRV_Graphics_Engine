#include "Grid.h"

Grid::Grid()
{
	for (float z = -gridSize; z <= gridSize; z += gridSpacing) {
		vertices.push_back({ Vector3D(-gridSize, 0.0f, z), WHITE });
		vertices.push_back({ Vector3D(gridSize, 0.0f, z), WHITE });
		indices.push_back(static_cast<uint16_t>(vertices.size() - 2));
		indices.push_back(static_cast<uint16_t>(vertices.size() - 1));
	}

	// Создаем линии, параллельные оси Z
	for (float x = -gridSize; x <= gridSize; x += gridSpacing) {
		vertices.push_back({ Vector3D(x, 0.0f, -gridSize), WHITE });
		vertices.push_back({ Vector3D(x, 0.0f, gridSize), WHITE });
		indices.push_back(static_cast<DWORD>(vertices.size() - 2));
		indices.push_back(static_cast<DWORD>(vertices.size() - 1));
	}

	HRESULT hr = vertexBuffer.Initialize(&this->vertices[0], this->vertices.size());

	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create vertex buffer.");
	}

	if (indices.size() > 0)
	{
		hr = indexBuffer.Initialize(&this->indices[0], this->indices.size());

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

void Grid::Draw(VertexShader& vs, PixelShader& ps)
{
	// Устанавливаем примитив как линии
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Устанавливаем буфер вершин
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	DeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Устанавливаем буфер индексов
	DeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// Устанавливаем шейдеры
	DeviceContext->VSSetShader(vs.GetShader(), nullptr, 0);
	DeviceContext->PSSetShader(ps.GetShader(), nullptr, 0);

	// Устанавливаем входной layout (если требуется)
	//DeviceContext->IASetInputLayout();

	DeviceContext->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);
}
