#include <Windows.h>
#include <iostream>

#include "./Utils/Logger.h"
#include "./Engine/Engine.h"

#include "./Input/Keyboard/Keyboard.h"
#include"./Input/PongInput.h"
#include"./Shapes2D/Shapes2D.h"


void PongScene(Engine* engine)
{
	RenderComponent* ball = new RenderComponent
	{
		{
		Vertex3D({-0.03f, -0.04f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({-0.03f, 0.04f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.03f, 0.04f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.03f, -0.04f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		},
		{
			0,1,2,
			0,2,3
		}
	};

	ball->SetPosition({ 0.0f, 0.0f , 1 });
	ball->id = 0;
	engine->AddGameObject(ball);

	RenderComponent* leftPlayer = new RenderComponent
	{
		{
		Vertex3D({-0.01f, -0.3f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({-0.01f, 0.3f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.01f, 0.3f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.01f, -0.3f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		},
		{
			0,1,2,
			0,2,3
		}
	};

	leftPlayer->SetPosition({ -0.9, 0.0f, 1 });
	leftPlayer->id = 1;
	leftPlayer->MovePosition({ 1, 1, 1 });
	engine->AddGameObject(leftPlayer);


	

	/*RenderComponent* ball2 = new RenderComponent
	{
		{
		Vertex3D({-0.1f, -0.01f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({-0.1f, 0.01f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.1f, 0.01f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.1f, -0.01f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		},
		{
			0,1,2,
			0,2,3
		}
	};

	ball2->SetPosition({ 0.0f, 0.0f , 1 });
	ball2->id = 3;
	engine->AddGameObject(ball2);*/

	// input
	PongInput* input1 = new PongInput();
	//PongInput* input2 = new PongInput();
	Keyboard::GetInstance().EnableAutoRepeatKeys();
	Keyboard::GetInstance().KeyPressedEvent.AddRaw(input1, &PongInput::PlayerInput);
	//Keyboard::GetInstance().KeyPressedAction.AddRaw(input2, &PongInput::PlayerInput);

	input1->SetInput('W', 'S');
	input1->SetInputTarget(ball);

	//input2->SetInput('W', 'S');
	//input2->SetInputTarget(leftPlayer);

	//auto ballShape = Shapes2D::GetCircleShape({ 0.0,0.0,0.0, }, 0.1, 32, { 1,0,0 });
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
	Engine engine;
	engine.Initialize(hInstance, applicationName, windowClass, 1000, 600);


	PongScene(&engine);


	while (engine.ProcessMessages())
	{
		engine.Update();
		engine.RenderFrame();
	}
}

