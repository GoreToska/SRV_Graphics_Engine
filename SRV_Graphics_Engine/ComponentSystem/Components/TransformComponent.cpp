#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	position = Vector3D(0, 0, 0);
	rotation = Vector3D(0, 0, 0);
	scale = Vector3D(1, 1, 1);
}

TransformComponent::TransformComponent(const Vector3D& position, const Vector3D& rotation, const Vector3D& scale)
	: position(position), rotation(rotation), scale(scale)
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

void TransformComponent::SetRotation(const Vector3D& rotation)
{
	this->rotation = rotation;
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
