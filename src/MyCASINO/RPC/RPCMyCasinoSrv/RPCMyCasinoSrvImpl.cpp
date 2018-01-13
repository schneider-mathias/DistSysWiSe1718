/**--------------------------------------------------------------------------------------------------
// project:	RPCMyCasinoSrv
// file:	RPCMyCasinoSrvImpl.cpp
//
// summary:	Implements the RPC my casino server implementation class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#include "MyCasino_i.h"

#include "AuthServiceWrapper.h"
#include "MyCasinoWrapper.h"

#include "CharStringConverter.h"
#include "MyCasinoBet.h"
#include "MyCasinoTransaction.h"

#include "MyCasinoDefines.h"

// include jsoncpp writer
#include "json/writer.h"

/**--------------------------------------------------------------------------------------------------
 * <summary>	Converts this object to a JSON. </summary>
 *
 * <param name="currentInformation">	[in,out] Information describing the current. </param>
 * <param name="resJson">				[in,out] If non-null, the result JSON. </param>
 *
 * <returns>	The given data converted to a bool. </returns>
 *-----------------------------------------------------------------------------------------------**/

bool toJson(std::vector<TaggedUnion>& currentInformation, Json::Value** resJson)
{
	*resJson = new Json::Value("information");
	(**resJson) = Json::arrayValue;
	

	int jsonArrValue = 0;
	for (std::vector<TaggedUnion>::iterator it = currentInformation.begin(); it != currentInformation.end(); ++it)
	{
		//const char *arrayPos = std::to_string(jsonArrValue).c_str();

		switch ((*it).getType())
		{
		case TaggedUnion::Type::Boolean:
			(**resJson)[jsonArrValue]["type"] = "bool";
			(**resJson)[jsonArrValue]["value"] = (*it).getBoolean();
			break;
		case TaggedUnion::Type::Char:
			(**resJson)[jsonArrValue]["type"] = "char";
			(**resJson)[jsonArrValue]["value"] = (*it).getChar();
			break;
		case TaggedUnion::Type::Double:
			(**resJson)[jsonArrValue]["type"] = "double";
			(**resJson)[jsonArrValue]["value"] = (*it).getDouble();
			break;
		case TaggedUnion::Type::Int:
			(**resJson)[jsonArrValue]["type"] = "int";
			(**resJson)[jsonArrValue]["value"] = (*it).getInt();
			break;
		case TaggedUnion::Type::WStringPtr:
			(**resJson)[jsonArrValue]["type"] = "wstring";
			(**resJson)[jsonArrValue]["value"] = wstring_to_string((*it).getWString()).c_str();
			break;
		default:
			return false;
		}

		jsonArrValue++;
	}
	return true;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Login. </summary>
 *
 * <param name="sessionId">	[in,out] If non-null, identifier for the session. </param>
 * <param name="username"> 	[in,out] If non-null, the username. </param>
 * <param name="password"> 	[in,out] If non-null, the password. </param>
 * <param name="userType"> 	[in,out] If non-null, type of the user. </param>
 *
 * <returns>	An error_status_t. </returns>
 *-----------------------------------------------------------------------------------------------**/

error_status_t login(unsigned long*  sessionId, unsigned char *username, unsigned char *password, short* userType)
{

	if (!getAuthService()->login(char_to_wstring((char*)username), char_to_wstring((char*)password), sessionId))
		return E_MY_CASINO_ACCESS_DENIED;

	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(*sessionId, &user))
		return E_MY_CASINO_INTERNAL_AUTH_SERVICE_ERROR;

	*userType = user->GetUserType();
	if (*userType == MyCasinoUserTypes::Operator)
	{
		BOOL retVal = getCasino()->Open(user);
		if (FAILED(retVal))
			return retVal;
	}
	// close all open bets
	else if (user->GetUserType() == MyCasinoUserTypes::Gamer)
	{
		BOOL retVal = getCasino()->CloseBets(*user);
		if (FAILED(retVal))
			return retVal;
	}

	BOOL resVal = RPC_S_OK;
	if (!getCasino()->IsOpened())
		resVal = S_MY_CASINO_NO_OPERATOR_LOGGED_IN;

	return resVal;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Logout. </summary>
 *
 * <param name="sessionId">	Identifier for the session. </param>
 *
 * <returns>	An error_status_t. </returns>
 *-----------------------------------------------------------------------------------------------**/

error_status_t logout(unsigned long sessionId)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return E_MY_CASINO_USER_NOT_LOGGED_IN;

	// check if user is operator and close casino if it is the current operator
	if (user->GetUserType() == MyCasinoUserTypes::Operator)
	{
		if (!getCasino()->IsOperator(*user))
			return E_MY_CASINO_USER_NOT_LOGGED_IN;

		getCasino()->Close();
	}

	if (!getAuthService()->logout(sessionId))
		return E_MY_CASINO_USER_LOGOUT_FAILED;

	BOOL resVal = S_OK;
	if (!getCasino()->IsOpened())
		resVal = S_MY_CASINO_NO_OPERATOR_LOGGED_IN;

	return resVal;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Deposits. </summary>
 *
 * <param name="sessionId">  	Identifier for the session. </param>
 * <param name="name">		 	[in,out] If non-null, the name. </param>
 * <param name="amountMoney">	The amount money. </param>
 *
 * <returns>	An error_status_t. </returns>
 *-----------------------------------------------------------------------------------------------**/

error_status_t deposit(unsigned long sessionId, unsigned char *name, double amountMoney)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return E_MY_CASINO_USER_NOT_LOGGED_IN;

	if (!getCasino()->IsOperator(*user))
		return E_MY_CASINO_USER_PERMISSION_DENIED;

	ULONG sessionIdForDeposit = 0;
	getAuthService()->isLoggedIn(char_to_wstring((char*)name), &sessionIdForDeposit);

	// get user for deposit
	MyCasinoUser* userForDeposit = NULL;
	if (!getAuthService()->isLoggedIn(sessionIdForDeposit, &userForDeposit))
		return E_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN;

	BOOL resVal = getCasino()->Deposit(*userForDeposit, amountMoney);
	if (FAILED(resVal))
		return E_MY_CASINO_ACCOUNT_DEPOSIT_FAILED;

	return RPC_S_OK;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Bets. </summary>
 *
 * <param name="sessionId">   	Identifier for the session. </param>
 * <param name="amountMoney"> 	The amount money. </param>
 * <param name="firstNumber"> 	The first number. </param>
 * <param name="secondNumber">	The second number. </param>
 *
 * <returns>	An error_status_t. </returns>
 *-----------------------------------------------------------------------------------------------**/

error_status_t bet(unsigned long sessionId, double amountMoney, short firstNumber, short secondNumber)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return E_MY_CASINO_USER_NOT_LOGGED_IN;

	if (!getCasino()->IsOpened())
		return E_MY_CASINO_NO_OPERATOR;

	if (user->GetUserType() == MyCasinoUserTypes::Operator)
		return E_MY_CASINO_USER_PERMISSION_DENIED;

	// create the bet
	BOOL retVal = getCasino()->Bet(*user, firstNumber, secondNumber, amountMoney);
	if (FAILED(retVal))
		return retVal;

	return RPC_S_OK;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Calculates the profit. </summary>
 *
 * <param name="sessionId">			 	Identifier for the session. </param>
 * <param name="amountMoney">		 	The amount money. </param>
 * <param name="firstNumber">		 	The first number. </param>
 * <param name="secondNumber">		 	The second number. </param>
 * <param name="profitForOneMatch">  	[in,out] If non-null, a match specifying the profit for
 * 										one. </param>
 * <param name="profitForTwoMatches">	[in,out] If non-null, the profit for two matches. </param>
 *
 * <returns>	The calculated profit. </returns>
 *-----------------------------------------------------------------------------------------------**/

error_status_t calculateProfit(unsigned long sessionId, double amountMoney, short firstNumber, short secondNumber, double* profitForOneMatch, double* profitForTwoMatches)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return E_MY_CASINO_USER_NOT_LOGGED_IN;

	// create a dummy bet object in order to calculate profits
	MyCasinoBet dummyBet(user->m_username, 1, firstNumber, secondNumber, amountMoney);
	getCasino()->CalculateProfit(dummyBet, profitForOneMatch, profitForTwoMatches);

	return RPC_S_OK;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Showbets. </summary>
 *
 * <param name="sessionId">	Identifier for the session. </param>
 * <param name="bets">	   	[in,out] If non-null, the bets. </param>
 * <param name="count">	   	[in,out] If non-null, number of. </param>
 *
 * <returns>	An error_status_t. </returns>
 *-----------------------------------------------------------------------------------------------**/

error_status_t showbets(unsigned long sessionId, MyCasinoBet_t** bets, unsigned long* count)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return E_MY_CASINO_USER_NOT_LOGGED_IN;

	std::vector<MyCasinoBet*> betsSnapshot = getCasino()->GetBets();
	*count = betsSnapshot.size();

	// midl allocate MyCasinoBet_t array (has to be freed by client)
	(*bets) = (MyCasinoBet_t*)midl_user_allocate((*count) * sizeof(MyCasinoBet_t));

	// fill array
	for (int i = 0; i<(*count); i++)
	{
		(*bets)[i].firstNumber = betsSnapshot.at(i)->GetFirstNumber();
		(*bets)[i].secondNumber = betsSnapshot.at(i)->GetSecondNumber();
		(*bets)[i].wager = betsSnapshot.at(i)->GetSetAmount();

		// allocate memory for String_t which is contained in MyCasinoBet_t
		unsigned short stringSize = strlen((char*)wstring_to_string(betsSnapshot.at(i)->GetUsername()).c_str()) + 1;

		// copy value
		(*bets)[i].name.str = (unsigned char*)midl_user_allocate(stringSize);
		(*bets)[i].name.size = stringSize;
		(*bets)[i].name.len = stringSize;
		strcpy((char*)(*bets)[i].name.str, (char*)wstring_to_string(betsSnapshot.at(i)->GetUsername()).c_str());
	}

	BOOL resVal = S_OK;
	if (!getCasino()->IsOpened())
		resVal = S_MY_CASINO_NO_OPERATOR_LOGGED_IN;


	return resVal;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	result numbers equal input. </summary>
 *
 * <param name="sessionId">		  	Identifier for the session. </param>
 * <param name="firstNumberTest"> 	The first number test. </param>
 * <param name="secondNumberTest">	The second number test. </param>
 *
 * <returns>	An error_status_t. </returns>
 *-----------------------------------------------------------------------------------------------**/

error_status_t drawTest(unsigned long sessionId, short firstNumberTest, short secondNumberTest)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return E_MY_CASINO_USER_NOT_LOGGED_IN;

	if (!getCasino()->IsOperator(*user))
		return E_MY_CASINO_USER_PERMISSION_DENIED;

	short *drawnFirstNumber = new short(firstNumberTest);
	short *drawnSecondNumber = new short(secondNumberTest);

	BOOL hr = getCasino()->Draw(&drawnFirstNumber, &drawnSecondNumber);
	if (FAILED(hr))
		return hr;

	delete drawnFirstNumber;
	delete drawnSecondNumber;

	return RPC_S_OK;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Draws. </summary>
 *
 * <param name="sessionId">   	Identifier for the session. </param>
 * <param name="firstNumber"> 	[in,out] If non-null, the first number. </param>
 * <param name="secondNumber">	[in,out] If non-null, the second number. </param>
 *
 * <returns>	An error_status_t. </returns>
 *-----------------------------------------------------------------------------------------------**/

error_status_t draw(unsigned long sessionId, short* firstNumber, short* secondNumber)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return E_MY_CASINO_USER_NOT_LOGGED_IN;

	if (!getCasino()->IsOperator(*user))
		return E_MY_CASINO_USER_PERMISSION_DENIED;

	short *drawnFirstNumber = NULL;
	short *drawnSecondNumber = NULL;

	BOOL hr = getCasino()->Draw(&drawnFirstNumber, &drawnSecondNumber);
	if (FAILED(hr))
		return hr;

	*firstNumber = *drawnFirstNumber;
	*secondNumber = *drawnSecondNumber;

	delete drawnFirstNumber;
	delete drawnSecondNumber;

	return RPC_S_OK;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets the transactions. </summary>
 *
 * <param name="sessionId">		 	Identifier for the session. </param>
 * <param name="isFinished">	 	[in,out] If non-null, true if this object is finished. </param>
 * <param name="transaction">	 	[in,out] If non-null, the transaction. </param>
 * <param name="transactionType">	[in,out] If non-null, type of the transaction. </param>
 *
 * <returns>	The transactions. </returns>
 *-----------------------------------------------------------------------------------------------**/

error_status_t getTransactions(unsigned long sessionId, boolean* isFinished, MyCasinoTransaction_t* transaction, unsigned long* transactionType)
{

	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return E_MY_CASINO_USER_NOT_LOGGED_IN;

	MyCasinoTransaction* nextTransaction = NULL;
	*isFinished = getCasino()->GetNextTransaction(*user, &nextTransaction);

	if (NULL != nextTransaction)
	{
		transaction->id = nextTransaction->GetId();
		transaction->resultAmount = nextTransaction->GetResultBalance();
		transaction->changeAmount = nextTransaction->GetChangeAmount();

		*transactionType = nextTransaction->GetTransactionType();
	}

	BOOL resVal = RPC_S_OK;
	if (!getCasino()->IsOpened())
		resVal = S_MY_CASINO_NO_OPERATOR_LOGGED_IN;

	return resVal;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets transaction information. </summary>
 *
 * <param name="sessionId">		 	Identifier for the session. </param>
 * <param name="transactionId">  	Identifier for the transaction. </param>
 * <param name="information">	 	[in,out] If non-null, the information. </param>
 * <param name="informationType">	[in,out] If non-null, type of the information. </param>
 *
 * <returns>	The transaction information. </returns>
 *-----------------------------------------------------------------------------------------------**/

error_status_t getTransactionInformation(unsigned long sessionId, unsigned long transactionId, String_t* information, unsigned long* informationType)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return E_MY_CASINO_USER_NOT_LOGGED_IN;

	IMyCasinoTransactionInformation *currentDetails = NULL;
	MyCasinoTransactionsInformationTypes detailType;
	if (!getCasino()->GetTransactionInfomation(*user, transactionId, &currentDetails, &detailType))
		return E_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE;

	// get information details as TaggedUnion vector and convert it to a CComSafeArray
	std::vector<TaggedUnion>currentInformation = (*currentDetails).GetInformation();

	Json::StreamWriterBuilder wbuilder;
	wbuilder["indentation"] = "\t";
	Json::Value* transactionInformationAsJson = NULL;
	if (!::toJson((*currentDetails).GetInformation(), &transactionInformationAsJson))
		return E_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE;
	
	std::string document = Json::writeString(wbuilder, *transactionInformationAsJson);

	// allocate memory for String_t which is contained in MyCasinoBet_t
	unsigned short stringSize = strlen((char*)document.c_str()) + 1;

	// copy value
	information->str = (unsigned char*)midl_user_allocate(stringSize);
	information->size = stringSize;
	information->len = stringSize;
	strcpy((char*)information->str, (char*)document.c_str());

	*informationType = detailType;

	BOOL resVal = RPC_S_OK;
	if (!getCasino()->IsOpened())
		resVal = S_MY_CASINO_NO_OPERATOR_LOGGED_IN;

	return resVal;
}