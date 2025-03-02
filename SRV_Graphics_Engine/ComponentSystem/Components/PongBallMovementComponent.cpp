#include "PongBallMovementComponent.h"
#include <random>

PongBallMovementComponent::PongBallMovementComponent(GameObject* gameObject, float xSpeed, float ySpeed)
{
	this->gameObject = gameObject;

	this->xSpeed = xSpeed;
	this->ySpeed = ySpeed;

	gameObject->GetComponent<CollisionComponent>()->OnCollisionEnter.AddRaw(this, &PongBallMovementComponent::OnCollide);
}

void PongBallMovementComponent::Update(float deltaTime)
{
	gameObject->GetTransform()->MovePosition(Vector3D(xSpeed * deltaTime, ySpeed * deltaTime, 0));
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

void PongBallMovementComponent::OnCollide(CollisionComponent* owner, CollisionComponent* other)
{
	xSpeed *= -1;
}

Vector3D PongBallMovementComponent::GetDirection()
{
	return Vector3D(xSpeed, ySpeed, 0);
}

void PongBallMovementComponent::SetRandomSpeed()
{

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<> dist6(0.8, 1.2);

	float value = dist6(rng);
	xSpeed *= value;
	ySpeed *= value;
}

void PongBallMovementComponent::ChangeDirection()
{
	xSpeed *= -1;
}
