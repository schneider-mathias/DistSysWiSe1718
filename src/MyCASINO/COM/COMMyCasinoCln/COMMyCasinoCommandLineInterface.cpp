#include <iostream>
#include <atlsafe.h>

#include "COMMyCasinoCommandLineInterface.h"
#include "BstrStringConverter.h"

COMMyCasinoCommandLineInterface::COMMyCasinoCommandLineInterface(ICOMMyCasino* pICOMMyCasinoSrv)
{
	m_pICOMMyCasinoSrv = pICOMMyCasinoSrv;
	m_pSessionId = NULL;
	m_pUserType = NULL;
}

COMMyCasinoCommandLineInterface::~COMMyCasinoCommandLineInterface()
{
	if(NULL != m_pSessionId)
		delete m_pSessionId;
	if (NULL != m_pUserType)
		delete m_pUserType;
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

		std::cout << "Failure: Could not log in to server - " << std::hex << hr << std::endl;
		return false;
	}

	std::cout << "Success: Could log in to server" << std::endl;
	std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
	std::cout << "Session Id: " << (*m_pSessionId) << std::endl;
	std::cout << "User Type: " << ((*m_pUserType)? "gamer" : "operator") << std::endl;	

	return true;
}

bool COMMyCasinoCommandLineInterface::payin(std::wstring user, double amount)
{
	BSTR errMsg;

	HRESULT hr = m_pICOMMyCasinoSrv->deposit(*m_pSessionId, wstr_to_bstr(user), amount, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: deposit - " << std::hex << hr << std::endl;
		return false;
	}

	std::cout << "Success: deposit" << std::endl;
	std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
	return true;
}

bool COMMyCasinoCommandLineInterface::bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber)
{
	BSTR errMsg;
	HRESULT hr = m_pICOMMyCasinoSrv->bet(*m_pSessionId, setAmount, firstNumber, secondNumber, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: bet - " << std::hex << hr << std::endl;
		return false;
	}
	
	std::cout << "Success: bet" << std::endl;
	std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
	
	return true;
}

bool COMMyCasinoCommandLineInterface::showbets()
{
	SAFEARR_VAR bets;
	ULONG betCount;
	BSTR errMsg;
	HRESULT hr = m_pICOMMyCasinoSrv->showbets(*m_pSessionId, &bets, &betCount, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: showbets - " << std::hex << hr << std::endl;
		return false;
	}
	
	std::cout << "Success: showbets" << std::endl;
	std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
	std::cout << "Bets: " << betCount << std::endl;
	CComSafeArray<VARIANT> betsResult(bets);

#define BET_DETAILS_PROPTERY_COUNT 3
	
	SHORT* currentFirstNumber = NULL;
	SHORT* currentSecondNumber = NULL;
	for (int i = 0; i < betCount*BET_DETAILS_PROPTERY_COUNT; i++)
	{
		
		if (i % BET_DETAILS_PROPTERY_COUNT == 0)
			*currentFirstNumber = betsResult[i].intVal;
		else if (i % BET_DETAILS_PROPTERY_COUNT == 1)
			*currentSecondNumber = betsResult[i].intVal;
		else if (i % BET_DETAILS_PROPTERY_COUNT == 2)
			std::cout << betsResult[i].dblVal << std::endl;
	
	
		if (currentFirstNumber && currentSecondNumber)
		{
			DOUBLE profitForOne;
			DOUBLE profitForTwo;
			hr = m_pICOMMyCasinoSrv->calculateProfit(*m_pSessionId, 50.0, &profitForOne, &profitForTwo, &errMsg);
			if (FAILED(hr))
			{
				std::cout << "Failure: calculateProfit - " << std::hex << hr << std::endl;
				currentFirstNumber = NULL;
				currentSecondNumber = NULL;
				continue;
			}
			
			std::cout << "Success: calculateProfit" << std::endl;
			std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
			std::cout << "First number: " << *currentFirstNumber << std::endl;
			std::cout << "Second number: " << *currentSecondNumber << std::endl;
			std::cout << "Profit for one: " << profitForOne << std::endl;
			std::cout << "Profit for two: " << profitForTwo << std::endl;
			
			currentFirstNumber = NULL;
			currentSecondNumber = NULL;
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
		
		HRESULT hr = m_pICOMMyCasinoSrv->drawTest(*m_pSessionId, *firstNumberTest, 1, &errMsg);
		if (FAILED(hr))
		{
			std::cout << "Failure: drawTest - " << std::hex << hr << std::endl;
		}
		else
		{
			std::cout << "Success: drawTest" << std::endl;
			std::cout << "Message: " << bstr_to_str(errMsg) << std::endl;
		}

	}
	else if (NULL == firstNumberTest && NULL == secondNumberTest)
	{
		SHORT firstNumber;
		SHORT secondNumber;
		hr = m_pICOMMyCasinoSrv->draw(*m_pSessionId, &firstNumber, &secondNumber, &errMsg);
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

	}
	else 
	{
		return false;
	}
}

bool COMMyCasinoCommandLineInterface::showstatus()
{
	BOOL isFinished = FALSE;
	SAFEARR_VAR transaction;
	SAFEARR_VAR transactionInformation;
	ULONG informationType;
	ULONG transactionType;
	ULONG currentTransactionId;
	BSTR errMsg;

	while (!isFinished)
	{

		HRESULT hr = m_pICOMMyCasinoSrv->getTransactions(*m_pSessionId, &isFinished, &transaction, &transactionType, &errMsg);
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
				hr = m_pICOMMyCasinoSrv->getTransactionInformation(*m_pSessionId, currentTransactionId, &transactionInformation, &informationType, &errMsg);
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

	return true;
}

bool COMMyCasinoCommandLineInterface::bye()
{
	BSTR errMsg;
	HRESULT hr = m_pICOMMyCasinoSrv->logout(*m_pSessionId, &errMsg);
	if (FAILED(hr))
	{
		std::cout << "Failure: Could not log out to server - " << std::hex << hr << std::endl;
		return false;
	}

	std::cout << "Success: Could log out from server" << std::endl;
	return true;
}