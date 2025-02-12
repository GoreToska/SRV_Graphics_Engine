#pragma once
#include <string>
#include <Windows.h>


class Logger
{
public:
	static void LogError(std::string message);
	static void LogError(HRESULT result, std::string message);
	static void LogError(HRESULT result, std::wstring message);
};

