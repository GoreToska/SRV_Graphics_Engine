#include "ShadowMapCalculator.h"

float ShadowMapCalculator::GetFOV() 
{
	return (90 / 360.0f) * DirectX::XM_2PI;
}

DirectX::XMMATRIX ShadowMapCalculator::GetProjectionMatrix() 
{
	return DirectX::XMMatrixOrthographicLH(40, 40, 0.1f, 1000.0f);
}

DirectX::XMMATRIX ShadowMapCalculator::GetViewMatrixDirectional(GameObject* gameObject)
{
	return DirectX::XMMatrixLookAtLH(
		gameObject->GetTransform()->GetPosition().ToXMVector(),
		DirectX::XMVectorAdd(gameObject->GetTransform()->GetPosition().ToXMVector(), gameObject->GetTransform()->GetForwardVector().ToXMVector()),
		DirectX::XMVECTOR{ 0, 1, 0 });  // +X
}
