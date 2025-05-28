#include "ParticleSystem.h"
#include "../../../Graphics/ShaderManager/ShaderManager.h"
#include <WICTextureLoader.h>
#include "../../../Engine/Asserter.h"


ParticleSystem::ParticleSystem(GameObject* owner, size_t maxParticles, std::wstring texturePath) : gameObject(owner), maxParticles(maxParticles)
{

	ThrowIfFailed(DirectX::CreateWICTextureFromFileEx(SRVDevice, SRVDeviceContext, texturePath.c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		DirectX::WIC_LOADER_DEFAULT, nullptr, particleTexture.GetAddressOf()),
		"Failed to create texture.");

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.MaxLOD = INT_MAX;

	SRVDevice->CreateSamplerState(&samplerDesc, &sampler);

	Initialize();
}

void ParticleSystem::Update(const float& deltaTime)
{
}

void ParticleSystem::Simulate(const float& deltaTime)
{
}

void ParticleSystem::Render()
{
	SRVDeviceContext->PSSetShaderResources(0, 1, particleTexture.GetAddressOf());
	SRVDeviceContext->PSSetSamplers(0, 1, sampler.GetAddressOf());
}

void ParticleSystem::Emit(int count)
{
}

void ParticleSystem::Initialize()
{
	bitonicSortShader = ShaderManager::GetInstance().GetCS(ShaderManager::CS_BitonicSort);
	bitonicTransposeShader = ShaderManager::GetInstance().GetCS(ShaderManager::CS_BitonicTranspose);

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = sizeof(Particle) * injectionBufferSize;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = sizeof(Particle);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = SRVDevice->CreateBuffer(&desc, 0, injectionBuffer.GetAddressOf());

	constSortBuf.Initialize();

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = injectionBufferSize;
	SRVDevice->CreateShaderResourceView(injectionBuffer.Get(), &srvDesc, &injectionSRV);
}

void ParticleSystem::SetConstBuffer(UINT iLevel, UINT iLevelMask, UINT iWidth, UINT iHeight)
{
	constSortBuf.GetData()->iLevel = iLevel;
	constSortBuf.GetData()->iLevelMask = iLevelMask;
	constSortBuf.GetData()->iWidth = iWidth;
	constSortBuf.GetData()->iHeight = iHeight;
	constSortBuf.ApplyChanges();
	SRVDeviceContext->CSSetConstantBuffers(0, 1, constSortBuf.GetAddressOf());
}

// Bitonic Sort
void ParticleSystem::Sort()
{
	auto context = SRVDeviceContext;
	const UINT MATRIX_HEIGHT = maxParticles / BITONIC_BLOCK_SIZE;

	// Sort the data
	// First sort the rows for the levels <= to the block size
	for (UINT level = 2; level <= BITONIC_BLOCK_SIZE; level = level * 2)
	{
		SetConstBuffer(level, level, MATRIX_HEIGHT, MATRIX_WIDTH);

		// Sort the row data
		context->CSSetUnorderedAccessViews(0, 1, sortListBufferUAV.GetAddressOf(), nullptr);
		context->CSSetShader(bitonicSortShader->GetShader(), nullptr, 0);

		if (maxParticles / BITONIC_BLOCK_SIZE > 0u)
			context->Dispatch(maxParticles / BITONIC_BLOCK_SIZE, 1, 1);
	}

	// Then sort the rows and columns for the levels > than the block size
	// Transpose. Sort the Columns. Transpose. Sort the Rows.
	for (UINT level = (BITONIC_BLOCK_SIZE * 2); level <= maxParticles; level = level * 2)
	{
		SetConstBuffer(level / BITONIC_BLOCK_SIZE, level, MATRIX_WIDTH, MATRIX_HEIGHT);

		// Transpose the data from buffer 1 into buffer 2
		context->CSSetUnorderedAccessViews(0, 1, sortListBufferUAV.GetAddressOf(), nullptr);
		context->CSSetShader(bitonicTransposeShader->GetShader(), nullptr, 0);

		if (MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE > 0 && MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE > 0)
			context->Dispatch(MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE, MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE, 1);

		// Sort the transposed column data
		context->CSSetShader(bitonicSortShader->GetShader(), nullptr, 0);
		if (maxParticles / BITONIC_BLOCK_SIZE > 0)
			context->Dispatch(maxParticles / BITONIC_BLOCK_SIZE, 1, 1);

		SetConstBuffer(BITONIC_BLOCK_SIZE, level, MATRIX_HEIGHT, MATRIX_WIDTH);

		// Transpose the data from buffer 2 back into buffer 1
		context->CSSetUnorderedAccessViews(0, 1, sortListBufferUAV.GetAddressOf(), nullptr);
		context->CSSetShader(bitonicTransposeShader->GetShader(), nullptr, 0);
		if (MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE > 0 && MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE > 0)
			context->Dispatch(MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE, MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE, 1);

		// Sort the row data
		context->CSSetShader(bitonicSortShader->GetShader(), nullptr, 0);
		if (maxParticles / BITONIC_BLOCK_SIZE > 0)
			context->Dispatch(maxParticles / BITONIC_BLOCK_SIZE, 1, 1);

		ID3D11UnorderedAccessView* pViewnullptr = nullptr;
		context->CSSetUnorderedAccessViews(0, 1, &pViewnullptr, nullptr);
	}

	ID3D11UnorderedAccessView* pViewnullptr = nullptr;
	context->CSSetUnorderedAccessViews(0, 1, &pViewnullptr, nullptr);
}

Vector3D ParticleSystem::GetOrigin() const
{
	return gameObject->GetTransform()->GetPosition();
}

void ParticleSystem::InitParticles(int count)
{
	for (int i = 0; i < count; ++i)
	{
		InitParticle(i);
	}
}
