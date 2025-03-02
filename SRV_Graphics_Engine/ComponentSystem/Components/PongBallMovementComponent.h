#pragma once

#include "IComponent.h"
#include "../GameObject.h"
#include "CollisionComponent.h"

class PongBallMovementComponent : public IComponent
{
public:
	PongBallMovementComponent(GameObject* gameObject, float xSpeed, float ySpeed);

	void Update(float deltaTime) override;
	void OnCollide(CollisionComponent* owner, CollisionComponent* other);

	Vector3D GetDirection();
	void SetRandomSpeed();
	void ChangeDirection();

private:
	float xSpeed;
	float ySpeed;

	GameObject* gameObject;
};

