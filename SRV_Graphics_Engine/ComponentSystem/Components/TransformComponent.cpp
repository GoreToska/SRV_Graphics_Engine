#include "TransformComponent.h"
#include<vector>
#include "../GameObject.h"
#include <SimpleMath.h>


// TODO:
// Remake this component to Simple Math library
// Must have methods:
	// SetPosition
	// SetRotation
	// SetLookAtRotation
	// GetForwardVector
	// GetRightVector
	// GetUpVector

TransformComponent::TransformComponent(GameObject* owner)
	:position(Vector3D(0, 0, 0)), rotation(Vector3D(0, 0, 0)), scale(Vector3D(1, 1, 1)),
	orientation(Vector3D::ZeroVector()), gameObject(owner)
{
}

TransformComponent::TransformComponent(GameObject* owner, const Vector3D& position, const Vector3D& rotation, const Vector3D& scale)
	: position(position), rotation(rotation), scale(scale), orientation(Vector3D::ZeroVector()), gameObject(owner)
{
}

void TransformComponent::Update(const float& deltaTime)
{
	// TODO: what should i do there?)
}

void TransformComponent::SetPosition(const Vector3D& position)
{
	Vector3D delta = position - this->position;
	this->position = position;

	std::vector<GameObject*> children = gameObject->GetChildren();
	for (int i = 0; i < children.size(); ++i)
	{
		Vector3D childPosition = children[i]->GetTransform()->GetPosition();
		children[i]->GetTransform()->SetPosition(childPosition + delta);
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

	DirectX::XMVECTOR axis = DirectX::XMVectorSet(rotationAxis.x, rotationAxis.y, rotationAxis.z, 0.0f);
	axis = DirectX::XMVector3Normalize(axis);

	float radians = DirectX::XMConvertToRadians(angle);
	DirectX::XMVECTOR newRotation = DirectX::XMQuaternionRotationAxis(axis, radians);

	rotation = newRotation;
	orientation = Vector3D(DirectX::XMQuaternionMultiply(newRotation, orientation.ToXMVector()));

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
	rotation = newRotation;
	orientation = Vector3D(DirectX::XMQuaternionMultiply(orientation.ToXMVector(), newRotation));

	std::vector<GameObject*> children = gameObject->GetChildren();
	for (GameObject* child : children)
	{
		Vector3D childPosition = child->GetTransform()->GetPosition();

		Vector3D relativePosition = childPosition - this->position;

		DirectX::XMVECTOR relativePositionVec = DirectX::XMVectorSet(relativePosition.x, relativePosition.y, relativePosition.z, 0.0f);
		DirectX::XMVECTOR rotatedPositionVec = DirectX::XMVector3Rotate(relativePositionVec, newRotation);

		Vector3D newChildPosition = this->position + Vector3D(rotatedPositionVec.m128_f32[0], rotatedPositionVec.m128_f32[1], rotatedPositionVec.m128_f32[2]);
		child->GetTransform()->SetPosition(newChildPosition);

		Vector3D childOrientation = child->GetTransform()->GetOrientation();
		child->GetTransform()->orientation = Vector3D(DirectX::XMQuaternionMultiply(childOrientation.ToXMVector(), newRotation));
	}
}

void TransformComponent::SetRotation(const Vector3D& rotation)
{
	DirectX::XMVECTOR rotationQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(
		DirectX::XMConvertToRadians(rotation.x), // Pitch
		DirectX::XMConvertToRadians(rotation.y), // Yaw
		DirectX::XMConvertToRadians(rotation.z)  // Roll
	);

	rotationQuaternion.m128_f32[3] = 0;

	orientation = rotationQuaternion;
}

void TransformComponent::SetLookAtRotation(Vector3D lookAtPosition)
{
	using namespace DirectX;

	XMVECTOR currentPos = XMVectorSet(
		GetPosition().x,
		GetPosition().y,
		GetPosition().z,
		0.0f
	);

	XMVECTOR targetPos = XMVectorSet(
		lookAtPosition.x,  
		lookAtPosition.y,
		lookAtPosition.z,
		0.0f
	);


	XMVECTOR up = GetUpVector().ToXMVector();//XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR forward = XMVector3Normalize(targetPos - currentPos);

	if (XMVector3Equal(forward, XMVectorZero()) /*||
		XMVector3Length(forward).m128_f32[0] < 0.00001f*/)
	{
		return;
	}

	XMVECTOR right = GetRightVector().ToXMVector();//XMVector3Normalize(XMVector3Cross(up, forward));
	//up = XMVector3Normalize(XMVector3Cross(forward, right));

	XMMATRIX rotationMatrix;
	rotationMatrix.r[0] = right;
	rotationMatrix.r[1] = up;
	rotationMatrix.r[2] = forward;
	rotationMatrix.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	XMVECTOR quaternion = XMQuaternionRotationMatrix(rotationMatrix);
	orientation = Vector3D(quaternion);
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

Vector3D TransformComponent::GetOrientation()
{
	return orientation;
}

Vector3D TransformComponent::GetForwardVector()
{
	if (orientation == Vector3D::ZeroVector())
		return worldForwardVector;

	auto forward = Vector3D(DirectX::XMVector3Rotate(worldForwardVector.ToXMVector(), orientation.ToXMVector()));
	return forward;
}

Vector3D TransformComponent::GetRightVector()
{
	if (orientation == Vector3D::ZeroVector())
		return worldRightVector;

	return Vector3D(DirectX::XMVector3Rotate(worldRightVector.ToXMVector(), orientation.ToXMVector())).Normalized();
}

Vector3D TransformComponent::GetUpVector()
{
	if (orientation == Vector3D::ZeroVector())
		return worldUpVector;

	return Vector3D(DirectX::XMVector3Rotate(worldUpVector.ToXMVector(), orientation.ToXMVector())).Normalized();
}
