#pragma once
#include "../../ComponentSystem/GameObject.h"
#include "../../ComponentSystem/Components/CollisionComponent.h"


class PongGameMode
{
public:
	PongGameMode(GameObject* ball, CollisionComponent* leftCollider, CollisionComponent* rightCollider,
		CollisionComponent* leftRocket, CollisionComponent* rightRocket);

	void OnLeftColliderHit(CollisionComponent* owner, CollisionComponent* other);
	void OnRightColliderHit(CollisionComponent* owner, CollisionComponent* other);

	void OnLeftRocketHit(CollisionComponent* owner, CollisionComponent* other);
	void OnRightRocketHit(CollisionComponent* owner, CollisionComponent* other);


	void Restart(GameObject* ball);
	void PrintScore();

private:
	int leftScore = 0;
	int rightScore = 0;

	GameObject* ball;

	CollisionComponent* leftPlayer;
	CollisionComponent* rightPlayer;
};

