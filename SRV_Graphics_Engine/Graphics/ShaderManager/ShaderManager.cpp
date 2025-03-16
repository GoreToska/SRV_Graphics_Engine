#include "ShaderManager.h"

ShaderManager& ShaderManager::GetInstance()
{
	static ShaderManager shaderManager;
	return shaderManager;
}

bool ShaderManager::Initialize()
{
	std::wstring shaderFolder = L"";

#pragma region ShaderPathMacro
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG
#ifdef _WIN64
		shaderFolder = L"../x64/Debug/";
#else
		shaderFolder = L"../Debug/";
#endif // _WIN64 or _WIN32

#else // RELEASE
#ifdef _WIN64
		shaderFolder = L"../x64/Release/";
#else
		shaderFolder = L"../Release/";
#endif
#endif // _DEBUG
	}
#pragma endregion

	D3D11_INPUT_ELEMENT_DESC textureLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numElements = ARRAYSIZE(textureLayoutDesc);

	if (!textureVS.Initialize(shaderFolder + L"TextureVertexShader.cso", textureLayoutDesc, numElements))
		return false;

	if (!texturePS.Initialize(shaderFolder + L"TexturePixelShader.cso"))
		return false;

	D3D11_INPUT_ELEMENT_DESC colorLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT colorNumElements = ARRAYSIZE(colorLayoutDesc);

	if (!colorVS.Initialize(shaderFolder + L"ColorVertexShader.cso", colorLayoutDesc, colorNumElements))
		return false;

	if (!colorPS.Initialize(shaderFolder + L"ColorPixelShader.cso"))
		return false;

	return true;
}

PixelShader* ShaderManager::GetPS(ShaderType type)
{
	switch (type)
	{
	case ShaderManager::None:
		return nullptr;
	case ShaderManager::Color:
		return &colorPS;
	case ShaderManager::Texture:
		return &texturePS;
	default:
		break;
	}
}

VertexShader* ShaderManager::GetVS(ShaderType type)
{
	switch (type)
	{
	case ShaderManager::None:
		return nullptr;
	case ShaderManager::Color:
		return &colorVS;
	case ShaderManager::Texture:
		return &textureVS;
	}
}
