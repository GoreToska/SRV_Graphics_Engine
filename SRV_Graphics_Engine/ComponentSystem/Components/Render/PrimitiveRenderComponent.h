#pragma once

#include <d3d11.h>
#include <vector>

#include "IRenderComponent.h"
#include "../../GameObject.h"
#include "../../../DataTypes/Vertex.h"
#include "../../../DataTypes/ConstantBufferTypes.h"
#include "../../../Graphics/Buffers/ConstantBuffer.h"
#include "../../../Graphics/Buffers/IndexBuffer.h"
#include "../../../Graphics/Buffers/VertexBuffer.h"


class PrimitiveRenderComponent : public IRenderComponent
{
public:
	PrimitiveRenderComponent(GameObject* gameObject, std::vector<CVertex> vertexes, ShaderManager::ShaderType type, std::vector<DWORD> indexes = {});
	void Update(const float& deltaTime) override;
	void Render() override;

private:
	std::vector<CVertex> vertexes = {};
	std::vector<DWORD> indexes = {};

	VertexBuffer<CVertex> vertexBuffer = {};
	IndexBuffer indexBuffer = {};
};

