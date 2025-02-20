#pragma once


#include <vector>
#include<wrl/client.h>
#include <d3d11.h>

#include "../../DataTypes/Vertex.h"
#include "../Buffers/VertexBuffer.h"
#include "../Buffers/IndexBuffer.h"
#include "../../DataTypes/ConstantBufferTypes.h"
#include "../Buffers/ConstantBuffer.h"

#pragma comment(lib, "d3d11.lib")


class RenderComponent
{
public:
	RenderComponent(std::vector<Vertex3D> vertices, std::vector<DWORD> indexes = {});
	void Render();
	void SetPosition(const Vector3D& position);
	void MovePosition(const Vector3D& delta);
	Vector3D GetPosition();

	int id = 0;

private:
	std::vector<Vertex3D> vertices = {};
	std::vector<DWORD> indexes = {};

	Vector3D position = {};

	VertexBuffer<Vertex3D> vertexBuffer = {};
	IndexBuffer indexBuffer = {};
	ConstantBuffer<CB_VS_VertexShader> constBuffer = {};
};

