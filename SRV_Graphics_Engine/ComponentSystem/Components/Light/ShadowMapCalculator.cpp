#include "ShadowMapCalculator.h"
#include "../../../Engine/Engine.h"

float ShadowMapCalculator::GetFOV()
{
	return 90;
}

DirectX::XMMATRIX ShadowMapCalculator::GetProjectionMatrix()
{
	//float fov = (90 / 360.0f) * DirectX::XM_2PI;
	return DirectX::XMMatrixOrthographicLH(100, 100, 0.1, 1000);
	//return DirectX::XMMatrixPerspectiveFovLH(fov, 1, 0.1, 1000);
}

DirectX::XMMATRIX ShadowMapCalculator::GetViewMatrixDirectional(GameObject* gameObject)
{
	return gameObject->GetTransform()->GetViewMatrix();
}

void ShadowMapCalculator::GetDirectionalLightMatrices(GameObject* gameObject, std::vector<Matrix>& projections, Matrix& view)
{
	projections.clear();
	//projections.reserve(CascadeCount);

	auto cameraFrustumCorners = SRVEngine.GetGraphics().GetCamera()->GetFrustumCornersWorldSpace();

	Vector3D center = Vector3D::Zero;
	for (const auto& v : cameraFrustumCorners)
	{
		center += Vector3D(v);
	}
	center /= cameraFrustumCorners.size();

	view = Matrix::CreateLookAt(center, center + gameObject->GetTransform()->GetForwardVector(), Vector3D::Up);

	std::vector<Vector4D> slideVectors =
	{
		cameraFrustumCorners[4] - cameraFrustumCorners[0],
		cameraFrustumCorners[5] - cameraFrustumCorners[1],
		cameraFrustumCorners[6] - cameraFrustumCorners[2],
		cameraFrustumCorners[7] - cameraFrustumCorners[3]
	};

	for (int cascade = 1; cascade <= CascadeCount; ++cascade) 
	{
		std::vector<Vector4D> cascadeCorners;
		
		for (int i = 0; i < 4; i++)	
			cascadeCorners.push_back(cameraFrustumCorners[i] + (cascade - 1) * slideVectors[i] / CascadeCount);
		
		for (int i = 0; i < 4; i++)	
			cascadeCorners.push_back(cameraFrustumCorners[i] + cascade * slideVectors[i] / CascadeCount);

		float minX = (std::numeric_limits<float>::max)();
		float maxX = std::numeric_limits<float>::lowest();
		float minY = (std::numeric_limits<float>::max)();
		float maxY = std::numeric_limits<float>::lowest();
		float minZ = (std::numeric_limits<float>::max)();
		float maxZ = std::numeric_limits<float>::lowest();

		for (const auto& v : cascadeCorners) 
		{
			const auto trf = Vector4D::Transform(v, view);
			minX = (std::min)(minX, trf.x);
			maxX = (std::max)(maxX, trf.x);
			minY = (std::min)(minY, trf.y);
			maxY = (std::max)(maxY, trf.y);
			minZ = (std::min)(minZ, trf.z);
			maxZ = (std::max)(maxZ, trf.z);
		}

		constexpr float zMult = 15.0f;
		minZ = (minZ < 0) ? minZ * zMult : minZ / zMult;
		maxZ = (maxZ < 0) ? maxZ / zMult : maxZ * zMult;
		auto lightProjection = Matrix::CreateOrthographicOffCenter(minX, maxX, minY, maxY, minZ, maxZ);
		projections.push_back(lightProjection);
	}
}
