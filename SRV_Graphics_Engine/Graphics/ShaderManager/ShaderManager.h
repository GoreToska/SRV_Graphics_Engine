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
	};

	PixelShader* GetPS(ShaderType type);
	VertexShader* GetVS(ShaderType type);

private:
	PixelShader colorPS;
	PixelShader texturePS;
	VertexShader colorVS;
	VertexShader textureVS;
};

