#pragma once
#include <Windows.h>
#include <string>

std::wstring char_to_wstring(char* str2conv)
{
	const std::string& str(str2conv);
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}