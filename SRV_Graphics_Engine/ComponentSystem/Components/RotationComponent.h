#pragma once

#include "TransformComponent.h"

class RotationComponent : public IComponent
{
public:
	RotationComponent(TransformComponent* ownerTransform, const Vector3D& axis, const float& speed);

	void Update(const float& deltaTime) override;

private:
	TransformComponent* ownerTransform;
	Vector3D axis{};
	float speed = 0;
};

