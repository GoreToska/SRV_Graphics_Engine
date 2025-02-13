#include <Windows.h>
#include <iostream>

#include "./Utils/Logger.h"
#include "./Engine/Engine.h"


// 

int main()
{
	std::string applicationName = "My3DApp";
	std::string windowClass = "WindowClass";
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	Engine engine;
	engine.Initialize(hInstance, applicationName, windowClass, 800, 600);

	while (engine.ProcessMessages())
	{
		engine.Update();
		engine.RenderFrame();
	}
}