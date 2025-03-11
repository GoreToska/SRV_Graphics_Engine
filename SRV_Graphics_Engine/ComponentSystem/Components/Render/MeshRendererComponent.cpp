#include "MeshRendererComponent.h"

#include <WICTextureLoader.h>
#include <iostream>


MeshRendererComponent::MeshRendererComponent(const std::string& filePath, GameObject* gameObject)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(Device, L"Data\\Models\\seamless_grass.jpg", nullptr, texture.GetAddressOf());

	this->gameObject = gameObject;

	try
	{
		if (!LoadModel(filePath))
			return;
	}
	catch (const std::exception& ex)
	{
		Logger::LogError(ex.what());
	}



}

void MeshRendererComponent::Update(const float& deltaTime)
{
	// TODO: what to do here?
}

void MeshRendererComponent::Render()
{
	DeviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());

	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Render();
	}
}

bool MeshRendererComponent::LoadModel(const std::string& filePath)
{
	Assimp::Importer importer{};
	const aiScene* pScene = importer.ReadFile(filePath,
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	/*if (importer.GetErrorString() != " ")
		Logger::LogError(importer.GetErrorString());*/

	if (pScene == nullptr)
		return false;

	ProcessNode(pScene->mRootNode, pScene);

	return true;
}

void MeshRendererComponent::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh MeshRendererComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex3D> vertexes = {};
	std::vector<DWORD> indexes = {};

	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex3D vertex = {};
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.textCoord.x = mesh->mTextureCoords[0][i].x;
			vertex.textCoord.y = mesh->mTextureCoords[0][i].y;
		}

		vertexes.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; ++j)
		{
			indexes.push_back(face.mIndices[j]);
		}
	}

	return Mesh(gameObject, vertexes, indexes);
}
