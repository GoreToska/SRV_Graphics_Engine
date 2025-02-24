#pragma once
#include "../../ComponentSystem/GameObject.h"
#include "../../ComponentSystem/Components/CollisionComponent.h"


class PongGameMode
{
public:
	PongGameMode(GameObject* ball, CollisionComponent* leftCollider, CollisionComponent* rightCollider);

	void OnLeftColliderHit(CollisionComponent* collider);
	void OnRightColliderHit(CollisionComponent* collider);

	void Start();
	void Restart();
	void PrintScore();

private:
	int leftScore = 0;
	int rightScore = 0;

	GameObject* ball;
};

