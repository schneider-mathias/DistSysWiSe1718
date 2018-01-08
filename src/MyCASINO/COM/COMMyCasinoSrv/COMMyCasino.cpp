// COMMygetCasino().cpp : Implementation of CCOMMyCasino

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
CAuthServiceTemplate<MyCasinoUser>* pAuthServiceInstance=NULL;
MyCasino* pCasinoInstance = NULL;
CAuthServiceTemplate<MyCasinoUser>& getAuthService()
{
	if (pAuthServiceInstance == NULL)
	{
		pAuthServiceInstance = new CAuthServiceTemplate<MyCasinoUser>(&std::wstring(L"USERPROFILE"));
		pAuthServiceInstance->readRegisteredUser(L"\\_MyCasinoData\\mycasino_user.txt");
	}

	return *pAuthServiceInstance;
}

MyCasino& getCasino()
{
	if (pCasinoInstance == NULL)
	{
		pCasinoInstance = new MyCasino(&std::wstring(L"USERPROFILE"));
		pCasinoInstance->LoadAccounts(L"\\_MyCasinoData\\mycasino_account.txt");
	}

	return *pCasinoInstance;
}

// CCOMMyCasino
CCOMMyCasino::CCOMMyCasino() 
{
}

STDMETHODIMP CCOMMyCasino::login(BSTR username, BSTR password, ULONG* sessionId, SHORT* userType, BSTR* errMsg)
{
	std::wstring errCode;

	
	if (!getAuthService().login(bstr_to_wstr(username), bstr_to_wstr(password), sessionId))
	{
		*errMsg = wstr_to_bstr(L"Invalid user name or password.");
		return E_ACCESSDENIED;
	}
	
	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(*sessionId, &user))
	{
		*errMsg = wstr_to_bstr(L"Internal server error in getAuthService().");
		return E_FAIL;
	}

	*userType = user->GetUserType();
	if (*userType == MyCasinoUserTypes::Operator)
	{
		BOOL retVal = getCasino().Open(user);
		if (FAILED(retVal))
		{
			*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, retVal));
			return E_FAIL;
		}
	}
	
	BOOL resVal = S_OK;
	if (!getCasino().IsOpened())
	{
		TRANSLATE_MYCASINO_CODE(errCode, INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN);
		*errMsg = wstr_to_bstr(errCode);
		resVal = INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN;
	}

	return resVal;
}

STDMETHODIMP CCOMMyCasino::logout(ULONG sessionId, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_ACCESSDENIED;
	}

	// check if user is operator and close casino if it is the current operator
	if (user->GetUserType() == MyCasinoUserTypes::Operator)
	{
		if (!getCasino().IsOperator(*user))
		{
			*errMsg = wstr_to_bstr(L"Invalid operator wants to log out.");
			return E_FAIL;
		}
		
		getCasino().Close();
	}

	// log out user
	if (!getAuthService().logout(sessionId))
	{
		*errMsg = wstr_to_bstr(L"Internal server error during logout.");
		return E_FAIL;
	}

	BOOL resVal = S_OK;
	if (!getCasino().IsOpened())
	{
		TRANSLATE_MYCASINO_CODE(errCode, INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN);
		*errMsg = wstr_to_bstr(errCode);
		resVal = INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN;
	}

	return resVal;
}


STDMETHODIMP CCOMMyCasino::deposit(ULONG sessionId, BSTR name, DOUBLE amountMoney, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_ACCESSDENIED;
	}

	if (!getCasino().IsOperator(*user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_PERMISSION_DENIED));
		return E_ACCESSDENIED;
	}

	ULONG sessionIdForDeposit = 0;
	getAuthService().isLoggedIn(name, &sessionIdForDeposit);

	// get user for deposit
	MyCasinoUser* userForDeposit = NULL;
	if (!getAuthService().isLoggedIn(sessionIdForDeposit, &userForDeposit))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN));
		return E_FAIL;
	}

	BOOL resVal = getCasino().Deposit(*userForDeposit, amountMoney);
	if(FAILED(resVal))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_ACCOUNT_DEPOSIT_FAILED));
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::bet(ULONG sessionId, DOUBLE amountMoney, SHORT firstNumber, SHORT secondNumber, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_ACCESSDENIED;
	}

	if (!getCasino().IsOpened())
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_NO_OPERATOR));
		return E_FAIL;
	}

	if (user->GetUserType() == MyCasinoUserTypes::Operator)
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_PERMISSION_DENIED));
		return E_ACCESSDENIED;
	}

	// create the bet
	BOOL retVal = getCasino().Bet(*user, firstNumber, secondNumber, amountMoney);
	if (FAILED(retVal))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, retVal));
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::calculateProfit(ULONG sessionId, DOUBLE amountMoney, SHORT firstNumber, SHORT secondNumber, DOUBLE* profitForOneMatch, DOUBLE* profitForTwoMatches, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_FAIL;
	}

	// create a dummy bet object in order to calculate profits
	MyCasinoBet dummyBet(user->m_username,1, firstNumber, secondNumber, amountMoney);
	getCasino().CalculateProfit(dummyBet, profitForOneMatch, profitForTwoMatches);

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::showbets(ULONG sessionId, SAFEARR_VAR* bets, ULONG* count, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_ACCESSDENIED;
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
		TRANSLATE_MYCASINO_CODE(errCode, INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN);
		*errMsg = wstr_to_bstr(errCode);
		resVal = INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN;
	}
		

	return resVal;
}

STDMETHODIMP CCOMMyCasino::drawTest(ULONG sessionId, SHORT firstNumberTest, SHORT secondNumberTest, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_ACCESSDENIED;
	}

	if (!getCasino().IsOperator(*user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_PERMISSION_DENIED));
		return E_ACCESSDENIED;
	}

	short *drawnFirstNumber = new short(firstNumberTest);
	short *drawnSecondNumber = new short(secondNumberTest);

	BOOL hr = getCasino().Draw(&drawnFirstNumber, &drawnSecondNumber);
	if (FAILED(hr))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, hr));
		return E_FAIL;
	}

	delete drawnFirstNumber;
	delete drawnSecondNumber;

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::draw(ULONG sessionId, SHORT* firstNumber, SHORT* secondNumber, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_ACCESSDENIED;
	}

	if (!getCasino().IsOperator(*user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_PERMISSION_DENIED));
		return E_ACCESSDENIED;
	}


	short *drawnFirstNumber = NULL;
	short *drawnSecondNumber = NULL;

	BOOL hr = getCasino().Draw(&drawnFirstNumber, &drawnSecondNumber);
	if (FAILED(hr))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, hr));
		return E_FAIL;
	}

	*firstNumber = *drawnFirstNumber;
	*secondNumber = *drawnSecondNumber;

	delete drawnFirstNumber;
	delete drawnSecondNumber;

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::getTransactions(ULONG sessionId, BOOL* isFinished, SAFEARR_VAR* transaction, ULONG* transactionType, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_ACCESSDENIED;
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
		TRANSLATE_MYCASINO_CODE(errCode, INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN);
		*errMsg = wstr_to_bstr(errCode);
		resVal = INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN;
	}

	return resVal;
}

STDMETHODIMP CCOMMyCasino::getTransactionInformation(ULONG sessionId, ULONG transactionId, SAFEARR_VAR* information, ULONG* informationType, BSTR* errMsg)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService().isLoggedIn(sessionId, &user))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_USER_NOT_LOGGED_IN));
		return E_ACCESSDENIED;
	}

	IMyCasinoTransactionInformation *currentDetails = NULL;
	MyCasinoTransactionsInformationTypes detailType;
	if(!getCasino().GetTransactionInfomation(*user, transactionId, &currentDetails,&detailType))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE));
		return E_FAIL;
	}

	// get information details as TaggedUnion vector and convert it to a CComSafeArray
	std::vector<TaggedUnion>currentInformation = (*currentDetails).GetInformation();
	CComSafeArray<VARIANT>* transactionInformationSafeArray = new CComSafeArray<VARIANT>(currentInformation.size());
	if (!toCComSafeArray((*currentDetails).GetInformation(), &transactionInformationSafeArray))
	{
		*errMsg = wstr_to_bstr(TRANSLATE_MYCASINO_ERRORCODE(errCode, ERROR_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE));
		return E_FAIL;
	}
	
	*informationType = detailType;

	transactionInformationSafeArray->CopyTo(information);
	delete transactionInformationSafeArray;

	BOOL resVal = S_OK;
	if (!getCasino().IsOpened())
	{
		TRANSLATE_MYCASINO_CODE(errCode, INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN);
		*errMsg = wstr_to_bstr(errCode);
		resVal = INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN;
	}

	return resVal;
}

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