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
#include <comutil.h>

// link necessary comsuppw.lib library when using this header
#pragma comment(lib,"comsuppw.lib")

inline std::wstring bstr_to_wstr(BSTR bs) {
	// given BSTR bs
	assert(bs != nullptr);
	std::wstring ws(bs, SysStringLen(bs));
	assert(0 == wcscmp(ws.c_str(), bs));
	return ws;
}

inline std::wstring wstr_to_bstr(std::wstring ws) {
	assert(!ws.empty());
	BSTR bs = SysAllocStringLen(ws.data(), ws.size());
	assert(0 == wcscmp(ws.c_str(), bs));
	return bs;
}

#endif // !__BSTR_STRING_CONVERSION__ 