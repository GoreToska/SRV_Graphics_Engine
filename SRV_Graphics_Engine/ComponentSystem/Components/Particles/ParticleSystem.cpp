#include "ParticleSystem.h"
#include "../../../Graphics/ShaderManager/ShaderManager.h"


ParticleSystem::ParticleSystem(GameObject* owner, size_t maxParticles) : gameObject(owner), maxParticles(maxParticles)
{
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
}

void ParticleSystem::Emit(int count)
{
}

void ParticleSystem::Initialize()
{
	bitonicSortShader = ShaderManager::GetInstance().GetCS(ShaderManager::CS_BitonicSort);
	bitonicTransposeShader = ShaderManager::GetInstance().GetCS(ShaderManager::CS_BitonicTranspose);
	constSortBuf.Initialize();
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

		if(MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE > 0 && MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE > 0)
			context->Dispatch(MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE, MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE, 1);

		// Sort the transposed column data
		context->CSSetShader(bitonicSortShader->GetShader(), nullptr, 0);
		if(maxParticles / BITONIC_BLOCK_SIZE > 0)
			context->Dispatch(maxParticles / BITONIC_BLOCK_SIZE, 1, 1);

		SetConstBuffer(BITONIC_BLOCK_SIZE, level, MATRIX_HEIGHT, MATRIX_WIDTH);

		// Transpose the data from buffer 2 back into buffer 1
		context->CSSetUnorderedAccessViews(0, 1, sortListBufferUAV.GetAddressOf(), nullptr);
		context->CSSetShader(bitonicTransposeShader->GetShader(), nullptr, 0);
		if(MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE > 0 && MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE > 0)
			context->Dispatch(MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE, MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE, 1);

		// Sort the row data
		context->CSSetShader(bitonicSortShader->GetShader(), nullptr, 0);
		if(maxParticles / BITONIC_BLOCK_SIZE > 0)
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
