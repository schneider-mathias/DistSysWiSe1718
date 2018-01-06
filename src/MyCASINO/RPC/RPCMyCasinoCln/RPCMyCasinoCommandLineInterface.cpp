#include "RPCMyCasinoCommandLineInterface.h"
#include "MyCasino_i.h"
#include "CharStringConverter.h"
#include "MyCasinoDefines.h"

RPCMyCasinoCommandLineInterface::RPCMyCasinoCommandLineInterface(CmdInterpreter* interpreter)
	:MyCasinoCommandLineInterface(interpreter)
{
	m_pSessionId = NULL;
	m_pUserType = NULL;
}

RPCMyCasinoCommandLineInterface::~RPCMyCasinoCommandLineInterface()
{
}

bool RPCMyCasinoCommandLineInterface::user(std::wstring user, std::wstring password)
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

	unsigned char* userCStr = (unsigned char*)wstring_to_char(user);
	unsigned char* passwordCStr = (unsigned char*)wstring_to_char(password);

	error_status_t hr = ::login(m_pSessionId, userCStr, passwordCStr, m_pUserType);
	if (hr == RPC_S_OK)
	{
		std::cout << "Logged in as " << (*m_pUserType ? "gamer" : "operator") << std::endl;
	}
	else if (hr == RPC_E_ACCESS_DENIED)
		std::cerr << "Passwort oder Benutzername falsch!" << std::endl;
	else if (hr == RPC_E_FAULT)
		std::cerr << "Sie sind bereits eingeloggt" << std::endl;

	delete userCStr;
	delete passwordCStr;

	return hr ? false : true;
}

bool RPCMyCasinoCommandLineInterface::payin(std::wstring user, double amount)
{
	unsigned char* userCStr = (unsigned char*)wstring_to_char(user);

	error_status_t hr = ::deposit(*m_pSessionId, userCStr, amount);
	if (hr == RPC_S_OK)
	{
		std::cout << "Success: deposit" << std::endl;
	}

	delete userCStr;

	return hr ? false : true;
}

bool RPCMyCasinoCommandLineInterface::bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber)
{
	error_status_t hr = ::bet(*m_pSessionId, setAmount, (short)firstNumber, (short)secondNumber);
	if (hr == RPC_S_OK)
	{
		std::cout << "Success: bet" << std::endl;
	}

	return hr ? false : true;
}

bool RPCMyCasinoCommandLineInterface::showbets()
{
	MyCasinoBet_t* bets = NULL;
	unsigned long count;
	DOUBLE profitForOne;
	DOUBLE profitForTwo;

	error_status_t hr = ::showbets(*m_pSessionId, &bets, &count);
	if (FAILED(hr))
	{
		return false;
	}

	if (count > 0)
		std::cout << "User | First number | Second number | Wager | Price for one | Price for two" << std::endl;
	else
		std::cout << "No bets" << std::endl;

	for (int i = 0; i < count; i++)
	{
		hr = ::calculateProfit(*m_pSessionId, bets[i].wager, bets[i].firstNumber, bets[i].secondNumber, &profitForOne, &profitForTwo);
		if (FAILED(hr))
		{
			continue;
		}

		std::cout << bets[i].name.str << " | " <<  bets[i].firstNumber << " | " << bets[i].secondNumber << " | " << bets[i].wager << " | " << profitForOne << " | " << profitForTwo << std::endl;
	}

	// free memory
	for (int i = 0; i < count; i++)
		MIDL_user_free(bets[i].name.str);
	MIDL_user_free(bets);

	return hr ? false : true;
}

bool RPCMyCasinoCommandLineInterface::draw(unsigned short* firstNumberTest, unsigned short* secondNumberTest)
{
	error_status_t hr;

	// check if only one test number is set is done in ProcessCommands method  
	if (NULL != firstNumberTest && NULL != secondNumberTest)
	{
		//Test
		hr = ::drawTest(*m_pSessionId, *firstNumberTest, *secondNumberTest);
		if (hr == RPC_S_OK)
		{
			std::cout << "Success: drawTest" << std::endl;
		}
		return hr ? false : true;
	}
	else if (NULL == firstNumberTest && NULL == secondNumberTest)
	{
		short firstNumber = 0;
		short secondNumber = 0;
		hr = ::draw(*m_pSessionId, &firstNumber, &secondNumber);
		if (hr == RPC_S_OK)
		{
			std::cout << "Success: drawTest" << std::endl;
			std::cout << "First number: " << firstNumber << std::endl;
			std::cout << "Second number: " << secondNumber << std::endl;
		}
		return hr ? false : true;
	}
	else
	{
		return false;
	}
}


bool RPCMyCasinoCommandLineInterface::showstatus()
{
	boolean isFinished = false;
	MyCasinoTransaction_t transaction;
	unsigned long informationType;
	unsigned long transactionType;
	unsigned long currentTransactionId;
	HRESULT hr;

	bool displayHeader = false;
	while (!isFinished)
	{
		if (!displayHeader)
		{
			std::wcout << "Transaction type | Change amount | Resulting balance | <Additional Information>" << std::endl;
			displayHeader = true;
		}

		hr = ::getTransactions(*m_pSessionId, &isFinished, &transaction, &transactionType);
		if (FAILED(hr))
		{
			return false;
		}

		if (transactionType == MyCasinoTransactionsTypes::DEPOSIT
			|| transactionType == MyCasinoTransactionsTypes::WITHDRAWAL)
		{
			std::wcout << resolve_transaction_type((MyCasinoTransactionsTypes)transactionType) << " | " << transaction.changeAmount << " | " << transaction.resultAmount << std::endl;
		}
		else if (transactionType == MyCasinoTransactionsTypes::BET_WIN
			|| transactionType == MyCasinoTransactionsTypes::BET_LOSS
			)
		{
			/*
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
				std::wcout << resolve_transaction_type((MyCasinoTransactionsTypes)transactionType) << " | " << changeAmount << " | " << resultBalance << " | " << transactionInformation << std::endl;
			}
			*/
		}
	}

	return true;
}

bool RPCMyCasinoCommandLineInterface::bye()
{
	error_status_t hr = ::logout(*m_pSessionId);
	if (hr == RPC_S_OK)
		std::cout << "Logged out" << std::endl;
	return hr ? false : true;
}





