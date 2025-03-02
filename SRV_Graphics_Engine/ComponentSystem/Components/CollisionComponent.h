#pragma once

#pragma region Includes
#include<DirectXCollision.h>
#include<DirectXMath.h>

#include "IComponent.h"
#include "../../DataTypes/Vector3D.h"
#include "../../Delegates/Delegates.h"

class GameObject;
class TransformComponent;
#pragma endregion


class CollisionComponent : public IComponent
{
public:
	CollisionComponent(GameObject* gameObject, Vector3D firstPoint, Vector3D secondPoint);

	CollisionComponent(GameObject* gameObject, Vector3D firstPoint, Vector3D secondPoint, CollisionChannel collisionChannel = CollisionChannel::Channel_1);

	void Update(const float& deltaTime) override;
	//void OnCollide(CollisionComponent* collider);

	const DirectX::BoundingBox& GetBoundingVolume();
	GameObject* GetGameObject() const;

	MulticastDelegate<CollisionComponent*,GameObject*> OnCollisionEnter;

private:
	DirectX::BoundingBox boxCollider;
	TransformComponent* transform;
	GameObject* gameObject;
};

