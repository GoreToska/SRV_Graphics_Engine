#include "Camera.h"
#include "../Engine/Engine.h"

using namespace DirectX;

Camera::Camera()
{
	position = XMFLOAT3(0.0f, -5.0f, 0.0f);
	positionVector = XMLoadFloat3(&this->position);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotationVector = XMLoadFloat3(&this->rotation);
	fov = (90 / 360.0f) * XM_2PI;
	aspectRatio = SRVEngine.GetGraphics().GetClientWidth() / SRVEngine.GetGraphics().GetClientHeight();
	orthoWidth = 80;
	orthoHeight = 80;
	nearZ = 0.1;
	farZ = 1000;

	UpdateViewMatrix();
}

void Camera::SetPerspectiveProjection()
{
	projectionMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);
}

void Camera::SetOrthographicProjection()
{
	projectionMatrix = XMMatrixOrthographicLH(orthoWidth, orthoHeight, nearZ, farZ);
}

void Camera::SetPerspectiveProjection(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	fov = fovDegrees / 360.0f * XM_2PI;
	this->aspectRatio = aspectRatio;
	this->nearZ = nearZ;
	this->farZ = farZ;

	SetPerspectiveProjection();
}

void Camera::SetOrthographicProjection(float viewWidth, float viewHeight, float nearZ, float farZ)
{
	this->orthoWidth = viewWidth;
	this->orthoHeight = viewHeight;
	this->nearZ = nearZ;
	this->farZ = farZ;

	SetOrthographicProjection();
}

const Matrix Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const Matrix Camera::GetProjectionMatrix() const
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

void Camera::SetPosition(const Vector3D& position)
{
	SetPosition(position.x, position.y, position.z);
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

float Camera::GetNearZ()
{
	return nearZ;
}

float Camera::GetFarZ()
{
	return farZ;
}

float Camera::GetFOV()
{
	return fov;
}

float Camera::GetAspectRatio()
{
	return aspectRatio;
}

std::vector<Vector4D> Camera::GetFrustumCornersWorldSpace(Matrix viewProj)
{
	const auto inv = viewProj.Invert();

	std::vector<Vector4D> frustumCorners;
	frustumCorners.reserve(8);
	
	for (int x = 0; x < 2; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			for (int z = 0; z < 2; ++z)
			{
				const Vector4D pt = Vector4D::Transform(Vector4D(
					2.0f * x - 1.0f,
					2.0f * y - 1.0f,
					z,
					1.0f
				), inv);
				frustumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustumCorners;
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
