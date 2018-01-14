/**--------------------------------------------------------------------------------------------------
// project:	RPCMyCasinoCln
// file:	RPCMyCasinoCommandLineInterface.cpp
//
// summary:	Implements the RPC my casino command line interface class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#include "RPCMyCasinoCommandLineInterface.h"
#include "MyCasino_i.h"
#include "CharStringConverter.h"
#include "MyCasinoDefines.h"

#include "json\reader.h"
#include "bprinter\table_printer.h"

/**--------------------------------------------------------------------------------------------------
 * <summary>	Constructor. </summary>
 *
 * <param name="interpreter">	[in,out] If non-null, the interpreter. </param>
 *-----------------------------------------------------------------------------------------------**/

RPCMyCasinoCommandLineInterface::RPCMyCasinoCommandLineInterface(CmdInterpreter* interpreter)
	:MyCasinoCommandLineInterface(interpreter)
{
	m_pSessionId = NULL;
	m_pUserType = NULL;
}

/** <summary>	Destructor. </summary> */
RPCMyCasinoCommandLineInterface::~RPCMyCasinoCommandLineInterface()
{
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Users. </summary>
 *
 * <param name="user">	  	The user. </param>
 * <param name="password">	The password. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

bool RPCMyCasinoCommandLineInterface::user(std::wstring user, std::wstring password)
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

	unsigned char* userCStr = (unsigned char*)wstring_to_char(user);
	unsigned char* passwordCStr = (unsigned char*)wstring_to_char(password);

	error_status_t hr = ::login(m_pSessionId, userCStr, passwordCStr, m_pUserType);
	if (FAILED(hr))
	{
		delete m_pSessionId;
		m_pSessionId = NULL;
		delete m_pUserType;
		m_pUserType = NULL;

		std::wstring errCode;
		TRANSLATE_MYCASINO_CODE(errCode, hr);
		resultHandler("Could not log in to server", hr, wstring_to_string(errCode));
		return false;
	}
	
	delete[] userCStr;
	delete[] passwordCStr;

	return true;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Payins. </summary>
 *
 * <param name="user">  	The user. </param>
 * <param name="amount">	The amount. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

bool RPCMyCasinoCommandLineInterface::payin(std::wstring user, double amount)
{
	unsigned char* userCStr = (unsigned char*)wstring_to_char(user);

	error_status_t hr = ::deposit(*m_pSessionId, userCStr, amount);
	if (FAILED(hr))
	{
		std::wstring errCode;
		TRANSLATE_MYCASINO_CODE(errCode, hr);
		resultHandler("payin", hr, wstring_to_string(errCode));
		return false;
	}

	delete[] userCStr;

	return true;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Bets. </summary>
 *
 * <param name="setAmount">   	The set amount. </param>
 * <param name="firstNumber"> 	The first number. </param>
 * <param name="secondNumber">	The second number. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

bool RPCMyCasinoCommandLineInterface::bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber)
{
	error_status_t hr = ::bet(*m_pSessionId, setAmount, (short)firstNumber, (short)secondNumber);
	std::wstring errCode;
	TRANSLATE_MYCASINO_CODE(errCode, hr);
	resultHandler("bet", hr, wstring_to_string(errCode));

	return SUCCEEDED(hr);
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Showbets this object. </summary>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

bool RPCMyCasinoCommandLineInterface::showbets()
{
	MyCasinoBet_t* bets = NULL;
	unsigned long count;
	DOUBLE profitForOne;
	DOUBLE profitForTwo;

	error_status_t hr = ::showbets(*m_pSessionId, &bets, &count);
	std::wstring errCode;
	TRANSLATE_MYCASINO_CODE(errCode, hr);
	resultHandler("bet", hr, wstring_to_string(errCode));

	if (FAILED(hr))
		return false;

	bprinter::TablePrinter tp(&std::cout, "|", 2);
	if (count > 0)
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

	for (int i = 0; i < count; i++)
	{
		hr = ::calculateProfit(*m_pSessionId, bets[i].wager, bets[i].firstNumber, bets[i].secondNumber, &profitForOne, &profitForTwo);
		if (FAILED(hr))
		{
			TRANSLATE_MYCASINO_CODE(errCode, hr);
			resultHandler("calculateProfit", hr, wstring_to_string(errCode));
			continue;
		}

		tp << bets[i].name.str << bets[i].firstNumber << bets[i].secondNumber << bets[i].wager << profitForOne << profitForTwo;
	}

	if (count > 0)
		tp.PrintFooter();

	// free memory
	for (int i = 0; i < count; i++)
		MIDL_user_free(bets[i].name.str);
	MIDL_user_free(bets);

	return true;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Draws. </summary>
 *
 * <param name="firstNumberTest"> 	[in,out] If non-null, the first number test. </param>
 * <param name="secondNumberTest">	[in,out] If non-null, the second number test. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

bool RPCMyCasinoCommandLineInterface::draw(unsigned short** firstNumberTest, unsigned short** secondNumberTest)
{
	error_status_t hr;
	std::wstring errCode;

	// check if only one test number is set is done in ProcessCommands method  
	if (NULL != *firstNumberTest && NULL != *secondNumberTest)
	{
		//Test
		hr = ::drawTest(*m_pSessionId, **firstNumberTest, **secondNumberTest);
		if (FAILED(hr))
		{		
			TRANSLATE_MYCASINO_CODE(errCode, hr);
			resultHandler("drawTest", hr, wstring_to_string(errCode));
		}
	}
	else if (NULL == *firstNumberTest && NULL == *secondNumberTest)
	{
		short firstNumber = 0;
		short secondNumber = 0;
		hr = ::draw(*m_pSessionId, &firstNumber, &secondNumber);
		if (FAILED(hr))
		{
			TRANSLATE_MYCASINO_CODE(errCode, hr);
			resultHandler("draw", hr, wstring_to_string(errCode));
			return false;
		}

		*firstNumberTest = new unsigned short(firstNumber);
		*secondNumberTest = new unsigned short(secondNumber);
	}
	else
	{
		return false;
	}

	return hr ? false : true;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Showstatus this object. </summary>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

bool RPCMyCasinoCommandLineInterface::showstatus()
{
	boolean isFinished = false;
	MyCasinoTransaction_t transaction;
	unsigned long informationType;
	unsigned long transactionType;
	unsigned long currentTransactionId;
	HRESULT hr;
	std::wstring errCode;

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

		hr = ::getTransactions(*m_pSessionId, &isFinished, &transaction, &transactionType);
		if (FAILED(hr))
		{
			TRANSLATE_MYCASINO_CODE(errCode, hr);
			resultHandler("getTransactions", hr, wstring_to_string(errCode));
			return false;
		}

		currentTransactionId = transaction.id;

		if (transactionType == MyCasinoTransactionsTypes::DEPOSIT
			|| transactionType == MyCasinoTransactionsTypes::WITHDRAWAL)
		{
			tp << wstring_to_string(resolve_transaction_type((MyCasinoTransactionsTypes)transactionType)) << transaction.changeAmount << transaction.resultAmount << "";
		}
		else if (transactionType == MyCasinoTransactionsTypes::BET_WIN
			|| transactionType == MyCasinoTransactionsTypes::BET_LOSS
			)
		{
			String_t transactionInformation = {0,0,NULL};
			
			hr = ::getTransactionInformation(*m_pSessionId, currentTransactionId, &transactionInformation, &informationType);
			if (FAILED(hr))
			{
				TRANSLATE_MYCASINO_CODE(errCode, hr);
				resultHandler("getTransactionInformation", hr, wstring_to_string(errCode));
				return false;
			}
				

			if (informationType == MyCasinoTransactionsInformationTypes::Bet)
			{
				
				// parse json response
				std::string parsedTransactionInformation;
				Json::Value informationJson;
				Json::CharReaderBuilder rbuilder;
				Json::CharReader* const reader(rbuilder.newCharReader());
				std::string errMsg;
				
				bool parsingSuccessful = reader->parse((char*)transactionInformation.str, 
					(char*)(transactionInformation.str) + transactionInformation.len,
					&informationJson, &errMsg);
				if (!parsingSuccessful)
				{
					std::cerr << "[Error] Parsing of transaction information failed: " << errMsg << std::endl;
					return false;
				}

				// finalize output message with parsed json
				Json::Value defaultValue("<Not found>");
				parsedTransactionInformation.append(informationJson[1].get("value", defaultValue).asString()).append(" ");
				parsedTransactionInformation.append(informationJson[2].get("value", defaultValue).asString()).append(" ");
				parsedTransactionInformation.append(informationJson[4].get("value", defaultValue).asString()).append(" ");
				parsedTransactionInformation.append(informationJson[5].get("value", defaultValue).asString()).append(" ");

				tp << wstring_to_string(resolve_transaction_type((MyCasinoTransactionsTypes)transactionType)) << transaction.changeAmount << transaction.resultAmount << parsedTransactionInformation;
			}
			else
			{
				std::cerr << "Unknown information type" << std::endl;
				return false;
			}
			
		}
	}

	tp.PrintFooter();

	// print once for displaying information messages
	if (hr > 0)
	{
		TRANSLATE_MYCASINO_CODE(errCode, hr);
		resultHandler("getTransactionInformation", hr, wstring_to_string(errCode));
	}

	return true;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Byes this object. </summary>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

bool RPCMyCasinoCommandLineInterface::bye()
{
	std::wstring errCode;

	error_status_t hr = ::logout(*m_pSessionId);
	TRANSLATE_MYCASINO_CODE(errCode, hr);
	resultHandler("getTransactionInformation", hr, wstring_to_string(errCode));
	return hr ? false : true;
}





