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
	};

	UINT numElements = ARRAYSIZE(textureLayoutDesc);

	if (!textureVS.Initialize(shaderFolder + L"TextureVertexShader.cso", textureLayoutDesc, numElements))
		return false;

	if (!texturePS.Initialize(shaderFolder + L"TexturePixelShader.cso"))
		return false;


	D3D11_INPUT_ELEMENT_DESC particleLayoutDesc[] =
	{
		{"SV_VertexID", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT particleLayoutElements = ARRAYSIZE(particleLayoutDesc);

	if (!particleVS.Initialize(shaderFolder + L"explosionVS.cso", particleLayoutDesc, particleLayoutElements))
		return false;

	if (!particlePS.Initialize(shaderFolder + L"explosionPS.cso"))
		return false;

	if (!explosionParticleGS.Initialize(shaderFolder + L"explosionGS.cso"))
		return false;

	if (!explosionSimulateShader.Initialize(shaderFolder + L"explosionSimulateCS.cso"))
		return false;

	if (!explosionEmitShader.Initialize(shaderFolder + L"explosionEmitCS.cso"))
		return false;

	// --- Texture Shaders ---

	// --- Deferred Opaque Shaders ---
	D3D11_INPUT_ELEMENT_DESC deferredOpaqueLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT deferredOpaqueNumElements = ARRAYSIZE(deferredOpaqueLayoutDesc);

	if (!deferredOpaqueVS.Initialize(shaderFolder + L"deferredOpaqueVS.cso", deferredOpaqueLayoutDesc, deferredOpaqueNumElements))
		return false;

	if (!deferredOpaquePS.Initialize(shaderFolder + L"deferredOpaquePS.cso"))
		return false;
	// --- Deferred Opaque Shaders ---

	// --- Deferred Light Shaders ---
	D3D11_INPUT_ELEMENT_DESC deferredLightLayoutDesc[] =
	{
		{"SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT deferredLightNumElements = ARRAYSIZE(deferredLightLayoutDesc);

	if (!deferredLightVS.Initialize(shaderFolder + L"deferredLightVS.cso", deferredLightLayoutDesc, deferredLightNumElements))
		return false;

	if (!deferredLightPS.Initialize(shaderFolder + L"deferredLightPS.cso"))
		return false;
	// --- Deferred Light Shaders ---


	// --- Color Shaders ---
	D3D11_INPUT_ELEMENT_DESC colorLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT colorNumElements = ARRAYSIZE(colorLayoutDesc);

	/*if (!colorVS.Initialize(shaderFolder + L"ColorVertexShader.cso", colorLayoutDesc, colorNumElements))
		return false;

	if (!colorPS.Initialize(shaderFolder + L"ColorPixelShader.cso"))
		return false;*/

	// --- Color Shaders ---

	// --- ShadowMap Shaders ---
	D3D11_INPUT_ELEMENT_DESC shadowMapLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT shadowMapNumElements = ARRAYSIZE(shadowMapLayoutDesc);

	if (!shadowMapVS.Initialize(shaderFolder + L"ShadowMapVS.cso", shadowMapLayoutDesc, shadowMapNumElements))
		return false;


	if (!shadowMapGS.Initialize(shaderFolder + L"ShadowMapGS.cso"))
		return false;

	// --- ShadowMap Shaders ---


	// --- Compute Shaders ---

	if (!bitonicSortShader.Initialize(shaderFolder + L"bitonicSortCS.cso"))
		return false;

	if (!bitonicTransposeShader.Initialize(shaderFolder + L"bitonicTransposeCS.cso"))
		return false;
	// --- Compute Shaders ---

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
		return nullptr;
	case Deferred_Opaque:
		return &deferredOpaquePS;
	case Deferred_Light:
		return &deferredLightPS;
	case Particle:
		return &particlePS;
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
	case Deferred_Opaque:
		return &deferredOpaqueVS;
	case Deferred_Light:
		return &deferredLightVS;
	case Particle:
		return &particleVS;
	}
}

GeomertyShader* ShaderManager::GetGS(ShaderType type)
{
	switch (type)
	{
	case None:
		return nullptr;
	case Color:
		return nullptr;
	case Texture:
		return nullptr;
	case ShadowMap:
		return &shadowMapGS;
	case Particle:
		return &explosionParticleGS;
	}
}

ComputeShader* ShaderManager::GetCS(ComputeShaderType type)
{
	switch (type)
	{
	case ShaderManager::None:
		return nullptr;
	case ShaderManager::CS_BitonicSort:
		return &bitonicSortShader;
	case ShaderManager::CS_BitonicTranspose:
		return &bitonicTransposeShader;
	default:
		return nullptr;
	}

	return nullptr;
}

ComputeShader* ShaderManager::GetParticleEmitCS(ParticleEmitShaderType type)
{
	switch (type)
	{
	case ShaderManager::PECS_None:
		return nullptr;
	case ShaderManager::PECS_Explosion_Emit:
		return &explosionEmitShader;
	default:
		return nullptr;
	}
}

ComputeShader* ShaderManager::GetParticleSimulateCS(ParticleSimulateShaderType type)
{
	switch (type)
	{
	case ShaderManager::PSCS_None:
		return nullptr;
	case ShaderManager::PSCS_Explosion_Simulate:
		return &explosionSimulateShader;
	default:
		return nullptr;
	}
}

GeomertyShader* ShaderManager::GetParticleGS(ParticleGeometryShaderType type)
{
	switch (type)
	{
	case ShaderManager::PGS_None:
		return nullptr;
	case ShaderManager::PGS_Explosion_Geometry:
		return &explosionParticleGS;
	default:
		return nullptr;
	}

}
