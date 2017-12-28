#include "AuthService.h"
#include <windows.h>
#include <iostream>

#include <string>

#include <atlsafe.h>

#import "COMMyCasinoSrv.dll"

#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.c"
#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.h"

#include "BstrStringConverter.h"


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

	hr = p_ICOMMyCasinoSrv->login(wstr_to_bstr(L"Mathias"), wstr_to_bstr(L"Passwort"), &sessionId, &userType, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: Could not log in to server - " << std::hex << hr << std::endl;
	}
	else 
	{
		std::cout << "Success: Could log in to server" << std::endl;
		std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
		std::cout << "Session Id: " << sessionId << std::endl;
		std::cout << "User Type: " << (userType? "gamer":"operator") << std::endl;
	}
	
	hr = p_ICOMMyCasinoSrv->deposit(sessionId, wstr_to_bstr(L"Manuel"), 50.0, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: deposit - " << std::hex << hr << std::endl;
	}
	else
	{
		std::cout << "Success: deposit" << std::endl;
		std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
	}

	hr = p_ICOMMyCasinoSrv->bet(sessionId, 50.0, 1, 1, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: bet - " << std::hex << hr << std::endl;
	}
	else
	{
		std::cout << "Success: bet" << std::endl;
		std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
	}

	DOUBLE profitForOne;
	DOUBLE profitForTwo;
	hr = p_ICOMMyCasinoSrv->calculateProfit(sessionId, 50.0, &profitForOne, &profitForTwo, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: calculateProfit - " << std::hex << hr << std::endl;
	}
	else
	{
		std::cout << "Success: calculateProfit" << std::endl;
		std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
		std::cout << "Profit for one: " << profitForOne << std::endl;
		std::cout << "Profit for two: " << profitForTwo << std::endl;
	}

	SAFEARR_VAR bets;
	ULONG betCount;
	hr = p_ICOMMyCasinoSrv->showbets(sessionId, &bets, &betCount, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: showbets - " << std::hex << hr << std::endl;
	}
	else
	{
		std::cout << "Success: showbets" << std::endl;
		std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
		std::cout << "Bets: " << betCount << std::endl;
		CComSafeArray<VARIANT> betsResult(bets);

		#define BET_DETAILS_PROPTERY_COUNT 3
		for (int i = 0; i < betCount*BET_DETAILS_PROPTERY_COUNT; i++)
		{
			if(i % BET_DETAILS_PROPTERY_COUNT == 0 || i % BET_DETAILS_PROPTERY_COUNT == 1)
				std::cout << betsResult[i].intVal << std::endl;
			else if (i % BET_DETAILS_PROPTERY_COUNT == 2)
				std::cout << betsResult[i].dblVal << std::endl;
		}
	}

	hr = p_ICOMMyCasinoSrv->drawTest(sessionId, 1, 1, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: drawTest - " << std::hex << hr << std::endl;
	}
	else
	{
		std::cout << "Success: drawTest" << std::endl;
		std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
	}

	SHORT firstNumber;
	SHORT secondNumber;
	hr = p_ICOMMyCasinoSrv->draw(sessionId, &firstNumber, &secondNumber, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: draw - " << std::hex << hr << std::endl;
	}
	else
	{
		std::cout << "Success: draw" << std::endl;
		std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
		std::cout << "First number: " << firstNumber << std::endl;
		std::cout << "Second number: " << secondNumber << std::endl;
	}


	BOOL isFinished = FALSE;
	SAFEARR_VAR transaction;
	SAFEARR_VAR transactionInformation;
	ULONG informationType;
	ULONG transactionType;
	ULONG currentTransactionId;
	while (!isFinished)
	{
		
		hr = p_ICOMMyCasinoSrv->getTransactions(sessionId, &isFinished, &transaction, &transactionType, &errMsg);
		if (FAILED(hr))
		{
			std::cout << "Failure: getTransactions - " << std::hex << hr << std::endl;
		}
		else
		{
			std::cout << "Success: getTransactions" << std::endl;
			std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
			CComSafeArray<VARIANT> transactionResult(transaction);
			
			#define TRANSACTION_PROPTERY_COUNT 3
			for (int i = 0; i < TRANSACTION_PROPTERY_COUNT; i++)
			{
				if (i % TRANSACTION_PROPTERY_COUNT == 0)
				{
					currentTransactionId = transactionResult[i].intVal;
					std::cout << currentTransactionId << std::endl;
				}
				else if (i % TRANSACTION_PROPTERY_COUNT == 1 || i % TRANSACTION_PROPTERY_COUNT == 2)
					std::cout << transactionResult[i].dblVal << std::endl;
			}


			if (transactionType == 2) // bet
			{
				hr = p_ICOMMyCasinoSrv->getTransactionInformation(sessionId, currentTransactionId, &transactionInformation, &informationType, &errMsg);
				if (FAILED(hr))
				{
					std::cout << "Failure: getTransactionInformation - " << std::hex << hr << std::endl;
				}
				else
				{
					std::cout << "Success: getTransactionInformation" << std::endl;
					std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;

					CComSafeArray<VARIANT> transactionInformationResult(transactionInformation);

					if (informationType == 2) // = bet
					{
						#define BET_FULL_DETAILS_PROPTERY_COUNT  7
						for (int i = 0; i < BET_FULL_DETAILS_PROPTERY_COUNT; i++)
						{
							if (i % BET_FULL_DETAILS_PROPTERY_COUNT == 0
								|| i % BET_FULL_DETAILS_PROPTERY_COUNT == 1
								|| i % BET_FULL_DETAILS_PROPTERY_COUNT == 4
								|| i % BET_FULL_DETAILS_PROPTERY_COUNT == 5)
								std::cout << transactionInformationResult[i].intVal << std::endl;
							else if (i % BET_FULL_DETAILS_PROPTERY_COUNT == 2
								|| i % BET_FULL_DETAILS_PROPTERY_COUNT == 6)
								std::cout << transactionInformationResult[i].dblVal << std::endl;
							else if (i % BET_FULL_DETAILS_PROPTERY_COUNT == 3)
							{
								BOOL isDrawn = transactionInformationResult[i].boolVal;
								std::cout << isDrawn << std::endl;
								if (!isDrawn)
									break;
							}
						}
					}
				}
			}
		}
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