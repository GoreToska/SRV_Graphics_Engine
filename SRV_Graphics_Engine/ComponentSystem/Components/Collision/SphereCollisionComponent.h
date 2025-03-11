#pragma once

#include "../../../DataTypes/Vector3D.h"
#include "CollisionComponent.h"
#include "../../../DataTypes/Collision/BoundingSphere.h"

class SphereCollisionComponent : public CollisionComponent
{
public:
	SphereCollisionComponent(GameObject* gameObject, Vector3D center, float radius);

	virtual void Update(const float& deltaTime) override;

private:
	TransformComponent* transform;
	GameObject* gameObject;
	std::unique_ptr<BoundingSphere> boundingVolume;
};

