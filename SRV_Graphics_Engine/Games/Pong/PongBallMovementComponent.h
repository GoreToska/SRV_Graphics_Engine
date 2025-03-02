#pragma once

#include "../../ComponentSystem/Components/IComponent.h"
#include "../../ComponentSystem/Components/CollisionComponent.h"
#include "../../ComponentSystem/GameObject.h"

class PongBallMovementComponent : public IComponent
{
public:
	PongBallMovementComponent(GameObject* gameObject, float xSpeed, float ySpeed);

	void Update(const float& deltaTime) override;
	void OnCollide(CollisionComponent* owner, CollisionComponent* other);

	Vector3D GetDirection();
	void SetRandomSpeed();
	void ChangeDirection();

private:
	float xSpeed;
	float ySpeed;

	GameObject* gameObject;
};

