#pragma region Includes
#include "CollisionComponent.h"
#include "../../GameObject.h"
#include "../../../Engine/Engine.h"
#include "../TransformComponent.h"

#include <iostream>  
#pragma endregion


CollisionComponent::CollisionComponent(GameObject* gameObject) : gameObject(gameObject)
{
}

void CollisionComponent::Update(const float& deltaTime)
{
	if (!IsEnabled()) return;

	for (GameObject* gameObject : SRVEngine.GetAllGameObjects())
	{
		CollisionComponent* colliderComponent = gameObject->GetComponent<CollisionComponent>();

		if (!colliderComponent)
			continue;

		if (!colliderComponent->IsEnabled())
			continue;

		if (colliderComponent == this)
			continue;

		if (Intersects(*colliderComponent))
		{
			OnCollisionEnter.Broadcast(colliderComponent);
		}
	}
}

GameObject* CollisionComponent::GetGameObject() const
{
	return gameObject;
}

bool CollisionComponent::Intersects(const CollisionComponent& other) const
{
	return GetBoundingVolume().Intersects(other.GetBoundingVolume());
}
