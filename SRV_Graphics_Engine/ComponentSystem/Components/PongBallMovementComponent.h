#pragma once

#include "IComponent.h"
#include "../GameObject.h"
#include "CollisionComponent.h"

class PongBallMovementComponent : public IComponent
{
public:
	PongBallMovementComponent(GameObject* gameObject, float xSpeed, float ySpeed);

	void Update() override;
	void OnCollide(CollisionComponent* collider, GameObject* gameObject);

private:
	float xSpeed;
	float ySpeed;

	GameObject* gameObject;
};

