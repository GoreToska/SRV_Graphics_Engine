#include "ExplosionParticleSystem.h"
#include "../../../Engine/Asserter.h"
#include "../../../Graphics/ShaderManager/ShaderManager.h"
#include "../../../Engine/Engine.h"

static bool firstEmit = true;

ExplosionParticleSystem::ExplosionParticleSystem(GameObject* owner, size_t maxParticles) : ParticleSystem(owner, maxParticles)
{

}

void ExplosionParticleSystem::Update(const float& deltaTime)
{
	Emit(emitionRate);
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
	particleDataBuffer.GetData()->numEmitInThisFrame = count;
	particleDataBuffer.GetData()->emitPosition = GetOrigin();
	particleDataBuffer.GetData()->maxNumParticles = maxParticles;
	particleDataBuffer.GetData()->cameraPosition = SRVEngine.GetGraphics().GetCamera()->GetPositionFloat3();

	if (firstEmit)
	{
		particleDataBuffer.GetData()->numAliveParticles = 0;
	}

	particleDataBuffer.ApplyChanges();

	if (!firstEmit)
	{
		SRVDeviceContext->CopyStructureCount(particleDataBuffer.Get(), 12, sortListBufferUAV.Get());
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

	// TODO: what?
	std::vector<unsigned int> deadIndices/*(maxParticles)*/;
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
}
