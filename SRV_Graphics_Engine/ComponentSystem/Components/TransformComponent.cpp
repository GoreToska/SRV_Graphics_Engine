#include "TransformComponent.h"
#include<vector>
#include "../GameObject.h"
#include <SimpleMath.h>



TransformComponent::TransformComponent(GameObject* owner)
	:position(Vector3D(0, 0, 0)), rotation(Quaternion::Identity), scale(Vector3D(1, 1, 1)), gameObject(owner)
{
}

TransformComponent::TransformComponent(GameObject* owner, const Vector3D& position, const Vector3D& rotation, const Vector3D& scale)
	: position(position), rotation(rotation), scale(scale), gameObject(owner)
{
}

void TransformComponent::Update(const float& deltaTime)
{
	UpdateWorldMatrix();
}

void TransformComponent::SetPosition(const Vector3D& position)
{
	this->position = position;
}

void TransformComponent::SetPosition(const float& x, const float& y, const float& z)
{
	SetPosition({ x,y,z });
}

void TransformComponent::SetLookAtRotation(const Vector3D& lookAtPosition)
{
	Matrix newRotation = Matrix::CreateLookAt(position, lookAtPosition, GetUpVector());
	rotation = Quaternion::CreateFromRotationMatrix(newRotation);
}

void TransformComponent::SetScale(const Vector3D& scale)
{
	this->scale = scale;
}

void TransformComponent::MovePosition(const Vector3D& vector)
{
	this->position += vector;
}

void TransformComponent::AddLocalRotation(const Quaternion& rotation)
{
	this->rotation *= rotation;
	//this->rotation.Normalize();
}

void TransformComponent::AddLocalRotation(const Vector3D& rotation)
{
	this->rotation *= Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
}

void TransformComponent::AddLocalRotation(Vector3D& axis, float angle)
{
	if (axis == Vector3D::Zero)
		return;

	axis.Normalize();
	this->rotation *= Quaternion::CreateFromAxisAngle(axis, angle);
}

void TransformComponent::SetRotation(const Quaternion& rotation)
{
	this->rotation = rotation;
}

void TransformComponent::SetRotation(const Vector3D& rotation)
{
	this->rotation = Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
}

Vector3D TransformComponent::GetPosition() const
{
	return position;
}

Quaternion TransformComponent::GetRotation() const
{
	return rotation;
}

Vector3D TransformComponent::GetScale() const
{
	return scale;
}

Vector3D TransformComponent::GetForwardVector()
{
	Vector3D Forward = worldMatrix.Forward();
	Forward.Normalize();
	return Forward;
}

Vector3D TransformComponent::GetRightVector()
{
	Vector3D Right = worldMatrix.Right();
	Right.Normalize();
	return Right;
}

Vector3D TransformComponent::GetUpVector()
{
	Vector3D Up = worldMatrix.Up();
	Up.Normalize();
	return Up;
}

Matrix TransformComponent::GetWorldMatrix()
{
	return worldMatrix;
}

Matrix TransformComponent::GetViewMatrix()
{
	return Matrix::CreateLookAt(position, position + GetForwardVector(), Vector3D::Up);
}

void TransformComponent::UpdateWorldMatrix()
{
	Matrix ParentMatrix = gameObject->GetParent() == nullptr ? Matrix::Identity : gameObject->GetParent()->GetTransform()->worldMatrix;

	worldMatrix = Matrix::CreateScale(scale)
		* Matrix::CreateFromQuaternion(rotation)
		* Matrix::CreateTranslation(position)
		* ParentMatrix;

}
