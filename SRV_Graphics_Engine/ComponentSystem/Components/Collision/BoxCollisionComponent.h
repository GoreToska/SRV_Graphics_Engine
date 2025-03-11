#pragma once
#include "../../../DataTypes/Vector3D.h"
#include "CollisionComponent.h"
#include "../../../DataTypes/Collision/BoundingBox.h"


class BoxCollisionComponent : public CollisionComponent
{
public:
	BoxCollisionComponent(GameObject* gameObject, Vector3D firstPoint, Vector3D secondPoint);

	virtual void Update(const float& deltaTime) override;

private:
	TransformComponent* transform;
	GameObject* gameObject;
	std::unique_ptr<BoundingBox> boundingVolume;
};

