#include "PongSpawnerComponent.h"
#include "../../ComponentSystem/GameObject.h"
#include <iostream>
#include "../../Shapes2D/Shapes2D.h"
#include "../../ComponentSystem/Components/PongBallMovementComponent.h"
#include "../../Engine/Engine.h"

PongSpawnerComponent::PongSpawnerComponent(GameObject* ownerBall)
{
	owner = ownerBall;
	owner->GetComponent<CollisionComponent>()->OnCollisionEnter.AddRaw(this, &PongSpawnerComponent::OnCollision);
}

void PongSpawnerComponent::Update(float deltaTime)
{
}

void PongSpawnerComponent::OnCollision(CollisionComponent* owner, CollisionComponent* other)
{
	Vector3D adjustedPosition = Vector3D(
		this->owner->GetTransform()->GetPosition().x - 50 * this->owner->GetComponent<PongBallMovementComponent>()->GetDirection().x,
		this->owner->GetTransform()->GetPosition().y,
		this->owner->GetTransform()->GetPosition().z);

	this->owner->GetTransform()->SetPosition(adjustedPosition);

	if (other->collisionChannel == CollisionComponent::CollisionChannel::Channel_1)
	{
		std::cout << "Collide with rocket!\n";

		for (int i = 0; i < ballSpawnCount; ++i)
		{
			SpawnBalls(i);
		}
	}
}

void PongSpawnerComponent::SpawnBalls(int i)
{
	auto circleShape = Shapes2D::GetCircleShape(0.02, 30);

	GameObject* ball = new GameObject(owner->GetTransform()->GetPosition() + owner->GetComponent<PongBallMovementComponent>()->GetDirection() * -20);

	RenderComponent* ballRender = new RenderComponent(ball->GetTransform(), std::get<0>(circleShape), std::get<1>(circleShape));
	CollisionComponent* boxCollision = new CollisionComponent(ball, Vector3D(-0.02f / 1.33, -0.02f, 1.0f), Vector3D(0.0125f / 1.33, 0.02f, 1.0f), CollisionComponent::Channel_2);
	boxCollision->AddIgnoreChannel(CollisionComponent::Channel_2);

	ball->AddComponent(boxCollision);
	ball->AddComponent(new PongSpawnerComponent(ball));

	Vector3D movementVector = owner->GetComponent<PongBallMovementComponent>()->GetDirection();
	PongBallMovementComponent* ballMovement = new PongBallMovementComponent(ball,
		movementVector.x,
		movementVector.y);

	ballMovement->SetRandomSpeed();
	ballMovement->ChangeDirection();

	ball->AddComponent(ballMovement);
	ball->AddComponent(ballRender);

	SRVEngine.AddGameObject(ball);
}
