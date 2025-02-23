#pragma once

#pragma region Includes
#include<DirectXCollision.h>
#include<DirectXMath.h>

#include "IComponent.h"
#include"../../DataTypes/Vector3D.h"

class GameObject;
class TransformComponent;
#pragma endregion


class CollisionComponent : public IComponent
{
public:
	CollisionComponent(GameObject* gameObject, Vector3D firstPoint, Vector3D secondPoint);

	void Update() override;
	void OnCollide(CollisionComponent* collider);

	const DirectX::BoundingBox& GetBoundingVolume();
	GameObject* GetGameObject() const;

private:
	DirectX::BoundingBox boxCollider;
	TransformComponent* transform;
	GameObject* gameObject;
};

