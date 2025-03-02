#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	this->position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->positionVector = XMLoadFloat3(&this->position);
	this->rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotationVector = XMLoadFloat3(&this->rotation);
	this->UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	return this->positionVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	return this->position;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return this->rotationVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	return this->rotation;
}

const Vector3D& Camera::GetForwardVector() const
{
	return Vector3D(forwardVector.m128_f32[0], forwardVector.m128_f32[1], forwardVector.m128_f32[2]);
}

const Vector3D& Camera::GetRightVector() const
{
	return Vector3D(rightVector.m128_f32[0], rightVector.m128_f32[1], rightVector.m128_f32[2]);
}

const Vector3D& Camera::GetUpVector() const
{
	return Vector3D(upVector.m128_f32[0], upVector.m128_f32[1], upVector.m128_f32[2]);
}

void Camera::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->position, pos);
	this->positionVector = pos;
	this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->position = XMFLOAT3(x, y, z);
	this->positionVector = XMLoadFloat3(&this->position);
	this->UpdateViewMatrix();
}

void Camera::AddPosition(const XMVECTOR& pos)
{
	this->positionVector += pos;
	XMStoreFloat3(&this->position, this->positionVector);
	this->UpdateViewMatrix();
}

void Camera::AddPosition(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	this->positionVector = XMLoadFloat3(&this->position);
	this->UpdateViewMatrix();
}

void Camera::AddPosition(const Vector3D& pos)
{
	AddPosition(pos.x, pos.y, pos.z);
}

void Camera::SetRotation(const XMVECTOR& rot)
{
	this->rotationVector = rot;
	XMStoreFloat3(&this->rotation, rot);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rotation = XMFLOAT3(x, y, z);
	this->rotationVector = XMLoadFloat3(&this->rotation);
	this->UpdateViewMatrix();
}

void Camera::AddRotation(const XMVECTOR& rot)
{
	this->rotationVector += rot;
	XMStoreFloat3(&this->rotation, this->rotationVector);
	this->UpdateViewMatrix();
}

void Camera::AddRotation(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
	this->rotationVector = XMLoadFloat3(&this->rotation);
	this->UpdateViewMatrix();
}

void Camera::AddRotation(const Vector3D& rot)
{
	AddRotation(rot.x, rot.y, rot.z);
}

void Camera::SetLookAtPosition(XMFLOAT3 lookAtPosition)
{
	if (lookAtPosition.x == this->position.x &&
		lookAtPosition.y == this->position.y &&
		lookAtPosition.z == this->position.z)
		return;

	lookAtPosition.x = this->position.x - lookAtPosition.x;
	lookAtPosition.y = this->position.y - lookAtPosition.y;
	lookAtPosition.z = this->position.z - lookAtPosition.z;

	float pitch = 0.0f;

	if (lookAtPosition.y != 0.0f)
	{
		const float distance = sqrt(lookAtPosition.x * lookAtPosition.x + lookAtPosition.z * lookAtPosition.z);
		pitch = atan(lookAtPosition.y / distance);
	}

	float yaw = 0.0f;

	if (lookAtPosition.x != 0)
	{
		yaw = atan(lookAtPosition.x / lookAtPosition.z);
	}

	if (lookAtPosition.z > 0)
	{
		yaw += XM_PI;
	}

	SetRotation(pitch, yaw, 0);
}

void Camera::SetLookAtPosition(Vector3D lookAtPosition)
{
	SetLookAtPosition(XMFLOAT3(lookAtPosition.x, lookAtPosition.y, lookAtPosition.z));
}

//Updates view matrix and also updates the movement vectors
void Camera::UpdateViewMatrix()
{
	//Calculate camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	//Adjust cam target to be offset by the camera's current position
	camTarget += this->positionVector;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	//Rebuild view matrix
	this->viewMatrix = XMMatrixLookAtLH(this->positionVector, camTarget, upDir);

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(0, rotation.y, 0);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, rotationMatrix);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, rotationMatrix);
	upVector = XMVector3TransformCoord(DEFAULT_UP_VECTOR, rotationMatrix);

}