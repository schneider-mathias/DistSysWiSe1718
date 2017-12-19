#include "AuthService.h"
#include <windows.h>
#include <iostream>


int main(int argc, char**argv)
{
	CAuthService* AuthService = new CAuthService();
	// Important: Set project property -> Debugging -> Working directory -> $(TargetDir)
	// in order to use relative paths during debugging!
	if (!AuthService->readRegisteredUser(L".\\..\\_data\\test.txt"))
	{
		std::cout << "Cannot read Test.txt" << std::endl;
		return FALSE;
	}

	ULONG mathiasSessionId;
	if (!AuthService->login(L"Mathias", L"Passwort",&mathiasSessionId))
	{
		std::cout << "First login failed" << std::endl;
		return FALSE;
	}

	if (!AuthService->login(L"Mathias", L"Passwort", &mathiasSessionId))
	{
		std::cout << "Second login failed" << std::endl;
		return FALSE;
	}


	if (!AuthService->isLoggedIn(mathiasSessionId))
	{
		std::cout << "First user should be logged in" << std::endl;
		return FALSE;
	}


	if (AuthService->login(L"Mathias", L"InvalidPasswort", &mathiasSessionId))
	{
		std::cout << "Login worked with wrong password" << std::endl;
		return FALSE;
	}

	ULONG manuelSessionId;
	if (!AuthService->login(L"Manuel", L"Passwort", &manuelSessionId))
	{
		std::cout << "Second user login failed" << std::endl;
		return FALSE;
	}

	if (!AuthService->logout(manuelSessionId))
	{
		std::cout << "Second user logout failed" << std::endl;
		return FALSE;
	}

	if (AuthService->logout(manuelSessionId))
	{
		std::cout << "Second user is logged out but could log out" << std::endl;
		return FALSE;
	}


	delete AuthService;

	return TRUE;
}