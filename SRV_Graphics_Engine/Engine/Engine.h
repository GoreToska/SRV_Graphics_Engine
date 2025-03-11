#pragma once

#define SRVEngine Engine::GetInstance()

#include <vector>

#include "../Render/WindowContainer.h"
#include "../ComponentSystem/GameObject.h"
#include "Timer/Timer.h"
#include "../Graphics/Graphics.h"


class Engine : WindowContainer
{
public:
	Engine() {};
	Engine(Engine const&) = delete;
	void operator =(Engine const&) = delete;

	static Engine& GetInstance();

	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	void AddGameObject(GameObject* gameObject);
	bool ProcessMessages();
	void Update(float deltaTime);
	void RenderFrame();
	Graphics& GetGraphics();

	Timer* GetTimer();
	std::vector<GameObject*> GetAllGameObjects();

private:
	Timer timer;
	std::vector<GameObject*> gameObjects{};
};

