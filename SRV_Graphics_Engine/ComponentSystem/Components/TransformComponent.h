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

	void SetScale(const Vector3D& scale);

	Vector3D GetPosition() const;
	Vector3D GetRotation() const;
	Vector3D GetScale() const;
	Vector3D GetCenter() const;
	DirectX::XMVECTOR GetOrientation() const;

private:
	GameObject* gameObject;

	Vector3D position{};
	Vector3D rotation{};
	Vector3D scale{};
	Vector3D center{};
	DirectX::XMVECTOR orientation{};
};

