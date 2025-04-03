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
	};

	PixelShader* GetPS(ShaderType type);
	VertexShader* GetVS(ShaderType type);

private:
	PixelShader colorPS;
	PixelShader texturePS;
	PixelShader shadowMapPS;

	VertexShader colorVS;
	VertexShader textureVS;
	VertexShader shadowMapVS;
};

