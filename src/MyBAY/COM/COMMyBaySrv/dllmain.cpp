// dllmain.cpp : Implementierung von DllMain.

#include "stdafx.h"
#include "resource.h"
#include "COMMyBaySrv_i.h"
#include "dllmain.h"

CCOMMyBaySrvModule _AtlModule;

// DLL-Einstiegspunkt
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
