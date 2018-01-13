/**--------------------------------------------------------------------------------------------------
// project:	MyCasinoDefines
// file:	MyCasinoResolveCodes.cpp
//
// summary:	Implements my casino resolve codes class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#include <Windows.h>
#include "MyCasinoDefines.h"
#include "CharStringConverter.h"

//https://limbioliong.wordpress.com/2011/06/16/returning-strings-from-a-c-api/
extern "C"
{
	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Resolve code. </summary>
	 *
	 * <param name="code">	The code. </param>
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
		// Return pszReturn.
		return pszReturn;
	}
}