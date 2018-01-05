#pragma once
#include "MyCasino.h"

static MyCasino* myCasino;

MyCasino* getCasino()
{
	if (NULL == myCasino)
	{
		myCasino = new MyCasino(&std::wstring(L"USERDATA"));
		myCasino->LoadAccounts(L"mycasino_user.txt");
	}

	return myCasino;
}

void cleanCasino()
{
	delete myCasino;
	myCasino = NULL;
}