#include <iostream>
#include <atlsafe.h>

#include "COMMyCasinoCommandLineInterface.h"
#include "BstrStringConverter.h"
#include "MyCasinoDefines.h"


COMMyCasinoCommandLineInterface::COMMyCasinoCommandLineInterface(CmdInterpreter* interpreter, ICOMMyCasino* pICOMMyCasinoSrv)
	: MyCasinoCommandLineInterface(interpreter)
{
	m_pICOMMyCasinoSrv = pICOMMyCasinoSrv;
}

COMMyCasinoCommandLineInterface::~COMMyCasinoCommandLineInterface()
{
	m_pICOMMyCasinoSrv = NULL;
}

bool COMMyCasinoCommandLineInterface::user(std::wstring user, std::wstring password)
{
	if (NULL != m_pSessionId)
	{
		delete m_pSessionId;
	}

	if (NULL != m_pUserType)
	{
		delete m_pUserType;
	}

	m_pSessionId = new unsigned long();
	m_pUserType = new short();

	BSTR errMsg;
	HRESULT hr = m_pICOMMyCasinoSrv->login(wstr_to_bstr(user), wstr_to_bstr(password), m_pSessionId, m_pUserType, &errMsg);
	if (FAILED(hr))
	{
		delete m_pSessionId;
		m_pSessionId = NULL;
		delete m_pUserType;
		m_pUserType = NULL;

		resultHandler("Could not log in to server", hr, bstr_to_str(errMsg));
		return false;
	}

	std::cout << "Logged in, Id: " << (*m_pSessionId) << ", User Type: " << ((*m_pUserType)? "gamer" : "operator") << std::endl;	

	return true;
}

bool COMMyCasinoCommandLineInterface::payin(std::wstring user, double amount)
{
	BSTR errMsg;

	HRESULT hr = m_pICOMMyCasinoSrv->deposit(*m_pSessionId, wstr_to_bstr(user), amount, &errMsg);
	if (FAILED(hr))
	{
		resultHandler("payin", hr, bstr_to_str(errMsg));
		return false;
	}

	return true;
}

bool COMMyCasinoCommandLineInterface::bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber)
{
	BSTR errMsg;
	HRESULT hr = m_pICOMMyCasinoSrv->bet(*m_pSessionId, setAmount, firstNumber, secondNumber, &errMsg);
	resultHandler("bet", hr, bstr_to_str(errMsg));

	return SUCCEEDED(hr);
}

bool COMMyCasinoCommandLineInterface::showbets()
{
	SAFEARR_VAR bets;
	ULONG betCount;
	BSTR errMsg;
	HRESULT hr = m_pICOMMyCasinoSrv->showbets(*m_pSessionId, &bets, &betCount, &errMsg);
	resultHandler("showbets", hr, bstr_to_str(errMsg));
	if (FAILED(hr))
		return false;
	
	if(betCount > 0 )
		std::cout << "User | First number | Second number | Wager | Price for one | Price for two" << std::endl;
	else
		std::cout << "No bets" << std::endl;

	CComSafeArray<VARIANT> betsResult(bets);
	
	BSTR username;
	SHORT currentFirstNumber = 0;
	SHORT currentSecondNumber = 0;
	DOUBLE currentWager = 0.0;
	bool readBetDone = false;
	for (int i = 0; i < betCount*BET_DETAILS_PROPTERY_COUNT; i++)
	{

		if (i % BET_DETAILS_PROPTERY_COUNT == 0)
		{
			username = betsResult[i].bstrVal;
			readBetDone = false;
		}
		else if (i % BET_DETAILS_PROPTERY_COUNT == 1)
			currentFirstNumber = betsResult[i].intVal;
		else if (i % BET_DETAILS_PROPTERY_COUNT == 2)
			currentSecondNumber = betsResult[i].intVal;
		else if (i % BET_DETAILS_PROPTERY_COUNT == 3)
		{
			currentWager = betsResult[i].dblVal;
			readBetDone = true;
		}
		
		if (readBetDone)
		{
			DOUBLE profitForOne;
			DOUBLE profitForTwo;
			hr = m_pICOMMyCasinoSrv->calculateProfit(*m_pSessionId, currentWager, currentFirstNumber, currentSecondNumber, &profitForOne, &profitForTwo, &errMsg);
			if (FAILED(hr))
			{
				resultHandler("calculateProfit", hr, bstr_to_str(errMsg));
				readBetDone = false;
				continue;
			}
			
			std::cout << bstr_to_str(username) << " | " << currentFirstNumber <<  " | " << currentSecondNumber << " | " << currentWager << " | " << profitForOne << " | " << profitForTwo << std::endl;
		}
	}

	return true;
}

bool COMMyCasinoCommandLineInterface::draw(unsigned short* firstNumberTest, unsigned short* secondNumberTest)
{
	BSTR errMsg;
	HRESULT hr;

	// check if only one test number is set is done in ProcessCommands method  
	if (NULL != firstNumberTest && NULL != secondNumberTest)
	{
		//Test
		
		HRESULT hr = m_pICOMMyCasinoSrv->drawTest(*m_pSessionId, *firstNumberTest, *secondNumberTest, &errMsg);
		if (FAILED(hr))
		{
			resultHandler("drawTest", hr, bstr_to_str(errMsg));
			return false;
		}

	}
	else if (NULL == firstNumberTest && NULL == secondNumberTest)
	{
		SHORT firstNumber;
		SHORT secondNumber;
		hr = m_pICOMMyCasinoSrv->draw(*m_pSessionId, &firstNumber, &secondNumber, &errMsg);
		if (FAILED(hr))
		{
			resultHandler("draw", hr, bstr_to_str(errMsg));
			return false;
		}

		std::cout << "First number: " << firstNumber << std::endl;
		std::cout << "Second number: " << secondNumber << std::endl;
	}
	else 
	{
		return false;
	}

	return true;
}

bool COMMyCasinoCommandLineInterface::showstatus()
{
	BOOL isFinished = FALSE;
	SAFEARR_VAR transaction;
	SAFEARR_VAR transactionInformation;
	ULONG informationType;
	ULONG transactionType;
	ULONG currentTransactionId;
	DOUBLE resultBalance;
	DOUBLE changeAmount;
	BSTR errMsg;
	HRESULT hr;

	bool displayHeader = false;
	while (!isFinished)
	{
		if (!displayHeader)
		{
			std::wcout << "Transaction type | Change amount | Resulting balance | <Additional Information>" << std::endl;
			displayHeader = true;
		}

		hr = m_pICOMMyCasinoSrv->getTransactions(*m_pSessionId, &isFinished, &transaction, &transactionType, &errMsg);
		if (FAILED(hr))
		{
			resultHandler("getTransactions", hr, bstr_to_str(errMsg));
			return false;
		}

		CComSafeArray<VARIANT> transactionResult(transaction);
		for (int i = 0; i < TRANSACTION_PROPTERY_COUNT; i++)
		{
			if (i % TRANSACTION_PROPTERY_COUNT == 0)
			{
				currentTransactionId = transactionResult[i].intVal;
			}
			else if (i % TRANSACTION_PROPTERY_COUNT == 1)
				resultBalance = transactionResult[i].dblVal;
			else if (i % TRANSACTION_PROPTERY_COUNT == 2)
				changeAmount = transactionResult[i].dblVal;
		}

		if (transactionType == MyCasinoTransactionsTypes::DEPOSIT
			|| transactionType == MyCasinoTransactionsTypes::WITHDRAWAL)
		{ 
			std::wcout << resolve_transaction_type((MyCasinoTransactionsTypes)transactionType) << " | "  << changeAmount << " | " << resultBalance << std::endl;
		}
		else if (transactionType == MyCasinoTransactionsTypes::BET_WIN
			|| transactionType == MyCasinoTransactionsTypes::BET_LOSS
			)
		{
			hr = m_pICOMMyCasinoSrv->getTransactionInformation(*m_pSessionId, currentTransactionId, &transactionInformation, &informationType, &errMsg);
			if (FAILED(hr))
			{
				resultHandler("getTransactionInformation", hr, bstr_to_str(errMsg));
				return false;
			}
			else
			{
				CComSafeArray<VARIANT> transactionInformationResult(transactionInformation);

				std::wstring transactionInformation(L"");
				BOOL isDrawn = false;
				if (informationType == MyCasinoTransactionsInformationTypes::Bet)
				{

					for (int i = 0; i < BET_FULL_DETAILS_PROPTERY_COUNT; i++)
					{
						if (i % BET_FULL_DETAILS_PROPTERY_COUNT == 1
							|| i % BET_FULL_DETAILS_PROPTERY_COUNT == 2
							|| i % BET_FULL_DETAILS_PROPTERY_COUNT == 4
							|| i % BET_FULL_DETAILS_PROPTERY_COUNT == 5)
							transactionInformation.append(L" ").append(std::to_wstring(transactionInformationResult[i].intVal));
					}
				}

				// only display wager of finished bets
				std::wcout << resolve_transaction_type((MyCasinoTransactionsTypes)transactionType) << " | " << changeAmount << " | " << resultBalance << " | " << transactionInformation <<std::endl;
			}
		}
		
	}

	// print once for displaying information messages
	if(hr > 0)
		resultHandler("getTransactionInformation", hr, bstr_to_str(errMsg));

	return true;
}

bool COMMyCasinoCommandLineInterface::bye()
{
	BSTR errMsg;
	HRESULT hr = m_pICOMMyCasinoSrv->logout(*m_pSessionId, &errMsg);
	resultHandler("bye", hr, bstr_to_str(errMsg));

	return SUCCEEDED(hr);
}