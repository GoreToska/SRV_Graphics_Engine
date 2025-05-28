#include "ExplosionParticleSystem.h"
#include "../../../Engine/Asserter.h"
#include "../../../Graphics/ShaderManager/ShaderManager.h"
#include "../../../Engine/Engine.h"
#include "../../../Utils/MathUtils.h"
#include <iostream>

static bool firstEmit = true;

ExplosionParticleSystem::ExplosionParticleSystem(GameObject* owner, size_t maxParticles, std::wstring texturePath) : ParticleSystem(owner, maxParticles, texturePath)
{

}

void ExplosionParticleSystem::Update(const float& deltaTime)
{
	Simulate(deltaTime);
	Sort();
}

void ExplosionParticleSystem::Simulate(const float& deltaTime)
{
	particleDataBuffer.GetData()->deltaTime = deltaTime;
	particleDataBuffer.GetData()->maxNumParticles = maxParticles;
	particleDataBuffer.GetData()->cameraPosition = SRVEngine.GetGraphics().GetCamera()->GetPositionVector();
	particleDataBuffer.GetData()->emitPosition = gameObject->GetTransform()->GetPosition();
	particleDataBuffer.ApplyChanges();

	SRVDeviceContext->CopyStructureCount(particleDataBuffer.Get(), 12, sortListBufferUAV.Get());

	UINT minOne = (UINT)-1;
	UINT zero = 0;
	SRVDeviceContext->CSSetUnorderedAccessViews(0, 1, particleBufferUAV.GetAddressOf(), nullptr);
	SRVDeviceContext->CSSetUnorderedAccessViews(1, 1, sortListBufferUAV.GetAddressOf(), &minOne);
	SRVDeviceContext->CSSetUnorderedAccessViews(2, 1, deadListBufferUAV.GetAddressOf(), &minOne);
	SRVDeviceContext->CSSetConstantBuffers(0, 1, particleDataBuffer.GetAddressOf());
	SRVDeviceContext->CSSetShader(simulateShader->GetShader(), nullptr, 0);
	SRVDeviceContext->Dispatch(32, 32, 1);

	ID3D11UnorderedAccessView* np = nullptr;
	SRVDeviceContext->CSSetUnorderedAccessViews(0, 1, &np, nullptr);
	SRVDeviceContext->CSSetUnorderedAccessViews(1, 1, &np, nullptr);
	SRVDeviceContext->CSSetUnorderedAccessViews(2, 1, &np, nullptr);
}

void ExplosionParticleSystem::Render()
{
	ParticleSystem::Render();

	cameraData.GetData()->view = SRVEngine.GetGraphics().GetCamera()->GetViewMatrix().Transpose();
	cameraData.GetData()->proj = SRVEngine.GetGraphics().GetCamera()->GetProjectionMatrix().Transpose();
	cameraData.GetData()->forward = SRVEngine.GetGraphics().GetCamera()->GetForwardVector();
	cameraData.GetData()->up = SRVEngine.GetGraphics().GetCamera()->GetUpVector();
	cameraData.ApplyChanges();

	SRVDeviceContext->CopyStructureCount(indirectArgsBuffer.Get(), 0, sortListBufferUAV.Get());

	ID3D11ShaderResourceView* views[] = { sortListBufferSRV.Get() };
	SRVDeviceContext->VSSetShaderResources(0, 1, views);
	SRVDeviceContext->VSSetShader(vertexShader->GetShader(), nullptr, 0);

	views[0] = particleBufferSRV.Get();
	SRVDeviceContext->GSSetShaderResources(0, 1, views);
	SRVDeviceContext->GSSetConstantBuffers(0, 1, cameraData.GetAddressOf());
	SRVDeviceContext->GSSetShader(geometryShader->GetShader(), nullptr, 0);

	SRVDeviceContext->PSSetShader(pixelShader->GetShader(), nullptr, 0);

	SRVDeviceContext->IASetInputLayout(nullptr);
	SRVDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	SRVDeviceContext->DrawInstancedIndirect(indirectArgsBuffer.Get(), 0);
}

void ExplosionParticleSystem::Emit(int count)
{
	InitParticles(count);

	D3D11_MAPPED_SUBRESOURCE mapped;
	auto hr = SRVDeviceContext->Map(injectionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, injectionParticleData, sizeof(Particle) * injectionBufferSize);
	SRVDeviceContext->Unmap(injectionBuffer.Get(), 0);

	particleDataBuffer.GetData()->numEmitInThisFrame = count;
	particleDataBuffer.GetData()->emitPosition = GetOrigin();
	particleDataBuffer.GetData()->maxNumParticles = maxParticles;
	particleDataBuffer.GetData()->cameraPosition = SRVEngine.GetGraphics().GetCamera()->GetPositionFloat3();
	//particleDataBuffer.GetData()->numAliveParticles = 0;

	if (firstEmit)
	{
		particleDataBuffer.GetData()->numAliveParticles = 0;
	}

	particleDataBuffer.ApplyChanges();

	if (!firstEmit)
	{
		SRVDeviceContext->CopyStructureCount(particleDataBuffer.Get(), 12, sortListBufferUAV.Get());
		std::cout << particleDataBuffer.GetData()->numAliveParticles << "\n";
	}

	UINT minOne = (UINT)-1;
	UINT zero = 0;
	SRVDeviceContext->CSSetShader(emitShader->GetShader(), nullptr, 0);
	SRVDeviceContext->CSSetUnorderedAccessViews(0, 1, particleBufferUAV.GetAddressOf(), nullptr);

	if (firstEmit)
	{
		SRVDeviceContext->CSSetUnorderedAccessViews(1, 1, sortListBufferUAV.GetAddressOf(), &zero);
		SRVDeviceContext->CSSetUnorderedAccessViews(2, 1, deadListBufferUAV.GetAddressOf(), &maxParticles);
		firstEmit = false;
	}
	else
	{
		SRVDeviceContext->CSSetUnorderedAccessViews(1, 1, sortListBufferUAV.GetAddressOf(), &minOne);
		SRVDeviceContext->CSSetUnorderedAccessViews(2, 1, deadListBufferUAV.GetAddressOf(), &minOne);
	}

	SRVDeviceContext->CSSetConstantBuffers(0, 1, particleDataBuffer.GetAddressOf());
	SRVDeviceContext->CSSetShaderResources(0, 1, injectionSRV.GetAddressOf());
	SRVDeviceContext->Dispatch(1, 1, 1);
	ID3D11UnorderedAccessView* np = nullptr;
	SRVDeviceContext->CSSetUnorderedAccessViews(0, 1, &np, nullptr);
	SRVDeviceContext->CSSetUnorderedAccessViews(1, 1, &np, nullptr);
	SRVDeviceContext->CSSetUnorderedAccessViews(2, 1, &np, nullptr);
	particleDataBuffer.GetData()->numEmitInThisFrame = 0;
}

void ExplosionParticleSystem::Initialize()
{
	ParticleSystem::Initialize();

	particleBuffer.Initialize(maxParticles);

	std::vector<SortListStruct> sl;
	sl.reserve(maxParticles);
	for (unsigned int i = 0; i < maxParticles; ++i)
	{
		sl.push_back({ i, std::numeric_limits<float>().max() });
	}

	sortListBuffer.Initialize(maxParticles, sl.data());

	std::vector<unsigned int> deadIndices;
	deadIndices.reserve(maxParticles);
	for (unsigned int i = 0; i < maxParticles; ++i)
	{
		deadIndices.push_back(i);
	}

	deadListBuffer.Initialize(maxParticles, deadIndices.data());
	particleDataBuffer.GetData()->numAliveParticles = 0;



	particleDataBuffer.Initialize();

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = maxParticles;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
	ThrowIfFailed(SRVDevice->CreateUnorderedAccessView(particleBuffer.Get(), &uavDesc, particleBufferUAV.GetAddressOf()), "Failed.");

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.NumElements = maxParticles;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	ThrowIfFailed(SRVDevice->CreateShaderResourceView(particleBuffer.Get(), &srvDesc, particleBufferSRV.GetAddressOf()), "Failed.");

	uavDesc = {};
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = maxParticles;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
	ThrowIfFailed(SRVDevice->CreateUnorderedAccessView(sortListBuffer.Get(), &uavDesc, sortListBufferUAV.GetAddressOf()), "Failed.");

	srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.NumElements = maxParticles;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	ThrowIfFailed(SRVDevice->CreateShaderResourceView(sortListBuffer.Get(), &srvDesc, sortListBufferSRV.GetAddressOf()), "Failed.");

	uavDesc = {};
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = maxParticles;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
	ThrowIfFailed(SRVDevice->CreateUnorderedAccessView(deadListBuffer.Get(), &uavDesc, deadListBufferUAV.GetAddressOf()), "Failed.");

	srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = maxParticles;
	ThrowIfFailed(SRVDevice->CreateShaderResourceView(deadListBuffer.Get(), &srvDesc, deadListBufferSRV.GetAddressOf()), "Failed.");

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = 16;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	desc.CPUAccessFlags = 0;

	D3D11_DRAW_INSTANCED_INDIRECT_ARGS args = {};
	args.VertexCountPerInstance = 0;
	args.InstanceCount = 1;
	args.StartVertexLocation = 0;
	args.StartInstanceLocation = 0;

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = &args;
	ThrowIfFailed(SRVDevice->CreateBuffer(&desc, &srd, indirectArgsBuffer.GetAddressOf()), "Failed.");

	cameraData.Initialize();

	simulateShader = ShaderManager::GetInstance().GetParticleSimulateCS(ShaderManager::PSCS_Explosion_Simulate);
	emitShader = ShaderManager::GetInstance().GetParticleEmitCS(ShaderManager::PECS_Explosion_Emit);
	vertexShader = ShaderManager::GetInstance().GetVS(ShaderManager::Particle);
	pixelShader = ShaderManager::GetInstance().GetPS(ShaderManager::Particle);
	geometryShader = ShaderManager::GetInstance().GetGS(ShaderManager::Particle);
}

void ExplosionParticleSystem::InitParticle(int index)
{
	if (index < 0 || index >= injectionBufferSize)
		return;

	Particle p;
	p.acceleration = Vector3D(0.0f, -0.981f, 0.0f) * 0.00005f;
	p.initialColor = Vector4D(1.0f, 1.0f, 1.0f, 1.0f);
	p.endColor = Vector4D(1.0f, 0.0f, 1.0f, 1.0f);
	p.initialSize = 0.02f;
	p.endSize = 1.0f;
	float rw = GetRandomFloat(0.3f, 2.0f);
	p.initialWeight = rw;
	p.endWeight = rw;
	p.lifetime = 0.0f;
	p.maxLifetime = GetRandomFloat(1000.0f, 2000.0f);
	p.position = GetOrigin();
	p.prevPosition = GetOrigin();

	float theta = GetRandomFloat(0.0f, DirectX::XM_2PI);  // Азимутальный угол
	float phi = GetRandomFloat(0.0f, DirectX::XM_PI);     // Полярный угол
	float speed = GetRandomFloat(0.3f, 2.0f) * 0.01f;

	float rf1 = GetRandomFloat(0.0f, 1.0f * DirectX::XM_2PI);
	float rf2 = GetRandomFloat(0.0f, 1.0f * DirectX::XM_2PI);
	//p.velocity = GetRandomFloat(0.3f, 2.0f) * Vector3D(std::sin(rf1), GetRandomFloat(1.0f, 3.0f), std::cos(rf2)) * 0.01;
	p.velocity = Vector3D(
		speed * sin(phi) * cos(theta),
		speed * cos(phi),
		speed * sin(phi) * sin(theta)
	) * 3;

	injectionParticleData[index] = p;
}
