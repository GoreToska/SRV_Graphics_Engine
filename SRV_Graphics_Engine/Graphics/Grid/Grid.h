#pragma once

#include <d3d11.h>
#include <vector>

#include "../../DataTypes/Vertex.h"
#include "../Buffers/ConstantBuffer.h"
#include "../../DataTypes/ConstantBufferTypes.h"
#include "../Buffers/IndexBuffer.h"
#include "../Buffers/VertexBuffer.h"
#include "../ShadersClass/Shaders.h"

class Grid
{
public:
	Grid();

	void Draw();

private:
	std::vector<CVertex> vertexes{};
	std::vector<DWORD> indexes{};

	const float gridSize = 150.0f;    
	const int gridLines = 10;  // Количество линий в сетке
	const float spacing = gridSize / gridLines; // Расстояние между линиями

	VertexBuffer<CVertex> vertexBuffer = {};
	IndexBuffer indexBuffer = {};
	ConstantBuffer<MatrixBuffer> constBuffer = {};
};

