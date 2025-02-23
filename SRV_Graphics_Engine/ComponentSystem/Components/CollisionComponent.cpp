#include "CollisionComponent.h"
#include "../../Engine/Engine.h"
#include "TransformComponent.h"

#include <iostream>


CollisionComponent::CollisionComponent()
{
	DirectX::BoundingBox::CreateFromPoints(boxCollider, { -0.005,-0.005,-0.005 }, { 0.005,0.005,0.005 });
	boxCollider.Center = { 0,0,0 };
}

CollisionComponent::CollisionComponent(TransformComponent* transform, Vector3D firstPoint, Vector3D secondPoint)
{
	this->transform = transform;

	DirectX::BoundingBox::CreateFromPoints(boxCollider,
		{ firstPoint.x,firstPoint.y,firstPoint.z },
		{ secondPoint.x,secondPoint.y,secondPoint.z });

	boxCollider.Center = { transform->GetCenter().x,transform->GetCenter().y,transform->GetCenter().z };
}

void CollisionComponent::Update()
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
			// todo do something!

			std::cout << "COLLIDE! " << id << "\n";
		}
	}
}

IComponent::ComponentType CollisionComponent::GetType()
{
	return CollisionComponentType;
}

const DirectX::BoundingBox& CollisionComponent::GetBoundingVolume()
{
	return boxCollider;
}
