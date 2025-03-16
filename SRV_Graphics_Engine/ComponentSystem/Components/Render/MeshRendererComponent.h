#pragma once

#pragma region Includes

#include <vector>
#include<wrl/client.h>
#include <d3d11.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "../../../DataTypes/ConstantBufferTypes.h"
#include "../../../Graphics/Buffers/ConstantBuffer.h"
#include "Mesh.h"
#include "IRenderComponent.h"

#pragma comment(lib, "d3d11.lib")  
#pragma endregion

struct ModelData;

class MeshRendererComponent : public IRenderComponent
{
public:
	MeshRendererComponent(const ModelData& modelData, GameObject* gameObject, ShaderManager::ShaderType shaderType);

	void Update(const float& deltaTime) override;
	void Render() override;

private:
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Mesh> meshes = {};

	GameObject* gameObject = nullptr;
	ConstantBuffer<CB_VS_VertexShader> constBuffer = {};

	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> texture;
};

