#pragma once

#include "../../ComponentSystem/Components/IComponent.h"
#include "../../ComponentSystem/Components/Collision/SphereCollisionComponent.h"
#include "../../ComponentSystem/GameObject.h"

class KatamariCollisionComponent : public IComponent
{
public:
	KatamariCollisionComponent(GameObject* gameObject);

	void Update(const float& deltaTime) override;

private:
	void OnCollide(CollisionComponent* other);

	SphereCollisionComponent* collision;
	GameObject* gameObject;
};

