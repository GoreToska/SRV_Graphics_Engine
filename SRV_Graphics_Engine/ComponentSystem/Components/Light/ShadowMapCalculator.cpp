#include "ShadowMapCalculator.h"

float ShadowMapCalculator::GetFOV() 
{
	return 90;
}

DirectX::XMMATRIX ShadowMapCalculator::GetProjectionMatrix() 
{

	float fov = (90 / 360.0f) * DirectX::XM_2PI;
	return DirectX::XMMatrixPerspectiveFovLH(fov, 1, 0.1, 1000);
}

DirectX::XMMATRIX ShadowMapCalculator::GetViewMatrixDirectional(GameObject* gameObject)
{
	auto eye = gameObject->GetTransform()->GetPosition().ToXMVector();
	auto forward = gameObject->GetTransform()->GetForwardVector().ToXMVector();
	auto lookat = DirectX::XMVectorAdd(eye, forward);
	auto up = gameObject->GetTransform()->GetUpVector().ToXMVector();
	return DirectX::XMMatrixLookAtLH(eye, forward, {0,1,0});
}
