#pragma once

#ifndef SRVEngine
#define SRVEngine Engine::GetInstance()
#endif // !SRVEngine

#include "../Render/WindowContainer.h"
#include "../ComponentSystem/GameObject.h"


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
	void Update();
	void RenderFrame();
	Graphics& GetGraphics();

	Timer* GetTimer();
	std::vector<GameObject*> GetAllGameObjects();

private:
	std::vector<GameObject*> gameObjects{};
};

