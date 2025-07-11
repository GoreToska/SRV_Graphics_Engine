#pragma once

#include "../../DataTypes/Vector3D.h"
#include "IComponent.h"
#include <DirectXMath.h>

class GameObject;

class TransformComponent : public IComponent
{
public:
	TransformComponent(GameObject* owner);
	TransformComponent(GameObject* owner, const Vector3D& position, const Vector3D& rotation, const Vector3D& scale);

	void Update(const float& deltaTime) override;

	void SetPosition(const Vector3D& position);
	void SetPosition(const float& x, const float& y, const float& z);
	void MovePosition(const Vector3D& vector);

	void AddLocalRotation(const Quaternion& rotation);
	void AddLocalRotation(const Vector3D& rotation);
	void AddLocalRotation(Vector3D& axis, float angle);
	void SetRotation(const Quaternion& rotation);
	void SetRotation(const Vector3D& rotation);
	void SetLookAtRotation(const Vector3D& position);

	void SetScale(const Vector3D& scale);

	Vector3D GetPosition() const;
	Quaternion GetRotation() const;
	Vector3D GetScale() const;

	Vector3D GetForwardVector();
	Vector3D GetRightVector();
	Vector3D GetUpVector();

	Matrix GetWorldMatrix();
	Matrix GetViewMatrix();

private:
	void UpdateWorldMatrix();

	GameObject* gameObject;

	Vector3D position{};
	Quaternion rotation{};
	Vector3D scale{};

	Matrix worldMatrix{};
};

