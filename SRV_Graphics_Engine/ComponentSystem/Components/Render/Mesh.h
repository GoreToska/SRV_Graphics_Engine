#pragma once

#include <d3d11.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include <string>

#include "../../GameObject.h"
#include "../../../Graphics/Buffers/IndexBuffer.h"
#include "../../../Graphics/Buffers/VertexBuffer.h"
#include "../../../Graphics/Buffers/ConstantBuffer.h"
#include "../../../DataTypes/Vertex.h"
#include "../../../DataTypes/ConstantBufferTypes.h"


#include "IRenderComponent.h"

class TextureMeshComponent : public IRenderComponent
{
public:
	TextureMeshComponent(GameObject* gameObject, std::vector<TVertex> vertexes, ShaderManager::ShaderType type, std::vector<DWORD> indexes = {});
	TextureMeshComponent(GameObject* gameObject, std::vector<TVertex> vertexes, std::wstring texturePath, ShaderManager::ShaderType type, std::vector<DWORD> indexes = {});
	TextureMeshComponent(const TextureMeshComponent& mesh);

	void Update(const float& deltaTime) override;
	void Render(bool setShaders = true) override;
	void RenderForShadows() override;

	virtual int GetVertexCount() const override;
	virtual void SetVertexBufferContext() override;

private:
	std::vector<TVertex> vertexes = {};
	std::vector<DWORD> indexes = {};

	VertexBuffer<TVertex> vertexBuffer = {};
	IndexBuffer indexBuffer = {};

	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> texture;
};

