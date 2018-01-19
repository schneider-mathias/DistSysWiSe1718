/**--------------------------------------------------------------------------------------------------
// project:	COMMyCasinoSrv
// file:	dllmain.cpp
//
// summary:	[Created by Visual Studio]. Implementation of DllMain.
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			19.12.2017	Mathias Schneider	Created
// *-----------------------------------------------------------------------------------------------**/


#include "stdafx.h"
#include "resource.h"
#include "COMMyCasinoSrv_i.h"
#include "dllmain.h"

CCOMMyCasinoSrvModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
