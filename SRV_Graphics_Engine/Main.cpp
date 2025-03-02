#pragma region Includes
#include <Windows.h>
#include <iostream>

#include "./Utils/Logger.h"
#include "./Engine/Engine.h"
#include "./Input/Keyboard/Keyboard.h"
#include"./Shapes2D/Shapes2D.h"
#include "ComponentSystem/GameObject.h"
#include "ComponentSystem/Components/PongInputComponent.h"
#include "ComponentSystem/Components/CollisionComponent.h"  
#include "ComponentSystem/Components/PongBallMovementComponent.h"
#include "Games/Pong/PongSpawnerComponent.h"
#include "Games/Pong/PongGameMode.h"
#pragma endregion


void PongScene()
{
	std::string applicationName = "Pong";
	std::string windowClass = "WindowClass";
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	SRVEngine.Initialize(hInstance, applicationName, windowClass, 1024, 768);

#pragma region Ball
	std::vector<Vertex3D> ballVertices
	{
		Vertex3D({-0.01f, -0.02f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({-0.01f, 0.02f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.01f, 0.02f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.01f, -0.02f, 1.0f}, {1.0f, 1.0f, 1.0f}),
	};
	std::vector<DWORD> ballIndecies
	{
		0,1,2,
		0,2,3
	};

	GameObject* ball = new GameObject(Vector3D(0.0f, 0.0f, 0.0f));
	ball->name = "Main ball";
	auto circleShape = Shapes2D::GetCircleShape(0.02, 30);
	RenderComponent* ballRender = new RenderComponent(ball->GetTransform(), std::get<0>(circleShape), std::get<1>(circleShape));
	CollisionComponent* boxCollision =
		new CollisionComponent(ball, Vector3D(-0.02f / 1.33, -0.02f, 1.0f), Vector3D(0.0125f / 1.33, 0.02f, 1.0f), CollisionComponent::Channel_2);
	boxCollision->AddIgnoreChannel(CollisionComponent::Channel_2);

	ball->AddComponent(ballRender);
	ball->AddComponent(boxCollision);
	ball->AddComponent(new PongSpawnerComponent(ball));

	PongBallMovementComponent* ballMovement = new PongBallMovementComponent(ball, 0.0005, 0.0003);
	ball->AddComponent(ballMovement);
	SRVEngine.AddGameObject(ball);
#pragma endregion

#pragma region Rockets
	std::vector<Vertex3D> rocketVertices
	{
		Vertex3D({-0.01f, -0.2f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({-0.01f, 0.2f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.01f, 0.2f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.01f, -0.2f, 1.0f}, {1.0f, 1.0f, 1.0f}),
	};
	std::vector<DWORD> rocketIndecies
	{
		0,1,2,
		0,2,3
	};

	GameObject* leftPlayer = new GameObject(Vector3D(-0.9, 0, 0));
	RenderComponent* leftRenderComponent = new RenderComponent(leftPlayer->GetTransform(), rocketVertices, rocketIndecies);
	PongInputComponent* leftInputComponent = new PongInputComponent(leftPlayer, 0.4);
	CollisionComponent* leftCollision = new CollisionComponent(leftPlayer, Vector3D(-0.00001f, -0.2f, 1.0f), Vector3D(0.01f, 0.2f, 1.0f), CollisionComponent::Channel_1);

	leftInputComponent->SetInput('W', 'S');
	leftInputComponent->SetSpeed(0.001);
	leftPlayer->AddComponent(leftRenderComponent);
	leftPlayer->AddComponent(leftInputComponent);
	leftPlayer->AddComponent(leftCollision);
	SRVEngine.AddGameObject(leftPlayer);

	GameObject* rightPlayer = new GameObject(Vector3D(0.9, 0, 0));
	RenderComponent* rightRenderComponent = new RenderComponent(rightPlayer->GetTransform(), rocketVertices, rocketIndecies);
	PongInputComponent* rightInputComponent = new PongInputComponent(rightPlayer, 0.4);
	CollisionComponent* rightCollision = new CollisionComponent(rightPlayer, Vector3D(-0.01f, -0.2f, 1.0f), Vector3D(0.00001f, 0.2f, 1.0f), CollisionComponent::Channel_1);

	rightInputComponent->SetInput('&', '(');
	rightInputComponent->SetSpeed(0.001);
	rightPlayer->AddComponent(rightRenderComponent);
	rightPlayer->AddComponent(rightCollision);
	rightPlayer->AddComponent(rightInputComponent);
	SRVEngine.AddGameObject(rightPlayer);
#pragma endregion

#pragma region Colliders

	std::vector<Vertex3D> colliderVertices
	{
		Vertex3D({-0.05f, -1, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({-0.05f, 1, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.05f, 1, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.05f, -1, 1.0f}, {1.0f, 1.0f, 1.0f}),
	};
	std::vector<DWORD> colliderIndecies
	{
		0,1,2,
		0,2,3
	};

	GameObject* leftCollider = new GameObject();
	leftCollider->GetTransform()->SetPosition(Vector3D(-1, 0, 0));
	//leftCollider->AddComponent(new RenderComponent(leftCollider->GetTransform(), colliderVertices, colliderIndecies));
	leftCollider->AddComponent(new CollisionComponent(leftCollider, { -0.05f, -1, 1.0f }, { 0.05f, 1, 1.0f }, CollisionComponent::Channel_3));

	GameObject* rightCollider = new GameObject();
	rightCollider->GetTransform()->SetPosition(Vector3D(1, 0, 0));
	//rightCollider->AddComponent(new RenderComponent(rightCollider->GetTransform(), colliderVertices, colliderIndecies));
	rightCollider->AddComponent(new CollisionComponent(rightCollider, { -0.05f, -1, 1.0f }, { 0.05f, 1, 1.0f }, CollisionComponent::Channel_3));

	SRVEngine.AddGameObject(leftCollider);
	SRVEngine.AddGameObject(rightCollider);
#pragma endregion

	PongGameMode* pongGameMode = new PongGameMode(ball,
		leftCollider->GetComponent<CollisionComponent>(),
		rightCollider->GetComponent<CollisionComponent>(),
		leftPlayer->GetComponent<CollisionComponent>(),
		rightPlayer->GetComponent<CollisionComponent>());
}

void OnKeyPressed(const unsigned char key)
{
	std::cout << key << std::endl;
}

int main()
{
	PongScene();


	while (SRVEngine.ProcessMessages())
	{
		float deltaTime = SRVEngine.GetTimer()->GetMilisecondsElapsed();
		SRVEngine.GetTimer()->Restart();

		SRVEngine.Update(deltaTime);
		SRVEngine.RenderFrame();
	}
}

