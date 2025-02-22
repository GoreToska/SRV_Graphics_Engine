#pragma once

#include "../Render/WindowContainer.h"
#include "../ComponentSystem/GameObject.h"

class Engine : WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	void AddGameObject(GameObject* gameObject);
	bool ProcessMessages();
	void Update();
	void RenderFrame();

private:
	std::vector<GameObject*> gameObjects{};
};

