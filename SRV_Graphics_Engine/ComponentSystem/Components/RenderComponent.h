#pragma once

#pragma region Includes
#include <vector>
#include<wrl/client.h>
#include <d3d11.h>

#include "../../DataTypes/Vertex.h"
#include "../../Graphics/Buffers/ConstantBuffer.h"
#include "../../Graphics/Buffers/VertexBuffer.h"
#include "../../Graphics/Buffers/IndexBuffer.h"
#include "../../DataTypes/ConstantBufferTypes.h"
#include "TransformComponent.h"
#include "../GameObject.h"

#pragma comment(lib, "d3d11.lib")  
#pragma endregion


class RenderComponent : public IComponent
{
public:
	RenderComponent(TransformComponent* transform, std::vector<Vertex3D> vertices, std::vector<DWORD> indexes = {});

	void Update(float deltaTime) override;

	void Render();

private:
	std::vector<Vertex3D> vertices = {};
	std::vector<DWORD> indexes = {};
	TransformComponent* transform = nullptr;

	VertexBuffer<Vertex3D> vertexBuffer = {};
	IndexBuffer indexBuffer = {};
	ConstantBuffer<CB_VS_VertexShader> constBuffer = {};
};

