/**---------------------------------------------------------------------
// project:	COMMyCasinoTestCln
// file:	COMMyCasinoTestCln.cpp
//
// summary:	Implements the com my casino test cln class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-------------------------------------------------------------------**/

#include <windows.h>
#include <iostream>

#include <string>

#include <atlsafe.h>

#import "COMMyCasinoSrv.dll"

#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.c"
#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.h"

#include "HelperLib\BstrStringConverter.h"
#include "COMMyCasinoCommandLineInterface.h"
#include "CmdInterpreterLib\CmdInterpreter.h"

/**---------------------------------------------------------------------
 * <summary>	Executes the command operation. </summary>
 *
 * <param name="interpreter">	[in,out] The interpreter. </param>
 * <param name="command">	 	The command. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool runCommand(CmdInterpreter& interpreter, std::wstring command)
{
	std::wcout << command << std::endl;
	if (!interpreter.execute(command))
	{
		interpreter.cout();
		std::wcout << "Error running: " << command << std::endl;
		return false;
	}

	interpreter.cout();
	return true;
}

/**---------------------------------------------------------------------
 * <summary>	Testcase payin different accounts. </summary>
 *
 * <param name="interpreter">	[in,out] The interpreter. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

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

	if (!runCommand(interpreter, L"bye"))
		return E_FAIL;

	return S_OK;
}

/**---------------------------------------------------------------------
 * <summary>	Testcase window two numbers bets. </summary>
 *
 * <param name="interpreter">	[in,out] The interpreter. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

BOOL testcase_win_two_numbers_bets(CmdInterpreter& interpreter)
{
	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 100 1 2"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showbets"))
		return E_FAIL;

	if (!runCommand(interpreter, L"draw 1 2"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	return S_OK;
}

/**---------------------------------------------------------------------
 * <summary>	Testcase lose bets. </summary>
 *
 * <param name="interpreter">	[in,out] The interpreter. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

BOOL testcase_lose_bets(CmdInterpreter& interpreter)
{
	// TEST CASE: payin for different accounts
	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;
	
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

/**---------------------------------------------------------------------
 * <summary>	Testcase window lose bets. </summary>
 *
 * <param name="interpreter">	[in,out] The interpreter. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

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

/**---------------------------------------------------------------------
 * <summary>	Testcase delete bet. </summary>
 *
 * <param name="interpreter">	[in,out] The interpreter. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

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

	return S_OK;
}

/**---------------------------------------------------------------------
 * <summary>	Testcase change bet more wager. </summary>
 *
 * <param name="interpreter">	[in,out] The interpreter. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

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

	return S_OK;
}

/**---------------------------------------------------------------------
 * <summary>	Testcase change bet less wager. </summary>
 *
 * <param name="interpreter">	[in,out] The interpreter. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

BOOL testcase_change_bet_less_wager(CmdInterpreter& interpreter)
{
	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

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

	return S_OK;
}

/**---------------------------------------------------------------------
 * <summary>	Testcase close casino. </summary>
 *
 * <param name="interpreter">	[in,out] The interpreter. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

BOOL testcase_close_casino(CmdInterpreter& interpreter)
{
	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

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

	if (!runCommand(interpreter, L"bye"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showbets"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	return S_OK;
}

/**---------------------------------------------------------------------
* <summary>	Testcase check transaction order. </summary>
*
* <param name="interpreter">	[in,out] The interpreter. </param>
*
* <returns>	True if it succeeds, false if it fails. </returns>
*-------------------------------------------------------------------**/

BOOL testcase_transaction_order_casino(CmdInterpreter& interpreter)
{
	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer_2 Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"payin Gamer_2 5000"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer_2 Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 1 2 4"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 1 1 3"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 1 3 4"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 1 4 5"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer_2 Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 2 1 4"))
		return E_FAIL;

	if (!runCommand(interpreter, L"bet 3 2 5"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Casino Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"draw 2 4"))
		return E_FAIL;

	if (!runCommand(interpreter, L"payin Casino 5000"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	if (!runCommand(interpreter, L"user Gamer_2 Passwort"))
		return E_FAIL;

	if (!runCommand(interpreter, L"showstatus"))
		return E_FAIL;

	return S_OK;
}

/**---------------------------------------------------------------------
 * <summary>	Main entry-point for this application. </summary>
 *
 * <param name="argc">	The number of command-line arguments provided. </param>
 * <param name="argv">	An array of command-line argument strings. </param>
 *
 * <returns>	Exit-code for the process - 0 for success, else an error code. </returns>
 *-------------------------------------------------------------------**/

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
	COMMyCasinoCommandLineInterface myCasinoCLI(&interpreter, p_ICOMMyCasinoSrv);
	dispatcherMemFunc p = &(ICommandLineInterface::ProcessCommand);
	interpreter.registerCmdDispatcher(&myCasinoCLI, p);
	interpreter.init();

	
	BOOL resVal = testcase_lose_bets(interpreter);
	if (!FAILED(resVal))
	{
		std::cout << "Error running test - testcase_lose_bets" << std::endl;
		return E_FAIL;
	}
		
	// TEST CASE: win with both numbers
	testcase_win_two_numbers_bets(interpreter);
	
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

	// TEST CASE: close casino (logout operator)
	testcase_close_casino(interpreter);
	
	// TEST CASE: check showstatus transaction order
	testcase_transaction_order_casino(interpreter);

	std::cin.get();

	p_ICOMMyCasinoSrv->Release();
	CoUninitialize();

	if (NULL != lpwstr_SrvName)
		delete[] lpwstr_SrvName;


	return S_OK;
}