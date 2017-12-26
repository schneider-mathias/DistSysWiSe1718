/********************************************************
* BstrStringConverter.h -- 
	Functions for BSTR <-> std::string conversions     *
*													   *
*                                                      *
* Authors:  Mathias, Schneider						   *
*                                                      *
* Purpose:  Implementation for converting BSTR 
			to std::strings and the other way around
*                                                      *
* Usage:                                               *
*		Use this header to use inline functions.
		Required comsuppw.lib is linked implicitly.
		Adapted version from 
		https://stackoverflow.com/questions/6284524/bstr-to-stdstring-stdwstring-and-vice-versa *
********************************************************/

#ifndef __BSTR_STRING_CONVERSION__ 
#define __BSTR_STRING_CONVERSION__


#include <string>
#include <cassert>
#include <comutil.h>

// link necessary comsuppw.lib library when using this header
#pragma comment(lib,"comsuppw.lib")
inline std::string bstr_to_str(BSTR str)
{
	USES_CONVERSION;
	assert(str != nullptr);
	return std::string(W2A(str));
}

inline std::wstring bstr_to_wstr(BSTR str)
{
	USES_CONVERSION;
	assert(str != nullptr);
	return std::wstring(str);
}

inline BSTR wstr_to_bstr(std::wstring str)
{
	USES_CONVERSION;
	assert(!str.empty());
	return CComBSTR(str.c_str()).Detach();
}

#endif // !__BSTR_STRING_CONVERSION__ 