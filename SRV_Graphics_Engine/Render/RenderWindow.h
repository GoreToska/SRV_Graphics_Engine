#pragma once

#include "../Utils/Logger.h"
#include "../Graphics/Graphics.h"

class WindowContainer;

class RenderWindow
{
public:
	~RenderWindow();

	bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();

	HWND GetHWND() const;

protected:
	void RegisterWindowClass();
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND hWindow = NULL;
	HINSTANCE hInstance = NULL;

	std::string windowTitle = "";
	std::wstring windowTitleW = L"";
	std::string windowClassName = "";
	std::wstring windowClassNameW = L"";

	int width = 800;
	int height = 800;
};

