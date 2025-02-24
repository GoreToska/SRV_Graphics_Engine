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
#include "Games/Pong/PongGameMode.h"
#pragma endregion


void PongScene()
{
	std::string applicationName = "Pong";
	std::string windowClass = "WindowClass";
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	SRVEngine.Initialize(hInstance, applicationName, windowClass, 1000, 600);

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

	Vector3D ballPosition = Vector3D(0.0f, 0.0f, 0.0f);
	GameObject* ball = new GameObject();
	auto circleShape = Shapes2D::GetCircleShape( 0.1, 32);
	RenderComponent* ballRender = new RenderComponent(ball->GetTransform(), ballVertices, ballIndecies);
	CollisionComponent* boxCollision = new CollisionComponent(ball, Vector3D(-0.01f, -0.02f, 1.0f), Vector3D(0.01f, 0.02f, 1.0f));

	ball->AddComponent(ballRender);
	ball->GetTransform()->SetPosition(ballPosition);
	ball->AddComponent(boxCollision);

	PongBallMovementComponent* ballMovement = new PongBallMovementComponent(ball, 0.005, 0.003);
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

	Vector3D leftPosition = Vector3D(-0.9, 0, 0);
	GameObject* leftPlayer = new GameObject();
	RenderComponent* leftRenderComponent = new RenderComponent(leftPlayer->GetTransform(), rocketVertices, rocketIndecies);
	PongInputComponent* leftInputComponent = new PongInputComponent(leftPlayer, 0.4);
	CollisionComponent* leftCollision = new CollisionComponent(leftPlayer, Vector3D(-0.01f, -0.2f, 1.0f), Vector3D(0.01f, 0.2f, 1.0f));

	leftInputComponent->SetInput('W', 'S');
	leftPlayer->AddComponent(leftRenderComponent);
	leftPlayer->AddComponent(leftInputComponent);
	leftPlayer->AddComponent(leftCollision);
	leftPlayer->GetTransform()->SetPosition(leftPosition);
	SRVEngine.AddGameObject(leftPlayer);

	Vector3D rightPosition = Vector3D(0.9, 0, 0);
	GameObject* rightPlayer = new GameObject();
	RenderComponent* rightRenderComponent = new RenderComponent(rightPlayer->GetTransform(), rocketVertices, rocketIndecies);
	PongInputComponent* rightInputComponent = new PongInputComponent(rightPlayer, 0.4);
	CollisionComponent* rightCollision = new CollisionComponent(rightPlayer, Vector3D(-0.01f, -0.2f, 1.0f), Vector3D(0.01f, 0.2f, 1.0f));

	rightInputComponent->SetInput('&', '(');
	rightPlayer->AddComponent(rightRenderComponent);
	rightPlayer->AddComponent(rightCollision);
	rightPlayer->AddComponent(rightInputComponent);
	rightPlayer->GetTransform()->SetPosition(rightPosition);
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
	leftCollider->AddComponent(new CollisionComponent(leftCollider, { -0.05f, -1, 1.0f }, { 0.05f, 1, 1.0f }));

	GameObject* rightCollider = new GameObject();
	rightCollider->GetTransform()->SetPosition(Vector3D(1, 0, 0));
	//rightCollider->AddComponent(new RenderComponent(rightCollider->GetTransform(), colliderVertices, colliderIndecies));
	rightCollider->AddComponent(new CollisionComponent(rightCollider, { -0.05f, -1, 1.0f }, { 0.05f, 1, 1.0f }));

	SRVEngine.AddGameObject(leftCollider);
	SRVEngine.AddGameObject(rightCollider);
#pragma endregion

	PongGameMode* pongGameMode = new PongGameMode(ball,
		leftCollider->GetComponent<CollisionComponent>(),
		rightCollider->GetComponent<CollisionComponent>());
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
		SRVEngine.Update();
		SRVEngine.RenderFrame();
	}
}

