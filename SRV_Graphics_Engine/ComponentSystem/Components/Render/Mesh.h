#pragma once

#include <d3d11.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

#include "../../GameObject.h"
#include "../../../Graphics/Buffers/IndexBuffer.h"
#include "../../../Graphics/Buffers/VertexBuffer.h"
#include "../../../Graphics/Buffers/ConstantBuffer.h"
#include "../../../DataTypes/Vertex.h"
#include "../../../DataTypes/ConstantBufferTypes.h"

class Mesh
{
public:
	Mesh(GameObject* gameObject, std::vector<TVertex> vertexes, std::vector<DWORD> indexes = {});
	Mesh(const Mesh& mesh);

	void Render();

private:
	std::vector<TVertex> vertexes = {};
	std::vector<DWORD> indexes = {};
	GameObject* gameObject;

	VertexBuffer<TVertex> vertexBuffer = {};
	IndexBuffer indexBuffer = {};
	ConstantBuffer<CB_VS_VertexShader> constBuffer = {};
};

