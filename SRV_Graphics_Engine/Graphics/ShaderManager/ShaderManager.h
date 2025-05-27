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
		Deferred_Light,
		Particle,
	};

	enum ComputeShaderType
	{
		CS_None,
		CS_BitonicSort,
		CS_BitonicTranspose,
	};

	enum ParticleEmitShaderType
	{
		PECS_None,
		PECS_Explosion_Emit,
	};

	enum ParticleSimulateShaderType
	{
		PSCS_None,
		PSCS_Explosion_Simulate,
	};

	enum ParticleGeometryShaderType
	{
		PGS_None,
		PGS_Explosion_Geometry,
	};

	PixelShader* GetPS(ShaderType type);
	VertexShader* GetVS(ShaderType type);
	GeomertyShader* GetGS(ShaderType type);
	ComputeShader* GetCS(ComputeShaderType type);
	ComputeShader* GetParticleEmitCS(ParticleEmitShaderType type);
	ComputeShader* GetParticleSimulateCS(ParticleSimulateShaderType type);
	GeomertyShader* GetParticleGS(ParticleGeometryShaderType type);

private:
	VertexShader colorVS;
	VertexShader textureVS;
	VertexShader deferredOpaqueVS;
	VertexShader deferredLightVS;
	VertexShader shadowMapVS;
	VertexShader particleVS; 

	PixelShader colorPS;
	PixelShader texturePS;
	PixelShader deferredOpaquePS;
	PixelShader deferredLightPS;
	PixelShader particlePS; 

	GeomertyShader shadowMapGS;
	GeomertyShader explosionParticleGS; 

	ComputeShader bitonicSortShader;
	ComputeShader bitonicTransposeShader;

	ComputeShader explosionSimulateShader; 
	ComputeShader explosionEmitShader; 

};

