#pragma once
#include "MyCasino.h"

static MyCasino* myCasino;

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets the casino. </summary>
 *
 * <returns>	Null if it fails, else the casino. </returns>
 *-----------------------------------------------------------------------------------------------**/

MyCasino* getCasino()
{
	if (NULL == myCasino)
	{
		myCasino = new MyCasino(&std::wstring(L"SystemDrive"));
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