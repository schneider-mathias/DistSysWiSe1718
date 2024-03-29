/**---------------------------------------------------------------------
// project:	HelperLib
// file:	CharStringConverter.h
//
// summary:	Inline methods for converting wstring to chars* and the other way around.
//			Adapted version from
// https://stackoverflow.com/questions/6284524/bstr-to-stdstring-stdwstring-and-vice-versa
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			28.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#ifndef CHAR_STRING_CONVERTER_INLCUDED
#define CHAR_STRING_CONVERTER_INLCUDED


#include <Windows.h>
#include <string>

/**---------------------------------------------------------------------
 * <summary>	Character to wstring. </summary>
 *
 * <param name="str2conv">	The char array to convert. </param>
 *
 * <returns>	A std::wstring. </returns>
 *-------------------------------------------------------------------**/

inline std::wstring char_to_wstring(const char* str2conv)
{
	const std::string& str(str2conv);
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

/**---------------------------------------------------------------------
 * <summary>	Wstring to character array. Has to be deleted by callee. </summary>
 *
 * <param name="s">	A std::wstring to convert to char array. </param>
 *
 * <returns>	Null if it fails, else a pointer to a char. </returns>
 *-------------------------------------------------------------------**/

inline char* wstring_to_char(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, buf, len, 0, 0);
	return buf;
}

/**---------------------------------------------------------------------
 * <summary>	Wstring to string.</summary>
 *
 * <param name="s">	A std::wstring to process. </param>
 *
 * <returns>	A std::string. </returns>
 *-------------------------------------------------------------------**/

inline std::string wstring_to_string(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), slength, buf, len, 0, 0);
	std::string retBuffer(buf);
	delete[] buf;
	return retBuffer;
}

#endif // !CHAR_STRING_CONVERTER_INLCUDED
