#pragma once
#include <Windows.h>
#include <string>


//https://codereview.stackexchange.com/questions/419/converting-between-stdwstring-and-stdstring

std::wstring char_to_wstring(const char* str2conv)
{
	const std::string& str(str2conv);
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

char* wstring_to_char(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, buf, len, 0, 0);
	return buf;
}