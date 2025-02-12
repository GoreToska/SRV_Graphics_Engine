#include "Logger.h"
#include <comdef.h>

void Logger::LogError(std::string message)
{
	std::string error_message = "Error: " + message;
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}

void Logger::LogError(HRESULT result, std::string message)
{
	_com_error error(result);
	std::wstring wstr(message.begin(), message.end());
	std::wstring error_message = L"Error: " + wstr + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void Logger::LogError(HRESULT result, std::wstring message)
{
	_com_error error(result);
	std::wstring error_message = L"Error: " + message + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}
