#include "RotationComponent.h"



RotationComponent::RotationComponent(TransformComponent* ownerTransform, const Vector3D& axis, const float& speed) :
	ownerTransform(ownerTransform), axis(axis), speed(speed)
{
}

void RotationComponent::Update(const float& deltaTime)
{
	ownerTransform->AddRotation(axis, speed * deltaTime);
}


