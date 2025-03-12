#include "KatamariCollisionComponent.h"

KatamariCollisionComponent::KatamariCollisionComponent(GameObject* gameObject) :
	gameObject(gameObject)
{
	collision = static_cast<SphereCollisionComponent*>(this->gameObject->GetComponent<CollisionComponent>());
	collision->OnCollisionEnter.AddRaw(this, &KatamariCollisionComponent::OnCollide);
}

void KatamariCollisionComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;
}

void KatamariCollisionComponent::OnCollide(CollisionComponent* other)
{
	if (other->GetRadius() > collision->GetRadius())
		return;

	gameObject->AttachChild(other->GetGameObject());
	gameObject->GetTransform()->SetScale(gameObject->GetTransform()->GetScale() + Vector3D(0.003,0.003,0.003));
	collision->SetRadius(collision->GetRadius() + 0.6);
}
