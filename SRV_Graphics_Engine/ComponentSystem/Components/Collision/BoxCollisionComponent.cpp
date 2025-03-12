#include "BoxCollisionComponent.h"
#include "../../../Engine/Engine.h"


BoxCollisionComponent::BoxCollisionComponent(GameObject* gameObject, Vector3D firstPoint, Vector3D secondPoint) :
	CollisionComponent(gameObject), transform(gameObject->GetTransform())
{
	DirectX::BoundingBox boxCollider;
	DirectX::BoundingBox::CreateFromPoints(boxCollider,
		{ firstPoint.x,firstPoint.y,firstPoint.z },
		{ secondPoint.x,secondPoint.y,secondPoint.z });

	boundingVolume = std::make_unique<BoundingBox>(boxCollider);
}

void BoxCollisionComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;

	boundingVolume.get()->SetCenter(transform->GetPosition());

	CollisionComponent::Update(deltaTime);
}

BoundingVolume& BoxCollisionComponent::GetBoundingVolume()
{
	return *boundingVolume.get();
}
