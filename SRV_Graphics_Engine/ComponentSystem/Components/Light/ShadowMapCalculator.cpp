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

Matrix ShadowMapCalculator::GetDirectionalLightViewProj(Vector3D direction, float nearZ, float farZ)
{
	Matrix cameraViewProjMatrix = Matrix::CreateLookAt(SRVEngine.GetGraphics().GetCamera()->GetPositionFloat3(), 
		SRVEngine.GetGraphics().GetCamera()->GetPositionFloat3() + SRVEngine.GetGraphics().GetCamera()->GetForwardVector(), 
		SRVEngine.GetGraphics().GetCamera()->GetUpVector()) *
		Matrix::CreatePerspectiveFieldOfView(
			SRVEngine.GetGraphics().GetCamera()->GetFOV(),
			SRVEngine.GetGraphics().GetCamera()->GetAspectRatio(),
			nearZ, farZ);
	auto cameraFrustumCorners = SRVEngine.GetGraphics().GetCamera()->GetFrustumCornersWorldSpace(cameraViewProjMatrix);

	Vector4D center = Vector4D::Zero;
	for (const auto& v : cameraFrustumCorners)
	{
		center += v;
	}
	center /= static_cast<float>(cameraFrustumCorners.size());

	//const auto lightView = Matrix::CreateLookAt(center, center + direction, Vector4D::Up);
	auto newView = DirectX::XMMatrixLookAtLH(center, center + Vector4D(direction.x, direction.y, direction.z, 1.0f), Vector3D::Up);
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float maxY = std::numeric_limits<float>::lowest();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const auto& v : cameraFrustumCorners)
	{
		const auto trf = Vector4D::Transform(v, newView);
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	constexpr float zMult = 10.0f;
	minZ = (minZ < 0) ? minZ * zMult : minZ / zMult;
	maxZ = (maxZ < 0) ? maxZ / zMult : maxZ * zMult;

	//const auto lightProjection = Matrix::CreateOrthographicOffCenter(minX, maxX, minY, maxY, minZ, maxZ);
	const auto lightProjection = DirectX::XMMatrixOrthographicOffCenterLH(minX, maxX, minY, maxY, minZ, maxZ);
	return newView * lightProjection;
}

// frustrum corners world space should be calculated for each cascade
// view for each cascade
// calculate for each cascade with different nearZ and farZ

