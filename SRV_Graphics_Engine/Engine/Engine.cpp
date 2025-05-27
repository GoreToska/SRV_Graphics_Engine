#pragma region Includes
#include <iostream>

#include "Engine.h"
#include "../Input/Keyboard/Keyboard.h"
#include "../Input/Keyboard/KeyboardInputEvent.h"
#include "../Input/Mouse/Mouse.h"
#include "../Input/Mouse/MouseInputEvent.h"
#include "../ComponentSystem/GameObject.h"  
#include "../ComponentSystem/Components/Render/IRenderComponent.h"
#include "../ComponentSystem/Components/Particles/ParticleSystem.h"

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

	grid = new Grid();

	return true;
}

void Engine::AddGameObject(GameObject* gameObject)
{

	IRenderComponent* renderComponent = gameObject->GetComponent<IRenderComponent>();
	if (renderComponent != nullptr)
	{
		graphics.AddObjectToRenderPool(renderComponent);
	}

	ParticleSystem* particleSystem = gameObject->GetComponent<ParticleSystem>();
	if (particleSystem != nullptr)
	{
		graphics.AddParticleSystemToPool(particleSystem);
		return;
	}

	gameObjects.push_back(gameObject);

}

bool Engine::ProcessMessages()
{
	return renderWindow.ProcessMessages();
}

void Engine::Update(float deltaTime)
{
	for (int i = 0; i < gameObjects.size(); ++i)
	{
		gameObjects[i]->Update(deltaTime);
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
#pragma endregion
}

void Engine::RenderFrame()
{
	graphics.RenderFrame();
	//grid->Draw();
}

Graphics& Engine::GetGraphics()
{
	return graphics;
}

float Engine::GetDeltaSeconds()
{
	return timer.GetMilisecondsElapsed();
}

Timer* Engine::GetTimer()
{
	return &timer;
}

std::vector<GameObject*> Engine::GetAllGameObjects()
{
	return gameObjects;
}
