/**---------------------------------------------------------------------
// project:	RPCMyCasinoSrv
// file:	MyCasinoWrapper.h
//
// summary:	Declares my casino wrapper class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			28.12.2017	Mathias Schneider	Created
 *-------------------------------------------------------------------**/

#pragma once
#include "MyCasino.h"

static MyCasino* myCasino;

/**---------------------------------------------------------------------
 * <summary>	Gets the casino. </summary>
 *
 * <returns>	Null if it fails, else the casino. </returns>
 *-------------------------------------------------------------------**/

MyCasino* getCasino()
{
	if (NULL == myCasino)
	{
		myCasino = new MyCasino(&std::wstring(L"SystemDrive"));
		// batch file copies data files before opening the solution
		myCasino->LoadAccounts(L"\\_MyCasinoData\\mycasino_account.txt");
	}

	return myCasino;
}

/** <summary>	Clean casino. </summary> */
void cleanCasino()
{
	delete myCasino;
	myCasino = NULL;
}