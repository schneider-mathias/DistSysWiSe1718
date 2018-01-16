/**--------------------------------------------------------------------------------------------------
// project:	MyCasinoDefines
// file:	MyCasinoResolveCodes.cpp
//
// summary:	Method for resolving MyCasino class based on defines. This method will be 
//			shared (as .DLL) with managed code (.NET) in order to have the same messages in the 
//			WCF client.
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			10.01.2018	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-----------------------------------------------------------------------------------------------**/

#include <Windows.h>
#include "MyCasinoDefines.h"
#include "CharStringConverter.h"

extern "C"
{
	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Resolves a MyCasino HRESULT code and returns a string to callee.
	 * 				Inspired by https://limbioliong.wordpress.com/2011/06/16/returning-strings-from-a-c-api/
	 * 				 </summary>
	 *
	 * <param name="code">	The return code which should be resolved. </param>
	 *
	 * <returns>	Null if it fails, else a pointer to a char. </returns>
	 *-----------------------------------------------------------------------------------------------**/

	__declspec(dllexport) char*  ResolveCode(long code)
	{
		std::wstring msg;
		TRANSLATE_MYCASINO_CODE(msg, code);

		char* szSampleString = wstring_to_char(msg);
		ULONG ulSize = strlen(szSampleString) + sizeof(char);
		char* pszReturn = NULL;

		pszReturn = (char*)::CoTaskMemAlloc(ulSize);
		// Copy the contents of szSampleString
		// to the memory pointed to by pszReturn.
		strcpy(pszReturn, szSampleString);
		delete[] szSampleString;

		return pszReturn;
	}
}