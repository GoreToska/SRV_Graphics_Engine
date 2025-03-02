#pragma region Includes
#include "CollisionComponent.h"
#include "../GameObject.h"
#include "../../Engine/Engine.h"
#include "TransformComponent.h"

#include <iostream>  
#pragma endregion



CollisionComponent::CollisionComponent(GameObject* gameObject, Vector3D firstPoint, Vector3D secondPoint)
{
	this->transform = gameObject->GetTransform();
	this->gameObject = gameObject;

	DirectX::BoundingBox::CreateFromPoints(boxCollider,
		{ firstPoint.x,firstPoint.y,firstPoint.z },
		{ secondPoint.x,secondPoint.y,secondPoint.z });

	boxCollider.Center = { transform->GetCenter().x,transform->GetCenter().y,transform->GetCenter().z };
}

void CollisionComponent::Update(const float& deltaTime)
{
	// TODO: what should i do here? 
	// 1. check collision for all other objects here <- will implement this for now
	// 2. check collision in other class for all objects

	boxCollider.Center =
	{
		transform->GetCenter().x,
		transform->GetCenter().y,
		transform->GetCenter().z
	};

	for (GameObject* gameObject : SRVEngine.GetAllGameObjects())
	{
		CollisionComponent* colliderComponent = gameObject->GetComponent<CollisionComponent>();

		if (!colliderComponent) continue;

		if (colliderComponent == this) continue;

		if (boxCollider.Intersects(colliderComponent->GetBoundingVolume()))
		{
			OnCollisionEnter.Broadcast(this, this->gameObject);
			// todo do something!
		}
	}
}

//void CollisionComponent::OnCollide(CollisionComponent* collider)
//{
//}

const DirectX::BoundingBox& CollisionComponent::GetBoundingVolume()
{
	return boxCollider;
}

GameObject* CollisionComponent::GetGameObject() const
{
	return gameObject;
}
