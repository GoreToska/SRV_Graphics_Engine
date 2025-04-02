#pragma once

#pragma once

#include <comdef.h>

#include "../Utils/StringConverter.h"

#define ThrowIfFailed(hr, msg) if(FAILED(hr)) throw SRVException(hr, msg, __FILE__, __FUNCTION__, __LINE__)

class SRVException
{
public:
	SRVException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line)
	{
		_com_error error(hr);
		errorMsg = L"Message: " + StringConverter::ToWide(std::string(msg)) + L"\n";
		errorMsg += error.ErrorMessage();
		errorMsg += L"\nFile: " + StringConverter::ToWide(std::string(file));
		errorMsg += L"\Function: " + StringConverter::ToWide(std::string(function));
		errorMsg += L"\Line: " + StringConverter::ToWide(std::to_string(line));
	}

	const wchar_t* ErrorMsg() const
	{
		return errorMsg.c_str();
	}

private:
	std::wstring errorMsg;
};