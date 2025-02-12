#include "WindowContainer.h"
#include "../Input/Mouse/Mouse.h"
#include "../Input/Keyboard/Keyboard.h"
#include <memory>

WindowContainer::WindowContainer()
{
	static bool raw_input_initialized = false;

	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01; // mouse
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = 0;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			Logger::LogError(GetLastError(), "Failed to register raw input devices.");
			exit(-1);
		}

		raw_input_initialized = true;
	}
}

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
#pragma region KeyboardMessages
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);

		if (Keyboard::GetInstance().IsCharsAutoRepeat())
		{
			Keyboard::GetInstance().OnKeyPressed(keycode);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;

			if (!wasPressed)
			{
				Keyboard::GetInstance().OnKeyPressed(keycode);
			}
		}

		return 0;
	}

	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		Keyboard::GetInstance().OnKeyReleased(keycode);

		return 0;
	}

	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);

		if (Keyboard::GetInstance().IsCharsAutoRepeat())
		{
			Keyboard::GetInstance().OnChar(ch);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;

			if (!wasPressed)
			{
				Keyboard::GetInstance().OnChar(ch);
			}
		}

		return 0;
	}
#pragma endregion

#pragma region MouseMessages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance().OnMouseMove(x, y);

		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance().OnLeftPressed(x, y);

		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance().OnLeftReleased(x, y);

		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance().OnRightPressed(x, y);

		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance().OnRightReleased(x, y);

		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance().OnMiddlePressed(x, y);

		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance().OnMiddleReleased(x, y);

		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			Mouse::GetInstance().OnWheelUp(x, y);
		else
			Mouse::GetInstance().OnWheelDown(x, y);

		return 0;
	}
	case WM_INPUT:
	{
		// TODO: mouse raw input returns not raw input, it returns actual position of cursor

		UINT dataSize{};
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first

		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);

			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());

				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					Mouse::GetInstance().OnMouseDelta(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam); //Need to call DefWindowProc for WM_INPUT messages
	}
#pragma endregion

	default:
		break;
	}
	return  DefWindowProc(hwnd, uMsg, wParam, lParam);
}
