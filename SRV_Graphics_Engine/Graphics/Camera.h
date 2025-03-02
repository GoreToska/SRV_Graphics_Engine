#pragma once

#include <DirectXMath.h>
#include "../DataTypes/Vector3D.h"

class Camera
{
public:
	Camera();

	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const DirectX::XMMATRIX& GetViewMatrix() const;
	const DirectX::XMMATRIX& GetProjectionMatrix() const;

	const DirectX::XMVECTOR& GetPositionVector() const;
	const DirectX::XMFLOAT3& GetPositionFloat3() const;
	const DirectX::XMVECTOR& GetRotationVector() const;
	const DirectX::XMFLOAT3& GetRotationFloat3() const;

	const Vector3D& GetForwardVector() const;
	const Vector3D& GetRightVector() const;
	const Vector3D& GetUpVector() const;


	void SetPosition(const DirectX::XMVECTOR& pos);
	void SetPosition(float x, float y, float z);
	void AddPosition(const DirectX::XMVECTOR& pos);
	void AddPosition(float x, float y, float z);
	void AddPosition(const Vector3D& pos);
	void SetRotation(const DirectX::XMVECTOR& rot);
	void SetRotation(float x, float y, float z);
	void AddRotation(const DirectX::XMVECTOR& rot);
	void AddRotation(float x, float y, float z);
	void AddRotation(const Vector3D& rot);
	void SetLookAtPosition(DirectX::XMFLOAT3 lookAtPosition);
	void SetLookAtPosition(Vector3D lookAtPosition);

private:
	void UpdateViewMatrix();

	DirectX::XMVECTOR positionVector;
	DirectX::XMVECTOR rotationVector;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;

	const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR forwardVector;
	DirectX::XMVECTOR rightVector;
	DirectX::XMVECTOR upVector;
};