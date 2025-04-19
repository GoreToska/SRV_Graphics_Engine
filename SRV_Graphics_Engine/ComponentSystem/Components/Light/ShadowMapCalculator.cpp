#include "ShadowMapCalculator.h"

float ShadowMapCalculator::GetFOV() 
{
	return 90;
}

DirectX::XMMATRIX ShadowMapCalculator::GetProjectionMatrix() 
{
	//float fov = (90 / 360.0f) * DirectX::XM_2PI;
	return DirectX::XMMatrixOrthographicLH(100,100,0.1, 1000);
	//return DirectX::XMMatrixPerspectiveFovLH(fov, 1, 0.1, 1000);
}

DirectX::XMMATRIX ShadowMapCalculator::GetViewMatrixDirectional(GameObject* gameObject)
{
	return gameObject->GetTransform()->GetViewMatrix();
}
