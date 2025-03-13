#include "MeshRendererComponent.h"

#include <WICTextureLoader.h>
#include <iostream>
#include "../../../Engine/Engine.h"
#include "../../../DataTypes/ModelData.h"

MeshRendererComponent::MeshRendererComponent(const ModelData& modelData, GameObject* gameObject)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(Device, modelData.texturePath.c_str(), nullptr, texture.GetAddressOf());
	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create texture.");
	}

	this->gameObject = gameObject;

	hr = constBuffer.Initialize();
	if (FAILED(hr))
	{
		Logger::LogError(hr, "Failed to create const buffer.");
	}

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

void MeshRendererComponent::Update(const float& deltaTime)
{
	// TODO: what to do here?
}

void MeshRendererComponent::Render()
{
	DeviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());

	DirectX::XMVECTOR orientation = gameObject->GetTransform()->GetOrientation();

	constBuffer.GetData()->matrix = DirectX::XMMatrixScaling(
		gameObject->GetTransform()->GetScale().x,
		gameObject->GetTransform()->GetScale().y,
		gameObject->GetTransform()->GetScale().z);

	constBuffer.GetData()->matrix *= DirectX::XMMatrixRotationQuaternion(orientation);

	constBuffer.GetData()->matrix *= DirectX::XMMatrixTranslation(
		gameObject->GetTransform()->GetPosition().x,
		gameObject->GetTransform()->GetPosition().y,
		gameObject->GetTransform()->GetPosition().z);

	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetWorldMatrix();
	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetCamera()->GetViewMatrix();
	constBuffer.GetData()->matrix *= SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix();

	constBuffer.GetData()->matrix = DirectX::XMMatrixTranspose(constBuffer.GetData()->matrix);

	if (constBuffer.ApplyChanges())
		DeviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());

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
	std::vector<Vertex> vertexes = {};
	std::vector<DWORD> indexes = {};

	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex = {};
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
