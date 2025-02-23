#pragma once

#include<DirectXCollision.h>
#include<DirectXMath.h>

#include "IComponent.h"
#include"../../DataTypes/Vector3D.h"

class TransformComponent;

class CollisionComponent : public IComponent
{
public:
	CollisionComponent();
	CollisionComponent(TransformComponent* transform, Vector3D firstPoint, Vector3D secondPoint);

	void Update() override;
	ComponentType GetType() override;
	const DirectX::BoundingBox& GetBoundingVolume();

	int id = 0;

private:
	DirectX::BoundingBox boxCollider;
	TransformComponent* transform;
};

