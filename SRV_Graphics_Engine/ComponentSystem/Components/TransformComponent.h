#pragma once

#include "../../DataTypes/Vector3D.h"
#include "IComponent.h"
#include <DirectXMath.h>

class TransformComponent : public IComponent
{
public:
	TransformComponent();
	TransformComponent(const Vector3D& position, const Vector3D& rotation, const Vector3D& scale);

	void Update(const float& deltaTime) override;

	void SetPosition(const Vector3D& position);
	void MovePosition(const Vector3D& vector);

	//void SetRotation(const Vector3D& rotation, const float& angle);
	void AddRotation(const Vector3D& rotation, const float& angle = 0);

	void SetScale(const Vector3D& scale);

	Vector3D GetPosition() const;
	Vector3D GetRotation() const;
	Vector3D GetScale() const;
	Vector3D GetCenter() const;
	DirectX::XMVECTOR GetOrientation() const;

private:
	Vector3D position{};
	Vector3D rotation{};
	Vector3D scale{};
	Vector3D center{};
	DirectX::XMVECTOR orientation{};
};

