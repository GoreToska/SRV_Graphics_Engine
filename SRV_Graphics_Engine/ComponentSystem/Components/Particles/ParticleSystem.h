#pragma once
#include "../../GameObject.h"
#include "../../../DataTypes/Particle.h"
#include "../../../DataTypes/ConstantBufferTypes.h"
#include "../../../Graphics/Buffers/ConstantBuffer.h"
#include "../../../Graphics/ShadersClass/Shaders.h"

#include <wrl/client.h>
#include <d3d11.h>

struct ParticleData
{
public:
	float deltaTime;
	unsigned int maxNumParticles;
	unsigned int numEmitInThisFrame;
	unsigned int numAliveParticles;
	Vector3D emitPosition;
	int _1;
	Vector3D cameraPosition;
	int _2;
};

struct CameraData
{
	Matrix view;
	Matrix proj;
	Vector3D forward;
	float _1;
	Vector3D up;
	float _2;
};

class ParticleSystem : public IComponent
{
public:
	ParticleSystem(GameObject* owner, size_t maxParticles);

	virtual void Update(const float& deltaTime) override;

	virtual void Simulate(const float& deltaTime);
	virtual void Render();
	virtual void Emit(int count);
	virtual void Initialize();
	void SetConstBuffer(UINT iLevel, UINT iLevelMask, UINT iWidth, UINT iHeight);
	void Sort();

	Vector3D GetOrigin() const;

protected:
	virtual void InitParticles(int count);
	virtual void InitParticle(int index) = 0;

	GameObject* gameObject;

	std::vector<Particle> particleList = {};
	float emitionRate = 10;
	unsigned int maxParticles = 1000;


	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particleBufferUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleBufferSRV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> sortListBufferUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sortListBufferSRV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> deadListBufferUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> deadListBufferSRV;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indirectArgsBuffer;

	//Microsoft::WRL::ComPtr<ID3D11Buffer> constSortBuf;
	ConstantBufferW<CS_Sort> constSortBuf;
	ConstantBufferRW<SortListStruct> sortListBuffer;
	ConstantBufferRW<unsigned int> deadListBuffer;
	ConstantBufferRW<Particle> particleBuffer;

	ComputeShader* bitonicSortShader;
	ComputeShader* bitonicTransposeShader;

	const UINT BITONIC_BLOCK_SIZE = 512;
	const UINT TRANSPOSE_BLOCK_SIZE = 16;
	const UINT MATRIX_WIDTH = BITONIC_BLOCK_SIZE;

	static constexpr int injectionBufferSize = 1024;
	const int maxNumInjectionParticles = injectionBufferSize;
	Particle injectionParticleData[injectionBufferSize];
	Microsoft::WRL::ComPtr<ID3D11Buffer> injectionBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> injectionSRV;
};

