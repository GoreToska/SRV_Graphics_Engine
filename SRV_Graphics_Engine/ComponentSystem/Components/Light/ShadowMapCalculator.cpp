#include "ShadowMapCalculator.h"

float ShadowMapCalculator::GetFOV() 
{
	return (90 / 360.0f) * DirectX::XM_2PI;
}

DirectX::XMMATRIX ShadowMapCalculator::GetProjectionMatrix() 
{
	return DirectX::XMMatrixPerspectiveFovLH(GetFOV(), 1, 0.1f, 1000.0f);
}

DirectX::XMMATRIX* ShadowMapCalculator::GetPerspectiveMatricesCube(GameObject* gameObject)
{
	DirectX::XMMATRIX matrices[6]{};

	matrices[0] = DirectX::XMMatrixLookAtLH(
		gameObject->GetTransform()->GetPosition().ToXMVector(),
		DirectX::XMVectorAdd(gameObject->GetTransform()->GetPosition().ToXMVector(), DirectX::XMVECTOR{ 1,0,0 }),
		DirectX::XMVECTOR{ 0, 1, 0 });  // +X

	matrices[1] = DirectX::XMMatrixLookAtLH(
		gameObject->GetTransform()->GetPosition().ToXMVector(),
		DirectX::XMVectorAdd(gameObject->GetTransform()->GetPosition().ToXMVector(), DirectX::XMVECTOR{ -1, 0, 0 }),
		DirectX::XMVECTOR{ 0, 1, 0 }); // -X

	matrices[2] = DirectX::XMMatrixLookAtLH(
		gameObject->GetTransform()->GetPosition().ToXMVector(),
		DirectX::XMVectorAdd(gameObject->GetTransform()->GetPosition().ToXMVector(), DirectX::XMVECTOR{ 0, 1, 0 }),
		DirectX::XMVECTOR{ 0, 0, -1 }); // +Y

	matrices[3] = DirectX::XMMatrixLookAtLH(
		gameObject->GetTransform()->GetPosition().ToXMVector(),
		DirectX::XMVectorAdd(gameObject->GetTransform()->GetPosition().ToXMVector(), DirectX::XMVECTOR{ 0, -1, 0 }),
		DirectX::XMVECTOR{ 0, 0, 1 });  // -Y

	matrices[4] = DirectX::XMMatrixLookAtLH(
		gameObject->GetTransform()->GetPosition().ToXMVector(),
		DirectX::XMVectorAdd(gameObject->GetTransform()->GetPosition().ToXMVector(), DirectX::XMVECTOR{ 0, 0, 1 }),
		DirectX::XMVECTOR{ 0, 1, 0 });  // +Z

	matrices[5] = DirectX::XMMatrixLookAtLH(
		gameObject->GetTransform()->GetPosition().ToXMVector(),
		DirectX::XMVectorAdd(gameObject->GetTransform()->GetPosition().ToXMVector(), DirectX::XMVECTOR{ 0, 0, -1 }),
		DirectX::XMVECTOR{ 0, 1, 0 }); // -Z

	return matrices;
}
