/**--------------------------------------------------------------------------------------------------
// project:	COMMyCasinoSrv
// file:	COMMyCasino.cpp
//
// summary:	Implements the COM MyCasino class 
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			1/13/2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#include "stdafx.h"
#include <atlsafe.h>
#include <vector>

#include "COMMyCasino.h"
#include "BstrStringConverter.h"
#include "MyCasinoBet.h"
#include "MyCasinoTransaction.h"
#include "MyCasinoDefines.h"

// Aggregation is not possible when using CoCreateInstanceEx:
// https://msdn.microsoft.com/de-de/library/windows/desktop/ms680701(v=vs.85).aspx
// thus share classes for all COM objects as global variables

/** <summary>	The shared authentication service instance. </summary> */
CAuthServiceTemplate<MyCasinoUser>* pAuthServiceInstance=NULL;

/** <summary>	The shared casino instance. </summary> */
MyCasino* pCasinoInstance = NULL;

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets authentication service (single instance shared for all COM objects). </summary>
 *
 * <returns>	The authentication service instance. </returns>
 *-----------------------------------------------------------------------------------------------**/

CAuthServiceTemplate<MyCasinoUser>& getAuthService()
{
	if (pAuthServiceInstance == NULL)
	{
		pAuthServiceInstance = new CAuthServiceTemplate<MyCasinoUser>(&std::wstring(L"SystemDrive"));
		pAuthServiceInstance->readRegisteredUser(L"\\_MyCasinoData\\mycasino_user.txt");
	}

	return *pAuthServiceInstance;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets the casino (single instance shared for all COM objects) </summary>
 *
 * <returns>	The casino instance. </returns>
 *-----------------------------------------------------------------------------------------------**/

MyCasino& getCasino()
{
	if (pCasinoInstance == NULL)
	{
		pCasinoInstance = new MyCasino(&std::wstring(L"SystemDrive"));
		pCasinoInstance->LoadAccounts(L"\\_MyCasinoData\\mycasino_account.txt");
	}

	return *pCasinoInstance;
}


/** <summary>	Default constructor. </summary> */
CCOMMyCasino::CCOMMyCasino() 
{
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Login to MyCasino using a username and password. Returns a session id and 
 * 				usertype. The session id is required  in order to call other COM instance
 * 				interfaces.
 * </summary>
 *
 * <param name="username"> 	The username for authentifictation </param>
 * <param name="password"> 	The password for authentifictation </param>
 * <param name="sessionId">	[in,out] If non-null, identifier for the session. </param>
 * <param name="userType"> 	[in,out] If non-null, type of the user. </param>
 * <param name="errMsg">   	[in,out] If non-null, message describing the error. </param>
 *
 * <returns>	A custom MyCasino HRESULT </returns>
 *-----------------------------------------------------------------------------------------------**/

STDMETHODIMP CCOMMyCasino::login(BSTR username, BSTR password, ULONG* sessionId, SHORT* userType, BSTR* errMsg)
{
	std::wstring errCode;

	if (!getAuthService().login(bstr_to_wstr(username), bstr_to_wstr(password), sessionId))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_ACCESS_DENIED));
		return E_MY_CASINO_ACCESS_DENIED;
	}
	
	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(*sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_INTERNAL_AUTH_SERVICE_ERROR));
		return E_MY_CASINO_INTERNAL_AUTH_SERVICE_ERROR;
	}

	*userType = user->GetUserType();
	if (*userType == MyCasinoUserTypes::Operator)
	{
		BOOL retVal = getCasino().Open(user);
		if (FAILED(retVal))
		{
			*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, retVal));
			return retVal;
		}
	}
	
	BOOL resVal = S_OK;
	if (!getCasino().IsOpened())
	{
		TRANSLATE_MYCASINO_CODE(errCode, S_MY_CASINO_NO_OPERATOR_LOGGED_IN);
		*errMsg = wstr_to_bstr(errCode);
		resVal = S_MY_CASINO_NO_OPERATOR_LOGGED_IN;
	}

	return resVal;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Logout for a user. Afterwards session id is not valid anymore. </summary>
 *
 * <param name="sessionId">	Identifier for the session. </param>
 * <param name="errMsg">   	[in,out] If non-null, message describing the error. </param>
 *
 * <returns> A custom MyCasino HRESULT </returns>
 *-----------------------------------------------------------------------------------------------**/

STDMETHODIMP CCOMMyCasino::logout(ULONG sessionId, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_MY_CASINO_USER_NOT_LOGGED_IN;
	}

	// check if user is operator and close casino if it is the current operator
	if (user->GetUserType() == MyCasinoUserTypes::Operator)
	{
		if (!getCasino().IsOperator(*user))
		{
			*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_INVALID_OPERATOR));
			return E_MY_CASINO_INVALID_OPERATOR;
		}
		
		getCasino().Close();
	}
	// close all open bets
	else if (user->GetUserType() == MyCasinoUserTypes::Gamer)
	{
		BOOL retVal = getCasino().CloseBets(*user);
		if (FAILED(retVal))
		{
			*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, retVal));
			return retVal;
		}
	}

	// log out user
	if (!getAuthService().logout(sessionId))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_INTERNAL_LOGOUT_ERROR));
		return E_MY_CASINO_INTERNAL_LOGOUT_ERROR;
	}

	BOOL resVal = S_OK;
	if (!getCasino().IsOpened())
	{
		TRANSLATE_MYCASINO_CODE(errCode, S_MY_CASINO_NO_OPERATOR_LOGGED_IN);
		*errMsg = wstr_to_bstr(errCode);
		resVal = S_MY_CASINO_NO_OPERATOR_LOGGED_IN;
	}

	return resVal;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Deposits a certain amount of money for a logged in user. This method is only 
 * 				allowed to be called by the operator. </summary>
 *
 * <param name="sessionId">  	Identifier for the session. </param>
 * <param name="name">		 	The name of the user for which a deposit should be exectued.
 * 								 </param>
 * <param name="amountMoney">	The amount of money. </param>
 * <param name="errMsg">	 	[in,out] If non-null, message describing the error. </param>
 *
 * <returns> A custom MyCasino HRESULT </returns>
 *-----------------------------------------------------------------------------------------------**/

STDMETHODIMP CCOMMyCasino::deposit(ULONG sessionId, BSTR name, DOUBLE amountMoney, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_MY_CASINO_USER_NOT_LOGGED_IN;
	}

	if (!getCasino().IsOperator(*user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_PERMISSION_DENIED));
		return E_MY_CASINO_USER_PERMISSION_DENIED;
	}

	ULONG sessionIdForDeposit = 0;
	getAuthService().isLoggedIn(name, &sessionIdForDeposit);

	// get user for deposit
	MyCasinoUser* userForDeposit = NULL;
	if (!getAuthService().isLoggedIn(sessionIdForDeposit, &userForDeposit))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN));
		return E_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN;
	}

	BOOL resVal = getCasino().Deposit(*userForDeposit, amountMoney);
	if(FAILED(resVal))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_ACCOUNT_DEPOSIT_FAILED));
		return E_MY_CASINO_ACCOUNT_DEPOSIT_FAILED;
	}

	return S_OK;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Create a bet on two numbers with certain amount of money. 
 * 				Numbers have to be valid and amount of money must not exceed 
 * 				operator's and gamer's account balance. Furthermore only
 * 				one user can bet on two same pair of number at a time. </summary>
 *
 * <param name="sessionId">   	Identifier for the session. </param>
 * <param name="amountMoney"> 	The amount money for this bet. </param>
 * <param name="firstNumber"> 	The first number to bet on. </param>
 * <param name="secondNumber">	The second number to bet on. </param>
 * <param name="errMsg">	  	[in,out] If non-null, message describing the error. </param>
 *
 * <returns> A custom MyCasino HRESULT </returns>
 *-----------------------------------------------------------------------------------------------**/

STDMETHODIMP CCOMMyCasino::bet(ULONG sessionId, DOUBLE amountMoney, SHORT firstNumber, SHORT secondNumber, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_MY_CASINO_USER_NOT_LOGGED_IN;
	}

	if (!getCasino().IsOpened())
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_NO_OPERATOR));
		return E_MY_CASINO_NO_OPERATOR;
	}

	if (user->GetUserType() == MyCasinoUserTypes::Operator)
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_PERMISSION_DENIED));
		return E_MY_CASINO_USER_PERMISSION_DENIED;
	}

	// create the bet
	BOOL retVal = getCasino().Bet(*user, firstNumber, secondNumber, amountMoney);
	if (FAILED(retVal))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, retVal));
		return retVal;
	}

	return S_OK;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Calculates the profit for two numbers and a amount of money. </summary>
 *
 * <param name="sessionId">			 	Identifier for the session. </param>
 * <param name="amountMoney">		 	The amount money. </param>
 * <param name="firstNumber">		 	The first number. </param>
 * <param name="secondNumber">		 	The second number. </param>
 * <param name="profitForOneMatch">  	[in,out] If non-null, a match specifying the profit for
 * 										one. </param>
 * <param name="profitForTwoMatches">	[in,out] If non-null, the profit for two matches. </param>
 * <param name="errMsg">			 	[in,out] If non-null, message describing the error. </param>
 *
 * <returns>	The calculated profit. </returns>
 *-----------------------------------------------------------------------------------------------**/

STDMETHODIMP CCOMMyCasino::calculateProfit(ULONG sessionId, DOUBLE amountMoney, SHORT firstNumber, SHORT secondNumber, DOUBLE* profitForOneMatch, DOUBLE* profitForTwoMatches, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_MY_CASINO_USER_NOT_LOGGED_IN;
	}

	// create a dummy bet object in order to calculate profits
	MyCasinoBet dummyBet(user->m_username,1, firstNumber, secondNumber, amountMoney);
	getCasino().CalculateProfit(dummyBet, profitForOneMatch, profitForTwoMatches);

	return S_OK;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Return all bets that are currently placed including information
 * 				about gamer name, first and second number and amount of money.</summary>
 *
 * <param name="sessionId">	Identifier for the session. </param>
 * <param name="bets">	   	[in,out] If non-null, the bets. </param>
 * <param name="count">	   	[in,out] If non-null, number of bets. </param>
 * <param name="errMsg">   	[in,out] If non-null, message describing the error. </param>
 *
 * <returns> A custom MyCasino HRESULT </returns>
 *-----------------------------------------------------------------------------------------------**/

STDMETHODIMP CCOMMyCasino::showbets(ULONG sessionId, SAFEARR_VAR* bets, ULONG* count, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_MY_CASINO_USER_NOT_LOGGED_IN;
	}


	std::vector<MyCasinoBet*> betsSnapshot = getCasino().GetBets();
	CComSafeArray<VARIANT> betsSafeArray(betsSnapshot.size() * BET_DETAILS_PROPTERY_COUNT);

	int safeArrayIterator = 0;

	for (int betIterator = 0; betIterator < betsSnapshot.size(); betIterator++)
	{
		betsSafeArray[safeArrayIterator++] = wstr_to_bstr(betsSnapshot.at(betIterator)->GetUsername());
		betsSafeArray[safeArrayIterator++] = betsSnapshot.at(betIterator)->GetFirstNumber();
		betsSafeArray[safeArrayIterator++] = betsSnapshot.at(betIterator)->GetSecondNumber();
		betsSafeArray[safeArrayIterator++] = betsSnapshot.at(betIterator)->GetSetAmount();
	}

	betsSafeArray.CopyTo(bets);
	*count = betsSnapshot.size();


	BOOL resVal = S_OK;
	if (!getCasino().IsOpened())
	{
		TRANSLATE_MYCASINO_CODE(errCode, S_MY_CASINO_NO_OPERATOR_LOGGED_IN);
		*errMsg = wstr_to_bstr(errCode);
		resVal = S_MY_CASINO_NO_OPERATOR_LOGGED_IN;
	}
		

	return resVal;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	(For testing) Draws the two specified numbers and calculates results 
 * 				(rewards). This method is only allowed to be called by the operator. </summary>
 *
 * <param name="sessionId">		  	Identifier for the session. </param>
 * <param name="firstNumberTest"> 	The first number for drawing test. </param>
 * <param name="secondNumberTest">	The second number for drawing test. </param>
 * <param name="errMsg">		  	[in,out] If non-null, message describing the error. </param>
 *
 * <returns> A custom MyCasino HRESULT </returns>
 *-----------------------------------------------------------------------------------------------**/

STDMETHODIMP CCOMMyCasino::drawTest(ULONG sessionId, SHORT firstNumberTest, SHORT secondNumberTest, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_MY_CASINO_USER_NOT_LOGGED_IN;
	}

	if (!getCasino().IsOperator(*user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_PERMISSION_DENIED));
		return E_MY_CASINO_USER_PERMISSION_DENIED;
	}

	short *drawnFirstNumber = new short(firstNumberTest);
	short *drawnSecondNumber = new short(secondNumberTest);

	BOOL resVal = getCasino().Draw(&drawnFirstNumber, &drawnSecondNumber);
	if (FAILED(resVal))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, resVal));
		return resVal;
	}

	delete drawnFirstNumber;
	delete drawnSecondNumber;

	return S_OK;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Draws two random numbers, calculates results (rewards) and return these number.
 * 				This method is only allowed to be called by the operator.</summary>
 *
 * <param name="sessionId">   	Identifier for the session. </param>
 * <param name="firstNumber"> 	[in,out] If non-null, the first number. </param>
 * <param name="secondNumber">	[in,out] If non-null, the second number. </param>
 * <param name="errMsg">	  	[in,out] If non-null, message describing the error. </param>
 *
 * <returns> A custom MyCasino HRESULT </returns>
 *-----------------------------------------------------------------------------------------------**/

STDMETHODIMP CCOMMyCasino::draw(ULONG sessionId, SHORT* firstNumber, SHORT* secondNumber, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_MY_CASINO_USER_NOT_LOGGED_IN;
	}

	if (!getCasino().IsOperator(*user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_PERMISSION_DENIED));
		return E_MY_CASINO_USER_PERMISSION_DENIED;
	}


	short *drawnFirstNumber = NULL;
	short *drawnSecondNumber = NULL;

	BOOL resVal = getCasino().Draw(&drawnFirstNumber, &drawnSecondNumber);
	if (FAILED(resVal))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, resVal));
		return resVal;
	}

	*firstNumber = *drawnFirstNumber;
	*secondNumber = *drawnSecondNumber;

	delete drawnFirstNumber;
	delete drawnSecondNumber;

	return S_OK;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets next account transaction of a user including deposits and bet results.
 * 				IsFinished return value indicates when all transactions were sent to client.
 * 				</summary>
 *
 * <param name="sessionId">		 	Identifier for the session. </param>
 * <param name="isFinished">	 	[in,out] If non-null, true if all transactions were sent. </param>
 * <param name="transaction">	 	[in,out] If non-null, the transaction. </param>
 * <param name="transactionType">	[in,out] If non-null, type of the transaction. </param>
 * <param name="errMsg">		 	[in,out] If non-null, message describing the error. </param>
 *
 * <returns> A custom MyCasino HRESULT </returns>
 *-----------------------------------------------------------------------------------------------**/

STDMETHODIMP CCOMMyCasino::getTransactions(ULONG sessionId, BOOL* isFinished, SAFEARR_VAR* transaction, ULONG* transactionType, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_MY_CASINO_USER_NOT_LOGGED_IN;
	}

	MyCasinoTransaction* nextTransaction = NULL;
	*isFinished = getCasino().GetNextTransaction(*user, &nextTransaction);
	
	CComSafeArray<VARIANT> transactionSafeArray(TRANSACTION_PROPTERY_COUNT);
	if (NULL != nextTransaction)
	{
		int safeArrayIterator = 0;
		transactionSafeArray[safeArrayIterator++] = nextTransaction->GetId();
		transactionSafeArray[safeArrayIterator++] = nextTransaction->GetResultBalance();
		transactionSafeArray[safeArrayIterator++] = nextTransaction->GetChangeAmount();

		*transactionType = nextTransaction->GetTransactionType();
	}

	transactionSafeArray.CopyTo(transaction);

	BOOL resVal = S_OK;
	if (!getCasino().IsOpened())
	{
		TRANSLATE_MYCASINO_CODE(errCode, S_MY_CASINO_NO_OPERATOR_LOGGED_IN);
		*errMsg = wstr_to_bstr(errCode);
		resVal = S_MY_CASINO_NO_OPERATOR_LOGGED_IN;
	}

	return resVal;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets transaction information for given transaction id. </summary>
 *
 * <param name="sessionId">		 	Identifier for the session. </param>
 * <param name="transactionId">  	Identifier for the transaction. </param>
 * <param name="information">	 	[in,out] If non-null, the transaction information. </param>
 * <param name="informationType">	[in,out] If non-null, type of the transaction 
 * 									information. </param>
 * <param name="errMsg">		 	[in,out] If non-null, message describing the error. </param>
 *
 * <returns> A custom MyCasino HRESULT </returns>
 *-----------------------------------------------------------------------------------------------**/

STDMETHODIMP CCOMMyCasino::getTransactionInformation(ULONG sessionId, ULONG transactionId, SAFEARR_VAR* information, ULONG* informationType, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_MY_CASINO_USER_NOT_LOGGED_IN;
	}

	IMyCasinoTransactionInformation *currentDetails = NULL;
	MyCasinoTransactionsInformationTypes detailType;
	if(!getCasino().GetTransactionInfomation(*user, transactionId, &currentDetails,&detailType))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE));
		return E_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE;
	}

	// get information details as TaggedUnion vector and convert it to a CComSafeArray
	std::vector<TaggedUnion>currentInformation = (*currentDetails).GetInformation();
	CComSafeArray<VARIANT>* transactionInformationSafeArray = new CComSafeArray<VARIANT>(currentInformation.size());
	if (!toCComSafeArray((*currentDetails).GetInformation(), &transactionInformationSafeArray))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, E_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE));
		return E_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE;
	}
	
	*informationType = detailType;

	transactionInformationSafeArray->CopyTo(information);
	delete transactionInformationSafeArray;

	BOOL resVal = S_OK;
	if (!getCasino().IsOpened())
	{
		TRANSLATE_MYCASINO_CODE(errCode, S_MY_CASINO_NO_OPERATOR_LOGGED_IN);
		*errMsg = wstr_to_bstr(errCode);
		resVal = S_MY_CASINO_NO_OPERATOR_LOGGED_IN;
	}

	return resVal;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Converts this object to a COMSafeArray. </summary>
 *
 * <param name="currentInformation">	[in,out] Current information list as TaggedUnion. </param>
 * <param name="resArr">				[in,out] If non-null, converted COMSafeArray of results. </param>
 *
 * <returns>	Whether conversion was successful. </returns>
 *-----------------------------------------------------------------------------------------------**/

bool CCOMMyCasino::toCComSafeArray(std::vector<TaggedUnion>& currentInformation, CComSafeArray<VARIANT>** resArr)
{
	int safeArrayIterator = 0;

	for (std::vector<TaggedUnion>::iterator it = currentInformation.begin(); it != currentInformation.end(); ++it)
	{
		switch ((*it).getType())
		{
		case TaggedUnion::Type::Boolean:
			(**resArr)[safeArrayIterator++] = (*it).getBoolean();
			break;
		case TaggedUnion::Type::Char:
			(**resArr)[safeArrayIterator++] = (*it).getChar();
			break;
		case TaggedUnion::Type::Double:
			(**resArr)[safeArrayIterator++] = (*it).getDouble();
			break;
		case TaggedUnion::Type::Int:
			(**resArr)[safeArrayIterator++] = (*it).getInt();
			break;
		case TaggedUnion::Type::WStringPtr:
			(**resArr)[safeArrayIterator++] = wstr_to_bstr((*it).getWString());
			break;
		default:
			return false;
		}
	}

	return true;
}