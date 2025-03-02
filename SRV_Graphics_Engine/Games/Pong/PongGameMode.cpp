#include "PongGameMode.h"
#include <iostream>

PongGameMode::PongGameMode(GameObject* ball, CollisionComponent* leftCollider, CollisionComponent* rightCollider,
	CollisionComponent* leftRocket, CollisionComponent* rightRocket)
{
	this->ball = ball;

	leftPlayer = leftRocket;
	rightPlayer = rightRocket;

	/*leftPlayer->OnCollisionEnter.AddRaw(this, &PongGameMode::OnLeftRocketHit);
	rightPlayer->OnCollisionEnter.AddRaw(this, &PongGameMode::OnRightRocketHit);*/

	leftCollider->OnCollisionEnter.AddRaw(this, &PongGameMode::OnLeftColliderHit);
	rightCollider->OnCollisionEnter.AddRaw(this, &PongGameMode::OnRightColliderHit);
}

void PongGameMode::OnLeftColliderHit(CollisionComponent* owner, CollisionComponent* other)
{
	++rightScore;
	PrintScore();
	Restart(other->GetGameObject());
}

void PongGameMode::OnRightColliderHit(CollisionComponent* owner, CollisionComponent* other)
{
	++leftScore;
	PrintScore();
	Restart(other->GetGameObject());
}

void PongGameMode::OnLeftRocketHit(CollisionComponent* owner, CollisionComponent* other)
{
	leftPlayer->IsEnabled = false;
	rightPlayer->IsEnabled = true;
}

void PongGameMode::OnRightRocketHit(CollisionComponent* owner, CollisionComponent* other)
{
	leftPlayer->IsEnabled = true;
	rightPlayer->IsEnabled = false;
}

void PongGameMode::Restart(GameObject* ball)
{
	ball->GetTransform()->SetPosition(Vector3D(0, 0, 0));
	leftPlayer->IsEnabled = true;
	rightPlayer->IsEnabled = true;
}

void PongGameMode::PrintScore()
{
	std::cout << "Left: " << leftScore << "\tRight: " << rightScore << "\n";
}

