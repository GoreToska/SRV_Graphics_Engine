#include "Engine.h"
#include "../Input/Keyboard/Keyboard.h"
#include "../Input/Keyboard/KeyboardInputEvent.h"
#include "../Input/Mouse/Mouse.h"
#include "../Input/Mouse/MouseInputEvent.h"
#include "../DataTypes/VectorInt2D.h"
#include <iostream>


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

	IComponent* renderComponent = gameObject->GetComponent(IComponent::RenderComponentType);
	if (renderComponent != nullptr)
	{
		graphics.AddObjectToRenderPool(dynamic_cast<RenderComponent*>(renderComponent));
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
		gameObject->Update();
	}

#pragma region Input


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

	Keyboard::GetInstance().BroadcastKeyPressed(Keyboard::GetInstance().ReadKey().GetKeyCode());

	/*if (Keyboard::GetInstance().IsKeyPressed('W'))
	{
		Keyboard::GetInstance().BroadcastKeyPressed('W');
	}

	if (Keyboard::GetInstance().IsKeyPressed('S'))
	{
		Keyboard::GetInstance().BroadcastKeyPressed('S');
	}*/
#pragma endregion
}

void Engine::RenderFrame()
{
	graphics.RenderFrame();
}
