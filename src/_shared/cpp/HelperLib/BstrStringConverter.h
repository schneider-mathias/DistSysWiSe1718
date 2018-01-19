/**---------------------------------------------------------------------
// project:
// file:	BstrStringConverter.h
//
// summary:	Inline methods for converting BSTR to std::strings and the other way around.
//			Requires comsuppw.lib is linked implicitly. Adapted version from
// https://stackoverflow.com/questions/6284524/bstr-to-stdstring-stdwstring-and-vice-versa
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			19.12.2017	Mathias Schneider	Created
//			XXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#ifndef __BSTR_STRING_CONVERSION__ 
#define __BSTR_STRING_CONVERSION__

#include <string>
#include <cassert>
#include <comutil.h>

// link necessary comsuppw.lib library when using this header
#pragma comment(lib,"comsuppw.lib")

/**---------------------------------------------------------------------
 * <summary>	Bstr to string. </summary>
 *
 * <param name="str">	The string. </param>
 *
 * <returns>	A std::string. </returns>
 *-------------------------------------------------------------------**/

inline std::string bstr_to_str(BSTR str)
{
	USES_CONVERSION;
	if (NULL == str)
		return std::string("");
	return std::string(W2A(str));
}

/**---------------------------------------------------------------------
 * <summary>	Converts a str to a bstr. </summary>
 *
 * <param name="str">	The string. </param>
 *
 * <returns>	Str as a BSTR. </returns>
 *-------------------------------------------------------------------**/

inline BSTR str_to_bstr(std::string str) 
{
	USES_CONVERSION;
	return CComBSTR(str.c_str()).Detach();
}

/**---------------------------------------------------------------------
 * <summary>	Bstr to wstr. </summary>
 *
 * <param name="str">	The string. </param>
 *
 * <returns>	A std::wstring. </returns>
 *-------------------------------------------------------------------**/

inline std::wstring bstr_to_wstr(BSTR str)
{
	USES_CONVERSION;
	if (NULL == str)
		return std::wstring(L"");
	return std::wstring(str);
}

/**---------------------------------------------------------------------
 * <summary>	Wstr to bstr. </summary>
 *
 * <param name="str">	The string. </param>
 *
 * <returns>	A BSTR. </returns>
 *-------------------------------------------------------------------**/

inline BSTR wstr_to_bstr(std::wstring str)
{
	USES_CONVERSION;
	return CComBSTR(str.c_str()).Detach();
}

#endif // !__BSTR_STRING_CONVERSION__ 