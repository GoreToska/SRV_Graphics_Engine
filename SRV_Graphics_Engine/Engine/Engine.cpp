#pragma region Includes
#include <iostream>

#include "Engine.h"
#include "../Input/Keyboard/Keyboard.h"
#include "../Input/Keyboard/KeyboardInputEvent.h"
#include "../Input/Mouse/Mouse.h"
#include "../Input/Mouse/MouseInputEvent.h"
#include "../DataTypes/VectorInt2D.h"
#include "../ComponentSystem/GameObject.h"  
#pragma endregion


Engine& Engine::GetInstance()
{
	static Engine engine;

	return engine;
}

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	if (!renderWindow.Initialize(this, hInstance, window_title, window_class, width, height))
		return false;

	if (!graphics.Initialize(renderWindow.GetHWND(), width, height))
		return false;

	return true;
}

void Engine::AddGameObject(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);

	RenderComponent* renderComponent = gameObject->GetComponent<RenderComponent>();
	if (renderComponent != nullptr)
	{
		graphics.AddObjectToRenderPool(renderComponent);
	}
}

bool Engine::ProcessMessages()
{
	return renderWindow.ProcessMessages();
}

void Engine::Update()
{
	for (GameObject* gameObject : gameObjects)
	{
		gameObjects[i]->Update(deltaTime);
	}

	/*#pragma region Input
	
		while (!Keyboard::GetInstance().CharBufferIsEmpty())
		{
			unsigned char a = Keyboard::GetInstance().ReadChar();
		}
	
		while (!Keyboard::GetInstance().KeyBufferIsEmpty())
		{
			KeyboardInputEvent e = Keyboard::GetInstance().ReadKey();
		}
	
		while (!Mouse::GetInstance().EventBufferIsEmpty())
		{
			MouseInputEvent e = Mouse::GetInstance().ReadEvent();
		}
	#pragma endregion*/
}

void Engine::RenderFrame()
{
	graphics.RenderFrame();
}

Graphics& Engine::GetGraphics()
{
	return graphics;
}

Timer* Engine::GetTimer()
{
	return &timer;
}

std::vector<GameObject*> Engine::GetAllGameObjects()
{
	return gameObjects;
}
