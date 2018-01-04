#include "AuthService.h"
#include <windows.h>
#include <iostream>

#include <string>

#include <atlsafe.h>

#import "COMMyCasinoSrv.dll"

#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.c"
#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.h"

#include "BstrStringConverter.h"
#include "COMMyCasinoCommandLineInterface.h"
#include "CmdInterpreter.h"

bool runCommand(CmdInterpreter& interpreter, std::wstring command)
{
	std::wcout << command << std::endl;
	if (!interpreter.execute(command))
	{
		interpreter.cout();
		std::wcout << "Error running: " << command << std::endl;
		std::cin.get();
		return false;
	}

	interpreter.cout();
	return true;
}

BOOL testcase_payin_different_accounts(CmdInterpreter& interpreter)
{
	// TEST CASE: payin for different accounts
	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer_2 Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"payin Casino 5000"))
		return E_FAIL;

	if (!runCommand(interpreter, L"payin Gamer 5000"))
		return E_FAIL;

	if (!runCommand(interpreter, L"payin Gamer_2 5000"))
		return E_FAIL;

	return S_OK;
}


BOOL testcase_lose_bets(CmdInterpreter& interpreter)
{
	// TEST CASE: payin for different accounts
	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 50 1 2"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 100 1 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 200 2 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showbets"))
		return E_FAIL;

	if (!runCommand(interpreter, L"draw 4 5"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showbets"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;
	return S_OK;
}

BOOL testcase_win_lose_bets(CmdInterpreter& interpreter)
{
	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 50 1 2"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 100 1 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 200 2 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showbets"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"draw 1 5"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	return S_OK;
}

BOOL testcase_delete_bet(CmdInterpreter& interpreter)
{
	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 200 2 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 0 2 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showbets"))
		return E_FAIL;
}

BOOL testcase_change_bet_more_wager(CmdInterpreter& interpreter)
{
	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 200 2 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 1000 2 4"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 500 2 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showbets"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showbets"))
		return E_FAIL;

	if (!runCommand(interpreter, L"draw 1 5"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;
}


BOOL testcase_change_bet_less_wager(CmdInterpreter& interpreter)
{
	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 500 2 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 1000 2 4"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 200 2 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showbets"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showbets"))
		return E_FAIL;

	if (!runCommand(interpreter, L"draw 1 5"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;
}

int main(int argc, char**argv)
{
	CoInitialize(NULL);
	IClassFactory *pCF = NULL;

	ICOMMyCasino *p_ICOMMyCasinoSrv = NULL;
	HRESULT hr;

	IID IID_tmpICOMMyINVENT = IID_ICOMMyCasino;

	// Erzeugung auf anderem Rechner (auch mit Rechernamen)
	COSERVERINFO srvInfo = { 0, L"127.0.0.1", 0, 0 };
	LPWSTR lpwstr_SrvName = 0;

	if (argc > 1)
	{
		std::wstring wstr_SrvName(argv[1], argv[1] + strlen(argv[1]) + 1);
		lpwstr_SrvName = new WCHAR[wstr_SrvName.length()];
		StrCpyNW(lpwstr_SrvName, wstr_SrvName.c_str(), wstr_SrvName.length());
		std::copy(argv[1], argv[1] + strlen(argv[1]), lpwstr_SrvName);

		srvInfo.pwszName = lpwstr_SrvName;
	}

	MULTI_QI multiQi = { &IID_tmpICOMMyINVENT, 0, 0 };

	hr = CoCreateInstanceEx(CLSID_COMMyCasino, NULL, CLSCTX_REMOTE_SERVER,
		&srvInfo, 1, &multiQi);

	if (FAILED(hr))
	{
		std::cout << "Failure: CoCreateInstanceEx - " << std::hex << hr << std::endl;
		return FALSE;
	}

	p_ICOMMyCasinoSrv = (ICOMMyCasino*)multiQi.pItf;

	std::cout << "--- MY CASINO ---" << std::endl;

	CmdInterpreter interpreter;
	COMMyCasinoCommandLineInterface myCasinoCLI(p_ICOMMyCasinoSrv);
	dispatcherMemFunc p = &(ICommandLineInterface::ProcessCommand);
	interpreter.registerCmdDispatcher(&myCasinoCLI, p);
	interpreter.init();

	// TEST CASE: payin for different accounts
	testcase_payin_different_accounts(interpreter);

	// TEST CASE: lose bets
	testcase_lose_bets(interpreter);

	// TEST CASE: win/lose bet
	testcase_win_lose_bets(interpreter);

	// TEST CASE: delete bet
	testcase_delete_bet(interpreter);

	// TEST CASE: change wager (more afterwards)
	testcase_change_bet_more_wager(interpreter);

	// TEST CASE: change wager (lessafterwards)
	testcase_change_bet_less_wager(interpreter);

	std::cin.get();

	p_ICOMMyCasinoSrv->Release();
	CoUninitialize();


	return S_OK;
}