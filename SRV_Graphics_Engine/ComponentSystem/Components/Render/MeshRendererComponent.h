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

#pragma comment(lib, "d3d11.lib")  
#pragma endregion


class MeshRendererComponent : public IComponent
{
public:
	MeshRendererComponent(const std::string& filePath, GameObject* gameObject);

	void Update(const float& deltaTime) override;
	void Render();

private:
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Mesh> meshes = {};

	GameObject* gameObject = nullptr;
	ConstantBuffer<CB_VS_VertexShader> constBuffer = {};

	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> texture;
};

