#pragma region Includes
#include "CollisionComponent.h"
#include "../../GameObject.h"
#include "../../../Engine/Engine.h"
#include "../TransformComponent.h"

#include <iostream>  
#pragma endregion


CollisionComponent::CollisionComponent(GameObject* gameObject) : gameObject(gameObject), transform(gameObject->GetTransform())
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

		if (gameObject->GetRoot() == this->gameObject->GetRoot())
			continue;

		if (Intersects(*colliderComponent))
		{
			OnCollisionEnter.Broadcast(colliderComponent);
		}
	}
}

void CollisionComponent::SetRadius(const float& radius)
{
	GetBoundingVolume().SetRadius(radius);
}

float CollisionComponent::GetRadius()
{
	return GetBoundingVolume().GetRadius();
}

GameObject* CollisionComponent::GetGameObject() const
{
	return gameObject;
}

bool CollisionComponent::Intersects(CollisionComponent& other)
{
	return GetBoundingVolume().Intersects(other.GetBoundingVolume());
}
