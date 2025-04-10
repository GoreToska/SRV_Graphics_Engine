#include "ShaderManager.h"

ShaderManager& ShaderManager::GetInstance()
{
	static ShaderManager shaderManager;
	return shaderManager;
}

bool ShaderManager::Initialize()
{
	std::wstring shaderFolder = L"../x64/Debug/";

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


	// --- Texture Shaders ---
	D3D11_INPUT_ELEMENT_DESC textureLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"LIGHT_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numElements = ARRAYSIZE(textureLayoutDesc);

	if (!textureVS.Initialize(shaderFolder + L"TextureVertexShader.cso", textureLayoutDesc, numElements))
		return false;

	if (!texturePS.Initialize(shaderFolder + L"TexturePixelShader.cso"))
		return false;
	// --- Texture Shaders ---

	// --- Color Shaders ---
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
	// --- Color Shaders ---

	// --- ShadowMap Shaders ---
	D3D11_INPUT_ELEMENT_DESC shadowMapLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT shadowMapNumElements = ARRAYSIZE(shadowMapLayoutDesc);

	if (!shadowMapVS.Initialize(shaderFolder + L"ShadowMapVS.cso", shadowMapLayoutDesc, shadowMapNumElements))
		return false;

	if (!shadowMapPS.Initialize(shaderFolder + L"ShadowMapPS.cso"))
		return false;
	// --- ShadowMap Shaders ---

	return true;
}

PixelShader* ShaderManager::GetPS(ShaderType type)
{
	switch (type)
	{
	case None:
		return nullptr;
	case Color:
		return &colorPS;
	case Texture:
		return &texturePS;
	case ShadowMap:
		return &shadowMapPS;
	default:
		break;
	}
}

VertexShader* ShaderManager::GetVS(ShaderType type)
{
	switch (type)
	{
	case None:
		return nullptr;
	case Color:
		return &colorVS;
	case Texture:
		return &textureVS;
	case ShadowMap:
		return &shadowMapVS;
	}
}
