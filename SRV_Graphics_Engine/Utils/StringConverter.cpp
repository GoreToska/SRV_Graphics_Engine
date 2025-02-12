#include "StringConverter.h"

std::wstring StringConverter::ToWide(std::string value)
{
	return std::wstring(value.begin(), value.end());
}
