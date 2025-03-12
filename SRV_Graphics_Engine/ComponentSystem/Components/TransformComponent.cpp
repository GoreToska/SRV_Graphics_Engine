#include "TransformComponent.h"
#include<vector>
#include "../GameObject.h"


TransformComponent::TransformComponent(GameObject* owner)
	:position(Vector3D(0, 0, 0)), rotation(Vector3D(0, 0, 0)), scale(Vector3D(1, 1, 1)),
	orientation(DirectX::XMQuaternionIdentity()), gameObject(owner)
{
}

TransformComponent::TransformComponent(GameObject* owner, const Vector3D& position, const Vector3D& rotation, const Vector3D& scale)
	: position(position), rotation(rotation), scale(scale), orientation(DirectX::XMQuaternionIdentity()), gameObject(owner)
{
}

void TransformComponent::Update(const float& deltaTime)
{
	// TODO: what should i do there?)
}

void TransformComponent::SetPosition(const Vector3D& position)
{
	this->position = position;

	std::vector<GameObject*> children = gameObject->GetChildren();

	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->GetTransform()->SetPosition(
			children[i]->GetTransform()->GetPosition() + position);
	}
}

void TransformComponent::SetPosition(const float& x, const float& y, const float& z)
{
	SetPosition({ x,y,z });
}

void TransformComponent::AddLocalRotation(const Vector3D& rotationAxis, const float& angle)
{
	if (rotationAxis == Vector3D::ZeroVector())
		return;

	// Нормализуем ось вращения
	DirectX::XMVECTOR axis = DirectX::XMVectorSet(rotationAxis.x, rotationAxis.y, rotationAxis.z, 0.0f);
	axis = DirectX::XMVector3Normalize(axis);

	// Преобразуем угол в радианы и создаем кватернион для нового вращения
	float radians = DirectX::XMConvertToRadians(angle);
	DirectX::XMVECTOR newRotation = DirectX::XMQuaternionRotationAxis(axis, radians);

	// Обновляем ориентацию родительского объекта
	orientation = DirectX::XMQuaternionMultiply(newRotation, orientation);

	// Получаем дочерние объекты
	std::vector<GameObject*> children = gameObject->GetChildren();
	for (GameObject* child : children)
	{
		//TODO: add local rotation
	}
}

void TransformComponent::AddWorldRotation(const Vector3D& rotationAxis, const float& angle)
{
	if (rotationAxis == Vector3D::ZeroVector())
		return;

	DirectX::XMVECTOR axis = DirectX::XMVectorSet(rotationAxis.x, rotationAxis.y, rotationAxis.z, 0.0f);
	axis = DirectX::XMVector3Normalize(axis);

	float radians = DirectX::XMConvertToRadians(angle);
	DirectX::XMVECTOR newRotation = DirectX::XMQuaternionRotationAxis(axis, radians);
	orientation = DirectX::XMQuaternionMultiply(orientation, newRotation);

	std::vector<GameObject*> children = gameObject->GetChildren();
	for (GameObject* child : children)
	{
		Vector3D childPosition = child->GetTransform()->GetPosition();

		Vector3D relativePosition = childPosition - this->position;

		DirectX::XMVECTOR relativePositionVec = DirectX::XMVectorSet(relativePosition.x, relativePosition.y, relativePosition.z, 0.0f);
		DirectX::XMVECTOR rotatedPositionVec = DirectX::XMVector3Rotate(relativePositionVec, newRotation);

		Vector3D newChildPosition = this->position + Vector3D(rotatedPositionVec.m128_f32[0], rotatedPositionVec.m128_f32[1], rotatedPositionVec.m128_f32[2]);
		child->GetTransform()->SetPosition(newChildPosition);

		DirectX::XMVECTOR childOrientation = child->GetTransform()->GetOrientation();
		child->GetTransform()->orientation = DirectX::XMQuaternionMultiply(childOrientation, newRotation);
	}
}

void TransformComponent::SetScale(const Vector3D& scale)
{
	this->scale = scale;
}

void TransformComponent::MovePosition(const Vector3D& vector)
{
	this->position += vector;

	std::vector<GameObject*> children = gameObject->GetChildren();

	for (GameObject* child : children)
	{
		child->GetTransform()->SetPosition(child->GetTransform()->GetPosition() + vector);
	}
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
