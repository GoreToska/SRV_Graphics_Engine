#include "ShadowMapCalculator.h"

float ShadowMapCalculator::GetFOV() 
{
	return 90;
}

DirectX::XMMATRIX ShadowMapCalculator::GetProjectionMatrix() 
{
	float fov = (90 / 360.0f) * DirectX::XM_2PI;
	return DirectX::XMMatrixPerspectiveFovLH(fov, 1, 0.1, 1000);
	//return DirectX::XMMatrixOrthographicLH(1000, 1000, 0.1f, 1000.0f);
}

DirectX::XMMATRIX ShadowMapCalculator::GetViewMatrixDirectional(GameObject* gameObject)
{
	auto eye = gameObject->GetTransform()->GetPosition().ToXMVector();
	auto forward = gameObject->GetTransform()->GetForwardVector().ToXMVector();
	auto lookat = DirectX::XMVectorAdd(eye, forward);
	auto up = gameObject->GetTransform()->GetUpVector().ToXMVector();

	return DirectX::XMMatrixLookAtLH(
		eye,
		lookat,
		up
		/*DirectX::XMVECTOR{ 0, 1, 0 }*/);  // +X
}
