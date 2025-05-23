#pragma once

#include "../ShadersClass/Shaders.h"

class ShaderManager
{
public:
	ShaderManager() {};
	ShaderManager(ShaderManager const&) = delete;
	void operator =(ShaderManager const&) = delete;

	static ShaderManager& GetInstance();

	bool Initialize();

	enum ShaderType
	{
		None,
		Color,
		Texture,
		ShadowMap,
		Deferred_Opaque,
		Deferred_Light
	};

	PixelShader* GetPS(ShaderType type);
	VertexShader* GetVS(ShaderType type);
	GeomertyShader* GetGS(ShaderType type);

private:
	PixelShader colorPS;
	PixelShader texturePS;
	PixelShader deferredOpaquePS;
	PixelShader deferredLightPS;

	VertexShader colorVS;
	VertexShader textureVS;
	VertexShader deferredOpaqueVS;
	VertexShader deferredLightVS;
	VertexShader shadowMapVS;

	GeomertyShader shadowMapGS;
};

