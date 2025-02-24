#include "PongGameMode.h"
#include <iostream>

PongGameMode::PongGameMode(GameObject* ball, CollisionComponent* leftCollider, CollisionComponent* rightCollider)
{
	this->ball = ball;

	leftCollider->OnCollisionEnter.AddRaw(this, &PongGameMode::OnLeftColliderHit);
	rightCollider->OnCollisionEnter.AddRaw(this, &PongGameMode::OnRightColliderHit);
}

void PongGameMode::OnLeftColliderHit(CollisionComponent* collider)
{
	++rightScore;
	PrintScore();
	Restart();
}

void PongGameMode::OnRightColliderHit(CollisionComponent* collider)
{
	++leftScore;
	PrintScore();
	Restart();
}

void PongGameMode::Restart()
{
	ball->GetTransform()->SetPosition(Vector3D(0, 0, 0));
}

void PongGameMode::PrintScore()
{
	std::cout << "Left: " << leftScore << "\tRight: " << rightScore << "\n";
}

