#include <Windows.h>
#include "MyCasinoDefines.h"
#include "CharStringConverter.h"

//https://limbioliong.wordpress.com/2011/06/16/returning-strings-from-a-c-api/
extern "C"
{
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