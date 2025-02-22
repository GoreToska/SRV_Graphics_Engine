#include <Windows.h>
#include <iostream>

#include "./Utils/Logger.h"
#include "./Engine/Engine.h"

#include "./Input/Keyboard/Keyboard.h"
#include"./Input/PongInput.h"
#include"./Shapes2D/Shapes2D.h"
#include "ComponentSystem/GameObject.h"
#include "ComponentSystem/Components/PongInputComponent.h"
#include "ComponentSystem/Components/CollisionComponent.h"

void PongScene()
{
	std::vector<Vertex3D> ballVertices
	{
		Vertex3D({-0.03f, -0.04f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({-0.03f, 0.04f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.03f, 0.04f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.03f, -0.04f, 1.0f}, {1.0f, 1.0f, 1.0f}),
	};
	std::vector<DWORD> ballIndecies
	{
		0,1,2,
		0,2,3
	};

	Vector3D ballPosition = Vector3D(-0.9, 0, 0);
	GameObject* ball = new GameObject();
	RenderComponent* ballRender = new RenderComponent(ball->GetTransform(), ballVertices, ballIndecies);
	ball->AddComponent(ballRender);
	ball->GetTransform()->SetPosition(ballPosition);
	CollisionComponent* boxCollision = new CollisionComponent(ball->GetTransform(), Vector3D(-0.03f, -0.04f, 1.0f), Vector3D(0.03f, 0.04f, 1.0f));
	ball->AddComponent(boxCollision);
	SRVEngine.AddGameObject(ball);

	std::vector<Vertex3D> rocketVertices
	{
		Vertex3D({-0.01f, -0.3f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({-0.01f, 0.3f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.01f, 0.3f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.01f, -0.3f, 1.0f}, {1.0f, 1.0f, 1.0f}),
	};
	std::vector<DWORD> rocketIndecies
	{
		0,1,2,
		0,2,3
	};

	Vector3D leftPosition = Vector3D(-0.9, 0, 0);
	GameObject* leftPlayer = new GameObject();
	RenderComponent* leftRenderComponent = new RenderComponent(leftPlayer->GetTransform(), rocketVertices, rocketIndecies);
	PongInputComponent* leftInputComponent = new PongInputComponent(leftPlayer);
	CollisionComponent* leftCollision = new CollisionComponent(leftPlayer->GetTransform(), Vector3D(-0.01f, -0.3f, 1.0f), Vector3D(0.01f, 0.3f, 1.0f));
	leftInputComponent->SetInput('W', 'S');

	leftPlayer->AddComponent(leftRenderComponent);
	leftPlayer->AddComponent(leftInputComponent);
	leftPlayer->AddComponent(leftCollision);
	leftPlayer->GetTransform()->SetPosition(leftPosition);
	SRVEngine.AddGameObject(leftPlayer);

	Vector3D rightPosition = Vector3D(0.9, 0, 0);
	GameObject* rightPlayer = new GameObject();
	RenderComponent* rightRenderComponent = new RenderComponent(rightPlayer->GetTransform(), rocketVertices, rocketIndecies);
	PongInputComponent* rightInputComponent = new PongInputComponent(rightPlayer);
	rightInputComponent->SetInput('&', '(');

	rightPlayer->AddComponent(rightRenderComponent);
	rightPlayer->AddComponent(rightInputComponent);
	rightPlayer->GetTransform()->SetPosition(rightPosition);
	SRVEngine.AddGameObject(rightPlayer);
}

void OnKeyPressed(const unsigned char key)
{
	std::cout << key << std::endl;
}

int main()
{
	std::string applicationName = "My3DApp";
	std::string windowClass = "WindowClass";
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	SRVEngine.Initialize(hInstance, applicationName, windowClass, 1000, 600);


	PongScene();


	while (SRVEngine.ProcessMessages())
	{
		SRVEngine.Update();
		SRVEngine.RenderFrame();
	}
}

