#include "AuthService.h"
#include <windows.h>
#include <iostream>

#include <string>

#include <atlsafe.h>

#import "COMMyCasinoSrv.dll"

#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.c"
#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.h"

BOOL AuthTest()
{
	CAuthService* AuthService = new CAuthService(&std::wstring(L"USERDATA"));
	// Important: Set project property -> Debugging -> Working directory -> $(TargetDir)
	// in order to use relative paths during debugging!
	if (!AuthService->readRegisteredUser(L"test.txt"))
	{
		std::cout << "Cannot read Test.txt" << std::endl;
		return FALSE;
	}

	ULONG mathiasSessionId;
	if (!AuthService->login(L"Mathias", L"Passwort", &mathiasSessionId))
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


int main(int argc, char**argv)
{
	// UTs
	if (!AuthTest())
	{
		std::cout << "Error in AuthService test. Aborting." << std::endl;
		return FALSE;
	}



	CoInitialize(NULL);
	IClassFactory *pCF = NULL;
	
	ICOMMyCasino *p_ICOMMyCasinoSrv = NULL;
	HRESULT hr;

	IID IID_tmpICOMMyINVENT = IID_ICOMMyCasino;

	// Erzeugung auf anderem Rechner (auch mit Rechernamen)
	COSERVERINFO srvInfo = { 0, L"127.0.0.1", 0, 0 };
	LPWSTR lpwstr_SrvName = 0;
	/*
	if (argc > 1)
	{
		std::wstring wstr_SrvName(argv[1], argv[1] + strlen(argv[1]) + 1);
		lpwstr_SrvName = new WCHAR[wstr_SrvName.length()];
		std::StrCpyNW(lpwstr_SrvName, wstr_SrvName.c_str(), wstr_SrvName.length());
		std::copy(argv[1], argv[1] + strlen(argv[1]), lpwstr_SrvName);

		srvInfo.pwszName = lpwstr_SrvName;
	}
	*/
	MULTI_QI multiQi = { &IID_tmpICOMMyINVENT, 0, 0 };

	hr = CoCreateInstanceEx(CLSID_COMMyCasino, NULL, CLSCTX_REMOTE_SERVER,
		&srvInfo, 1, &multiQi);

	if (FAILED(hr))
	{
		std::cout << "Failure: CoCreateInstanceEx - " << std::hex << hr << std::endl;
		return FALSE;
	}

	p_ICOMMyCasinoSrv = (ICOMMyCasino*)multiQi.pItf;

	std::cout << "--------------------------------------------------------" << std::endl;


	ULONG sessionId;
	SHORT userType;
	BSTR errMsg;

	hr = p_ICOMMyCasinoSrv->login(CComBSTR(L"Mathias").Detach(), CComBSTR(L"Passwort").Detach(), &sessionId, &userType, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: Could not log in to server - " << std::hex << hr << std::endl;
	}
	else 
	{
		std::cout << "Success: Could log in to server" << std::endl;
		std::cout << "Session Id: " << sessionId << std::endl;
		std::cout << "User Type: " << (userType? "gamer":"operator") << std::endl;
	}
	
	hr = p_ICOMMyCasinoSrv->logout(sessionId, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: Could not log out to server - " << std::hex << hr << std::endl;
	}
	else
	{
		std::cout << "Success: Could log out from server" << std::endl;
	}

	//console(p_ICOMMyINVENTSvr); //Konsoleneingabe in dauerschleife auslesen

	p_ICOMMyCasinoSrv->Release();
	CoUninitialize();


	return S_OK;
}