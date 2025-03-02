#include "PongBallMovementComponent.h"
#include "../../DataTypes/Vector3D.h"

#include <random>


PongBallMovementComponent::PongBallMovementComponent(GameObject* gameObject, float xSpeed, float ySpeed)
{
	this->gameObject = gameObject;

	this->xSpeed = xSpeed;
	this->ySpeed = ySpeed;

	gameObject->GetComponent<CollisionComponent>()->OnCollisionEnter.AddRaw(this, &PongBallMovementComponent::OnCollide);
}

void PongBallMovementComponent::Update(const float& deltaTime)
{
	gameObject->GetTransform()->MovePosition(Vector3D(xSpeed, ySpeed, 0));
	Vector3D position = gameObject->GetTransform()->GetPosition();

	if (position.x < -1 || position.x > 1)
	{
		xSpeed *= -1;
	}

	if (position.y < -1 || position.y > 1)
	{
		ySpeed *= -1;
	}
}

void PongBallMovementComponent::OnCollide(CollisionComponent* collider, GameObject* gameObject)
{
	xSpeed *= -1;
}
