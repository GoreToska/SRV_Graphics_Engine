#include "SphereCollisionComponent.h"
#include "../../../Engine/Engine.h"

SphereCollisionComponent::SphereCollisionComponent(GameObject* gameObject, Vector3D center, float radius) :
	CollisionComponent(gameObject), transform(gameObject->GetTransform())
{
	DirectX::BoundingSphere sphereCollider{ {center.x, center.y, center.z}, radius };

	boundingVolume = std::make_unique<BoundingSphere>(sphereCollider);
}

void SphereCollisionComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;

	boundingVolume.get()->SetCenter(transform->GetPosition());

	CollisionComponent::Update(deltaTime);
}

BoundingVolume& SphereCollisionComponent::GetBoundingVolume()
{
	return *boundingVolume.get();
}