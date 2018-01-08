#pragma once
#include "AuthServiceTemplate.h"
#include "MyCasinoUser.h"

static CAuthServiceTemplate<MyCasinoUser>* authService;

CAuthServiceTemplate<MyCasinoUser>* getAuthService()
{
	if (NULL == authService)
	{
		authService = new CAuthServiceTemplate<MyCasinoUser>(&std::wstring(L"SystemDrive"));
		authService->readRegisteredUser(L"\\_MyCasinoData\\mycasino_user.txt");
	}

	return authService;
}

void cleanAuthService()
{
	delete authService;
	authService = NULL;
}