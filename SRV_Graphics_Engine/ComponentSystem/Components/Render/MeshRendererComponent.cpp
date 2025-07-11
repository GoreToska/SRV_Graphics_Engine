#include "MeshRendererComponent.h"

#include <WICTextureLoader.h>
#include <iostream>
#include "../../../Engine/Engine.h"
#include "../../../DataTypes/ModelData.h"

MeshRendererComponent::MeshRendererComponent(const ModelData& modelData, GameObject* gameObject, ShaderManager::ShaderType shaderType)
	: IRenderComponent(gameObject, shaderType)
{
	if (modelData.texturePath != L"")
	{
		ThrowIfFailed(DirectX::CreateWICTextureFromFile(SRVDevice, modelData.texturePath.c_str(), nullptr, texture.GetAddressOf()),
			"Failed to create texture.");
	}

	if (modelData.modelPath != "")
	{
		try
		{
			if (!LoadModel(modelData.modelPath))
				return;
		}
		catch (const std::exception& ex)
		{
			Logger::LogError(ex.what());
		}
	}
}

void MeshRendererComponent::Update(const float& deltaTime)
{
	// TODO: what to do here?
}

void MeshRendererComponent::Render()
{
	IRenderComponent::Render();

	SRVDeviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
	SRVDeviceContext->PSSetShaderResources(1, 1, SRVEngine.GetGraphics().GetAllLights()[0]->GetShadowSRVAddress());
	SRVDeviceContext->PSSetShaderResources(2, 1, decalTexture.GetAddressOf());


	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Render();
	}
}

void MeshRendererComponent::RenderForShadows()
{
	IRenderComponent::RenderForShadows();

	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshes[i].RenderForShadows();
	}
}

int MeshRendererComponent::GetVertexCount() const
{
	return 0;
}

void MeshRendererComponent::SetVertexBufferContext()
{
	return;
}

void MeshRendererComponent::SetDecal(ID3D11ShaderResourceView* texture)
{
	decalTexture = texture;
}

bool MeshRendererComponent::LoadModel(const std::string& filePath)
{
	Assimp::Importer importer{};
	const aiScene* pScene = importer.ReadFile(filePath,
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

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

TextureMeshComponent MeshRendererComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<TVertex> vertexes = {};
	std::vector<DWORD> indexes = {};

	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		TVertex vertex = {};

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;


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

	return TextureMeshComponent(gameObject, vertexes, shaderType, indexes);
}
