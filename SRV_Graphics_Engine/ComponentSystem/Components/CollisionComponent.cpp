#pragma region Includes
#include "CollisionComponent.h"
#include "../GameObject.h"
#include "../../Engine/Engine.h"
#include "TransformComponent.h"

#include <iostream>  
#pragma endregion



CollisionComponent::CollisionComponent(GameObject* gameObject, Vector3D firstPoint, Vector3D secondPoint, CollisionChannel collisionChannel)
{
	this->transform = gameObject->GetTransform();
	this->gameObject = gameObject;

	DirectX::BoundingBox::CreateFromPoints(boxCollider,
		{ firstPoint.x,firstPoint.y,firstPoint.z },
		{ secondPoint.x,secondPoint.y,secondPoint.z });

	boxCollider.Center = { transform->GetCenter().x,transform->GetCenter().y,transform->GetCenter().z };

	this->collisionChannel = collisionChannel;
}

void CollisionComponent::Update(float deltaTime)
{
	boxCollider.Center =
	{
		transform->GetCenter().x,
		transform->GetCenter().y,
		transform->GetCenter().z
	};

	if (!IsEnabled) return;

	for (GameObject* gameObject : SRVEngine.GetAllGameObjects())
	{
		CollisionComponent* colliderComponent = gameObject->GetComponent<CollisionComponent>();

		if (!colliderComponent)
			continue;

		if (!colliderComponent->IsEnabled)
			continue;

		if (colliderComponent == this)
			continue;

		if (ignoreChannels.find(colliderComponent->collisionChannel) != ignoreChannels.end())
			continue;

		if (boxCollider.Intersects(colliderComponent->GetBoundingVolume()))
		{
			OnCollisionEnter.Broadcast(this, colliderComponent);
		}
	}
}

const DirectX::BoundingBox& CollisionComponent::GetBoundingVolume()
{
	return boxCollider;
}

GameObject* CollisionComponent::GetGameObject() const
{
	return gameObject;
}

void CollisionComponent::AddIgnoreChannel(CollisionChannel channel)
{
	ignoreChannels.insert(channel);
}
