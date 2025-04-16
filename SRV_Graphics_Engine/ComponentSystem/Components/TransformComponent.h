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

	void AddLocalRotation(const Vector3D& rotationAxis, const float& angle = 0);
	void AddWorldRotation(const Vector3D& rotationAxis, const float& angle = 0);
	void SetRotation(const Vector3D& rotation);
	void SetLookAtRotation(Vector3D position);

	void SetScale(const Vector3D& scale);

	Vector3D GetPosition() const;
	Vector3D GetRotation() const;
	Vector3D GetScale() const;
	Vector3D GetCenter() const;

	Vector3D GetOrientation();
	Vector3D GetForwardVector();
	Vector3D GetRightVector();
	Vector3D GetUpVector();


private:
	GameObject* gameObject;

	Vector3D position{};
	Vector3D rotation{};
	Vector3D scale{};
	Vector3D center{};
	Vector3D orientation{};

	Vector3D worldForwardVector = { 0.0f, 0.0f, 1.0f };
	Vector3D worldRightVector = { 1.0f, 0.0f, 0.0f};
	Vector3D worldUpVector = { 0.0f, 1.0f, 0.0f};
};

