#pragma once
#include "MyCasino.h"

static MyCasino* myCasino;

MyCasino* getCasino()
{
	if (NULL == myCasino)
	{
		myCasino = new MyCasino(&std::wstring(L"USERPROFILE"));
		myCasino->LoadAccounts(L"\\_MyCasinoData\\mycasino_account.txt");
	}

	return myCasino;
}

void cleanCasino()
{
	delete myCasino;
	myCasino = NULL;
}