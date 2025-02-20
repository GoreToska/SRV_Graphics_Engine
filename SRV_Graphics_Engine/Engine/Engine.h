#pragma once

#include "../Render/WindowContainer.h"

class Engine : WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	void AddGameObject(RenderComponent* component);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
};

