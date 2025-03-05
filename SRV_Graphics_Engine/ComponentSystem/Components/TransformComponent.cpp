#include "TransformComponent.h"

TransformComponent::TransformComponent()
	:position(Vector3D(0, 0, 0)), rotation(Vector3D(0, 0, 0)), scale(Vector3D(1, 1, 1)),
	 orientation(DirectX::XMQuaternionIdentity())

{
}

TransformComponent::TransformComponent(const Vector3D& position, const Vector3D& rotation, const Vector3D& scale)
	: position(position), rotation(rotation), scale(scale), orientation(DirectX::XMQuaternionIdentity())
{
}

void TransformComponent::Update(const float& deltaTime)
{
	// TODO: what should i do there?)
}

void TransformComponent::SetPosition(const Vector3D& position)
{
	this->position = position;
}

//void TransformComponent::SetRotation(const Vector3D& rotationAxis, const float& angle)
//{
//	DirectX::XMVECTOR axis = DirectX::XMVectorSet(rotationAxis.x, rotationAxis.y, rotationAxis.z, 0.0f);
//	axis = DirectX::XMVector3Normalize(axis);
//
//	float radians = DirectX::XMConvertToRadians(angle);
//	DirectX::XMVECTOR newRotation = DirectX::XMQuaternionRotationAxis(axis, radians);
//
//	orientation = DirectX::XMQuaternionMultiply(newRotation, orientation);
//}

void TransformComponent::AddRotation(const Vector3D& rotationAxis, const float& angle)
{
	DirectX::XMVECTOR axis = DirectX::XMVectorSet(rotationAxis.x, rotationAxis.y, rotationAxis.z, 0.0f);
	axis = DirectX::XMVector3Normalize(axis);

	float radians = DirectX::XMConvertToRadians(angle);
	DirectX::XMVECTOR newRotation = DirectX::XMQuaternionRotationAxis(axis, radians);

	orientation = DirectX::XMQuaternionMultiply(newRotation, orientation);
}

void TransformComponent::SetScale(const Vector3D& scale)
{
	this->scale = scale;
}

void TransformComponent::MovePosition(const Vector3D& vector)
{
	this->position += vector;
}

Vector3D TransformComponent::GetPosition() const
{
	return position;
}

Vector3D TransformComponent::GetRotation() const
{
	return rotation;
}

Vector3D TransformComponent::GetScale() const
{
	return scale;
}

Vector3D TransformComponent::GetCenter() const
{
	return position; // XD mb this can be changed later?)
}

DirectX::XMVECTOR TransformComponent::GetOrientation() const
{
	return orientation;
}
