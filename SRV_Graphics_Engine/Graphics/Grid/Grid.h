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

	void Draw(VertexShader& vs, PixelShader& ps);

private:
	std::vector<CVertex> vertices{};
	std::vector<DWORD> indices{};

	const float gridSize = 10.0f;    
	const float gridSpacing = 1.0f;

	VertexBuffer<CVertex> vertexBuffer = {};
	IndexBuffer indexBuffer = {};
	ConstantBuffer<CB_VS_VertexShader> constBuffer = {};
};

