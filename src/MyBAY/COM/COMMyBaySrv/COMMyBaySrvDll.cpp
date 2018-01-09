/************************************************************/
/*                                                          */
/* Inhalt:    CCOMString: DLL-Export                        */
/*                        V0: Klasse in DLL                 */
/*                                                          */
/* Autor(en): Johannes Sauer (js), <XXX>                    */
/* Firma:     Fachhochschule Amberg-Weiden                  */
/* Stand:     24. Dez 2017                                  */
/*                                                          */
/* Historie:  24. Dez 2017 js  erstellt                     */
/*											                */
/*                                                          */
/* Copyright 2001-2050 FH Amberg-Weiden ... usw.            */
/*                                                          */
/************************************************************/

#include "COMMyBaySrvDll.h"
#include "COMMyBay.h"
#include "stdafx.h"


BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
	DWORD     reason,
	LPVOID    dummy)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

HRESULT COMMYBAYSRVDLL_API DllGetClassObject(void **ppv)
{
	*ppv = new COMMyBay;

	return(S_OK);
}
