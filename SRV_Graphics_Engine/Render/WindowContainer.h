#pragma once

#include "RenderWindow.h"
//#include "../Graphics/Graphics.h"

class Graphics;

class WindowContainer
{
public:
	WindowContainer();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	RenderWindow renderWindow;
	Graphics graphics;

};

