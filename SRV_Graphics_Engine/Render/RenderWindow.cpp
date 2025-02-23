#include "RenderWindow.h"
#include "WindowContainer.h"
#include "../Utils/StringConverter.h"


RenderWindow::~RenderWindow()
{
	if (hWindow != NULL)
	{
		UnregisterClass(windowClassNameW.c_str(), hInstance);
		DestroyWindow(hWindow);
	}
}

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->windowTitle = window_title;
	this->windowTitleW = StringConverter::ToWide(window_title);
	this->windowClassName = window_class;
	this->windowClassNameW = StringConverter::ToWide(window_class);

	RegisterWindowClass();

	int centerX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int centerY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	RECT wr;
	wr.left = centerX;
	wr.top = centerY;
	wr.right = wr.left + width;
	wr.bottom = wr.top + height;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	hWindow = CreateWindowEx(0,
		windowClassNameW.c_str(),
		windowTitleW.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		wr.left, // window X position
		wr.top, // window Y position
		wr.right - wr.left, // width
		wr.bottom - wr.top, // height
		NULL,
		NULL,
		hInstance,
		pWindowContainer);

	if (hWindow == NULL)
	{
		Logger::LogError(GetLastError(), "CreateWindowEX failed for window " + window_title);
		return false;
	}

	ShowWindow(hWindow, SW_SHOW);
	SetForegroundWindow(hWindow);
	SetFocus(hWindow);

	return true;
}

bool RenderWindow::ProcessMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, hWindow, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(hWindow))
		{
			hWindow = NULL;
			UnregisterClass(windowClassNameW.c_str(), hInstance);

			return false;
		}
	}

	return true;
}

HWND RenderWindow::GetHWND() const
{
	return hWindow;
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // flags for redraw on size changed
	wc.lpfnWndProc = HandleMsgSetup; // ptr to window proc func for handling messages from this window
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClassNameW.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);
}

LRESULT CALLBACK RenderWindow::HandleMsgSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);

		if (pWindow == nullptr)
		{
			Logger::LogError("Critical error: pointer to window container is null during WM_NCCREATE.");
			exit(-1);
		}

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));

		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

LRESULT RenderWindow::HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	default:
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
}
