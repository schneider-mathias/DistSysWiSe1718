/************************************************************/
/*                                                          */
/* Inhalt:    CCOMString: DLL-Export-Schnittstellendatei    */
/*                        V0: Klasse in DLL                 */
/*                                                          */
/* Autor(en): Josef Pösl (jp), <XXX>                        */
/* Firma:     Fachhochschule Amberg-Weiden                  */
/* Stand:     18. Sep 2002                                  */
/*                                                          */
/* Historie:  18. Sep 2002 jp  erstellt                     */
/*            xx. xxx xxxx     modifiziert...               */
/*                                                          */
/* Copyright 2001-2050 FH Amberg-Weiden ... usw.            */
/*                                                          */
/************************************************************/

#ifndef COMMYBAY_SRV_DLL_H
#define COMMYBAY_SRV_DLL_H

#include <windows.h>


#ifdef COMMYBAYSRVDLL_EXPORTS
#define COMMYBAYSRVDLL_API __declspec(dllexport)
#else
#define COMMYBAYSRVDLL_API __declspec(dllimport)
#endif


HRESULT COMMYBAYSRVDLL_API DllGetClassObject(void **ppv);

#endif   
