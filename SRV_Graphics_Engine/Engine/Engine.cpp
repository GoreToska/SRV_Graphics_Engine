#include "Engine.h"
#include "../Input/Keyboard/Keyboard.h"
#include "../Input/Keyboard/KeyboardInputEvent.h"
#include "../Input/Mouse/Mouse.h"
#include "../Input/Mouse/MouseInputEvent.h"
#include "../DataTypes/VectorInt2D.h"



bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	if (!renderWindow.Initialize(this, hInstance, window_title, window_class, width, height))
		return false;

	if (!graphics.Initialize(renderWindow.GetHWND(), width, height))
		return false;

	return true;
}

bool Engine::ProcessMessages()
{
	return renderWindow.ProcessMessages();
}

void Engine::Update()
{
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
}
