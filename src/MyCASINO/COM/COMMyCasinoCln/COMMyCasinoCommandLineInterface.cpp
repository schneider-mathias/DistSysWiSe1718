/**---------------------------------------------------------------------
// project:	COMMyCasinoCln
// file:	COMMyCasinoCommandLineInterface.cpp
//
// summary:	Implements the com my casino command line interface class which calls
//			COM interfaces and displays results on commandline.
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			29.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#include <iostream>
#include <atlsafe.h>

#include "COMMyCasinoCommandLineInterface.h"
#include "BstrStringConverter.h"
#include "CharStringConverter.h"
#include "MyCasinoDefines.h"

#include "bprinter\table_printer.h"

/**---------------------------------------------------------------------
 * <summary>	Constructor. </summary>
 *
 * <param name="interpreter">	  	[in,out] If non-null, the interpreter. </param>
 * <param name="pICOMMyCasinoSrv">	[in,out] If non-null, the icom my casino server. </param>
 *-------------------------------------------------------------------**/

COMMyCasinoCommandLineInterface::COMMyCasinoCommandLineInterface(CmdInterpreter* interpreter, ICOMMyCasino* pICOMMyCasinoSrv)
	: MyCasinoCommandLineInterface(interpreter)
{
	m_pICOMMyCasinoSrv = pICOMMyCasinoSrv;
}

/** <summary>	Destructor. </summary> */
COMMyCasinoCommandLineInterface::~COMMyCasinoCommandLineInterface()
{
	m_pICOMMyCasinoSrv = NULL;
}

/**---------------------------------------------------------------------
 * <summary>	Wrapper for calling COM object's 'login' interface. 
 * 				For detailed information refer to COMMyCasino.cpp </summary>
 *
 * <param name="user">	  	The username. </param>
 * <param name="password">	The password. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool COMMyCasinoCommandLineInterface::user(std::wstring user, std::wstring password)
{

#ifndef _MYCASINO_TEST_CLIENT_
	if (NULL != m_pSessionId)
	{
		std::cout << "[ERROR]: Already logged in with user" << std::endl;
		return false;
	}
#endif

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
	resultHandler("Could not log in to server", hr, bstr_to_str(errMsg));
	if (FAILED(hr))
	{
		delete m_pSessionId;
		m_pSessionId = NULL;
		delete m_pUserType;
		m_pUserType = NULL;
		return false;
	}

	return true;
}

/**---------------------------------------------------------------------
 * <summary>	Wrapper for calling COM object's 'payin' interface.
 * 				For detailed information refer to COMMyCasino.cpp </summary>
 *
 * <param name="user">  	The username. </param>
 * <param name="amount">	The amount. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

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

/**---------------------------------------------------------------------
 * <summary>	Wrapper for calling COM object's 'bet' interface.
 * 				For detailed information refer to COMMyCasino.cpp </summary>
 * 
 * <param name="setAmount">   	The set amount. </param>
 * <param name="firstNumber"> 	The first number. </param>
 * <param name="secondNumber">	The second number. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool COMMyCasinoCommandLineInterface::bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber)
{
	BSTR errMsg;
	HRESULT hr = m_pICOMMyCasinoSrv->bet(*m_pSessionId, setAmount, firstNumber, secondNumber, &errMsg);
	resultHandler("bet", hr, bstr_to_str(errMsg));

	return SUCCEEDED(hr);
}

/**---------------------------------------------------------------------
 * <summary>	Wrapper for calling COM object's 'showbets' and 'calculateProfit' interfaces.
 * 				For detailed information refer to COMMyCasino.cpp </summary>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool COMMyCasinoCommandLineInterface::showbets()
{
	SAFEARR_VAR bets;
	ULONG betCount;
	BSTR errMsg;
	HRESULT hr = m_pICOMMyCasinoSrv->showbets(*m_pSessionId, &bets, &betCount, &errMsg);
	resultHandler("showbets", hr, bstr_to_str(errMsg));
	if (FAILED(hr))
		return false;
	
	bprinter::TablePrinter tp(&std::cout, "|", 2);
	if (betCount > 0)
	{
		tp.AddColumn("User", 15);
		tp.AddColumn("First number", 15);
		tp.AddColumn("Second number", 15);
		tp.AddColumn("Wager", 10);
		tp.AddColumn("Price for one", 15);
		tp.AddColumn("Price for two", 15);
		tp.PrintHeader();
	}
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
			
			tp << bstr_to_str(username) << currentFirstNumber << currentSecondNumber << currentWager << profitForOne << profitForTwo;
		}
	}

	if (betCount > 0)
		tp.PrintFooter();

	if (FAILED(SafeArrayDestroy(bets)))
	{
		std::cerr << "[Error] Clean up of SafeArray failed." << std::endl;
		return false;
	}
	
	return true;
}

/**---------------------------------------------------------------------
 * <summary>	Wrapper for calling COM object's 'draw' interface.
 * 				For detailed information refer to COMMyCasino.cpp </summary>
 *
 * <param name="firstNumberTest"> 	[in,out] If non-null, the first number test. </param>
 * <param name="secondNumberTest">	[in,out] If non-null, the second number test. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool COMMyCasinoCommandLineInterface::draw(unsigned short** firstNumberTest, unsigned short** secondNumberTest)
{
	BSTR errMsg;
	HRESULT hr;

	// check if only one test number is set is done in ProcessCommands method  
	if (NULL != *firstNumberTest && NULL != *secondNumberTest)
	{
		//Test
		
		HRESULT hr = m_pICOMMyCasinoSrv->drawTest(*m_pSessionId, **firstNumberTest, **secondNumberTest, &errMsg);
		if (FAILED(hr))
		{
			resultHandler("drawTest", hr, bstr_to_str(errMsg));
			return false;
		}

	}
	else if (NULL == *firstNumberTest && NULL == *secondNumberTest)
	{
		SHORT firstNumber;
		SHORT secondNumber;
		hr = m_pICOMMyCasinoSrv->draw(*m_pSessionId, &firstNumber, &secondNumber, &errMsg);
		if (FAILED(hr))
		{
			resultHandler("draw", hr, bstr_to_str(errMsg));
			return false;
		}

		*firstNumberTest = new unsigned short(firstNumber);
		*secondNumberTest = new unsigned short(secondNumber);
	}
	else 
	{
		return false;
	}

	return true;
}

/**---------------------------------------------------------------------
 * <summary>	Wrapper for calling COM object's 'getTransactions' 
 * 				and 'getTransactionInformation' interfaces.
 * 				For detailed information refer to COMMyCasino.cpp </summary>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

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

	bprinter::TablePrinter tp(&std::cout, "|", 2);

	bool displayHeader = false;
	while (!isFinished)
	{
		if (!displayHeader)
		{
			tp.AddColumn("Transaction type", 20);
			tp.AddColumn("Change amount", 20);
			tp.AddColumn("Resulting balance", 25);
			tp.AddColumn("<Additional Infomation>", 30);
			tp.PrintHeader();

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
			tp << wstring_to_string(resolve_transaction_type((MyCasinoTransactionsTypes)transactionType)) << changeAmount << resultBalance << "";
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

				std::wstring transactionInformationDetails(L"");
				BOOL isDrawn = false;
				if (informationType == MyCasinoTransactionsInformationTypes::Bet)
				{
					transactionInformationDetails.append(L"S:");
					for (int i = 0; i < BET_FULL_DETAILS_PROPTERY_COUNT; i++)
					{
						if (i % BET_FULL_DETAILS_PROPTERY_COUNT == 4)
						{
							transactionInformationDetails.append(L" D:");
						}

						if (i % BET_FULL_DETAILS_PROPTERY_COUNT == 1
							|| i % BET_FULL_DETAILS_PROPTERY_COUNT == 2
							|| i % BET_FULL_DETAILS_PROPTERY_COUNT == 4
							|| i % BET_FULL_DETAILS_PROPTERY_COUNT == 5)
							transactionInformationDetails.append(L" ").append(std::to_wstring(transactionInformationResult[i].intVal));
					}
				}

				// only display wager of finished bets
				tp << wstring_to_string(resolve_transaction_type((MyCasinoTransactionsTypes)transactionType)) << changeAmount << resultBalance << wstring_to_string(transactionInformationDetails);

				if (FAILED(SafeArrayDestroy(transactionInformation)))
				{
					std::cerr << "[Error] Clean up of SafeArray for transactions failed." << std::endl;
					return false;
				}
			}
		}

		if (FAILED(SafeArrayDestroy(transaction)))
		{
			std::cerr << "[Error] Clean up of SafeArray for transactions failed." << std::endl;
			return false;
		}
	}


	tp.PrintFooter();

	// print once for displaying information messages
	if(hr > 0)
		resultHandler("getTransactionInformation", hr, bstr_to_str(errMsg));

	return true;
}

/**---------------------------------------------------------------------
 * <summary>	Wrapper for calling COM object's 'logout' interface.
 * 				For detailed information refer to COMMyCasino.cpp </summary>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool COMMyCasinoCommandLineInterface::bye()
{
	BSTR errMsg;
	HRESULT hr = m_pICOMMyCasinoSrv->logout(*m_pSessionId, &errMsg);
	resultHandler("bye", hr, bstr_to_str(errMsg));

	return SUCCEEDED(hr);
}