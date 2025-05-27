#pragma once
#include "ParticleSystem.h"

class ExplosionParticleSystem :public ParticleSystem
{
public:
	ExplosionParticleSystem(GameObject* owner, size_t maxParticles);
	void Update(const float& deltaTime) override;
	void Simulate(const float& deltaTime) override;
	void Render() override;
	void Emit(int count) override;
	void Initialize() override;

protected:
	void InitParticle(int index) override;

	ComputeShader* simulateShader;
	ComputeShader* emitShader;
	GeomertyShader* geometryShader;
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	ConstantBufferW<ParticleData> particleDataBuffer;
	ConstantBufferW<CameraData> cameraData;
};
	
