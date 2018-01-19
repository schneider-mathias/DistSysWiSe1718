/**--------------------------------------------------------------------------------------------------
// project:	RPCMyCasinoSrv
// file:	AuthServiceWrapper.h
//
// summary:	Declares the authentication service wrapper class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			28.12.2017	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#pragma once
#include "AuthServiceTemplate.h"
#include "MyCasinoUser.h"

static CAuthServiceTemplate<MyCasinoUser>* authService;

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets authentication service. </summary>
 *
 * <returns>	Null if it fails, else the authentication service. </returns>
 *-----------------------------------------------------------------------------------------------**/

CAuthServiceTemplate<MyCasinoUser>* getAuthService()
{
	if (NULL == authService)
	{
		authService = new CAuthServiceTemplate<MyCasinoUser>(&std::wstring(L"SystemDrive"));
		// batch file copies data files before opening the solution
		authService->readRegisteredUser(L"\\_MyCasinoData\\mycasino_user.txt");
	}

	return authService;
}

/** <summary>	Clean authentication service. </summary> */
void cleanAuthService()
{
	delete authService;
	authService = NULL;
}