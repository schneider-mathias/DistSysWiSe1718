// COMMyCasino.cpp : Implementation of CCOMMyCasino

#include "stdafx.h"
#include <atlsafe.h>

#include "COMMyCasino.h"
#include "BstrStringConverter.h"
#include "MyCasinoBet.h"
#include "MyCasinoTransaction.h"
#include "MyCasinoDefines.h"

// CCOMMyCasino
CCOMMyCasino::CCOMMyCasino() 
	: m_AuthService(&std::wstring(L"USERDATA"))
{
	m_AuthService.readRegisteredUser(L"mycasino_user.txt");
}

STDMETHODIMP CCOMMyCasino::login(BSTR username, BSTR password, ULONG* sessionId, SHORT* userType, BSTR* errMsg)
{
	
	if (!m_AuthService.login(bstr_to_wstr(username), bstr_to_wstr(password), sessionId))
	{
		return E_ACCESSDENIED;
	}
	
	MyCasinoUser user;
	if (!m_AuthService.isLoggedIn(*sessionId, &user))
	{
		return E_FAIL;
	}

	*userType = user.GetUserType();
	if (*userType == MyCasinoUserTypes::Operator)
	{
		m_casino.Open(user);
	}
	


#ifdef STUB_METHODS
	*errMsg = wstr_to_bstr(L"STUB_METHOD - login");
#endif

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::logout(ULONG sessionId, BSTR* errMsg)
{
	MyCasinoUser user;
	if (!m_AuthService.isLoggedIn(sessionId, &user))
	{
		return E_FAIL;
	}

	// TODO: Add your implementation code here
	if (!m_AuthService.logout(sessionId))
	{
		return E_FAIL;
	}

#ifdef STUB_METHODS
	*errMsg = wstr_to_bstr(L"STUB_METHOD - logout");
#endif

	return S_OK;
}


STDMETHODIMP CCOMMyCasino::deposit(ULONG sessionId, BSTR name, DOUBLE amountMoney, BSTR* errMsg)
{
	MyCasinoUser user;
	if (!m_AuthService.isLoggedIn(sessionId, &user))
	{
		return E_FAIL;
	}

#ifdef STUB_METHODS
	*errMsg = wstr_to_bstr(L"STUB_METHOD - deposit");
#endif

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::bet(ULONG sessionId, DOUBLE amountMoney, SHORT firstNumber, SHORT secondNumber, BSTR* errMsg)
{
	MyCasinoUser user;
	if (!m_AuthService.isLoggedIn(sessionId, &user))
	{
		return E_FAIL;
	}

#ifdef STUB_METHODS
	*errMsg = wstr_to_bstr(L"STUB_METHOD - bet");
#endif

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::calculateProfit(ULONG sessionId, DOUBLE amountMoney, DOUBLE* profitForOneMatch, DOUBLE* profitForTwoMatches, BSTR* errMsg)
{
	MyCasinoUser user;
	if (!m_AuthService.isLoggedIn(sessionId, &user))
	{
		return E_FAIL;
	}

	// stub method:
#ifdef STUB_METHODS
	*profitForOneMatch = amountMoney * 2;
	*profitForTwoMatches = amountMoney * 10;
	*errMsg = wstr_to_bstr(L"STUB_METHOD - calculateProfit");
#endif

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::showbets(ULONG sessionId, SAFEARR_VAR* bets, ULONG* count, BSTR* errMsg)
{
	MyCasinoUser user;
	if (!m_AuthService.isLoggedIn(sessionId, &user))
	{
		return E_FAIL;
	}

	// stub method:
#ifdef STUB_METHODS
	const int STUB_DATA_COUNT = 2;
	MyCasinoBet betsStub[STUB_DATA_COUNT] = { MyCasinoBet(1, 1, 10.0), MyCasinoBet(2, 2, 20.0) };
	CComSafeArray<VARIANT> betsSafeArray(STUB_DATA_COUNT * BET_DETAILS_PROPTERY_COUNT);
	int dataCount = STUB_DATA_COUNT;
	*errMsg = wstr_to_bstr(L"STUB_METHOD - showbets");
#endif

	int safeArrayIterator = 0;
	for (int betIterator = 0; betIterator < dataCount; betIterator++)
	{
		betsSafeArray[safeArrayIterator++] = betsStub[betIterator].GetFirstNumber();
		betsSafeArray[safeArrayIterator++] = betsStub[betIterator].GetSecondNumber();
		betsSafeArray[safeArrayIterator++] = betsStub[betIterator].GetSetAmount();
	}

	betsSafeArray.CopyTo(bets);
	*count = dataCount;

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::drawTest(ULONG sessionId, SHORT firstNumberTest, SHORT secondNumberTest, BSTR* errMsg)
{
	MyCasinoUser user;
	if (!m_AuthService.isLoggedIn(sessionId, &user))
	{
		return E_FAIL;
	}

#ifdef STUB_METHODS
	*errMsg = wstr_to_bstr(L"STUB_METHOD - drawTest");
#endif

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::draw(ULONG sessionId, SHORT* firstNumber, SHORT* secondNumber, BSTR* errMsg)
{
	MyCasinoUser user;
	if (!m_AuthService.isLoggedIn(sessionId, &user))
	{
		return E_FAIL;
	}

#ifdef STUB_METHODS
	*firstNumber = 1;
	*secondNumber = 1;
	*errMsg = wstr_to_bstr(L"STUB_METHOD - draw");
#endif

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::getTransactions(ULONG sessionId, BOOL* isFinished, SAFEARR_VAR* transaction, ULONG* transactionType, BSTR* errMsg)
{
	MyCasinoUser user;
	if (!m_AuthService.isLoggedIn(sessionId, &user))
	{
		return E_FAIL;
	}

#ifdef STUB_METHODS
	*isFinished = TRUE;

	MyCasinoTransaction currentTransaction(1, 50.0, 100.0);
	MyCasinoBet betStub(1, 1, 50.0);
	currentTransaction.SetTransactionType(MyCasinoTransactionsTypes::BET, &betStub);

	CComSafeArray<VARIANT> transactionSafeArray(TRANSACTION_PROPTERY_COUNT);
	*errMsg = wstr_to_bstr(L"STUB_METHOD - getTransactions");
#endif

	int safeArrayIterator = 0;
	transactionSafeArray[safeArrayIterator++] = currentTransaction.GetId();
	transactionSafeArray[safeArrayIterator++] = currentTransaction.GetStartAmount();
	transactionSafeArray[safeArrayIterator++] = currentTransaction.GetChangeAmount();

	*transactionType = currentTransaction.GetTransactionType();

	transactionSafeArray.CopyTo(transaction);

	return S_OK;
}

STDMETHODIMP CCOMMyCasino::getTransactionInformation(ULONG sessionId, ULONG transactionId, SAFEARR_VAR* information, ULONG* informationType, BSTR* errMsg)
{
	MyCasinoUser user;
	if (!m_AuthService.isLoggedIn(sessionId, &user))
	{
		return E_FAIL;
	}

#ifdef STUB_METHODS
	MyCasinoTransaction currentTransaction(1, 50.0, 100.0);
	MyCasinoBet currentDetails(1, 1, 50.0);
	currentDetails.SetBetResult(2, 2, 20.5);
	currentTransaction.SetTransactionType(MyCasinoTransactionsTypes::BET, &currentDetails);

	int safearraySize = currentDetails.GetInformationCount();
	CComSafeArray<VARIANT> transactionInformationSafeArray(safearraySize);
	*errMsg = wstr_to_bstr(L"STUB_METHOD - getTransactions");
#endif

	int safeArrayIterator = 0;
	

	// ugly... need to refactor this TaggedArray stuff...
	std::vector<TaggedUnion>currentInformation = currentDetails.GetInformation();
	for (std::vector<TaggedUnion>::iterator it = currentInformation.begin(); it != currentInformation.end(); ++it) 
	{	
		switch ((*it).getType())
		{
			case TaggedUnion::Type::Boolean:
				transactionInformationSafeArray[safeArrayIterator++] = (*it).getBoolean();
				break;
			case TaggedUnion::Type::Char:
				transactionInformationSafeArray[safeArrayIterator++] = (*it).getChar();
				break;
			case TaggedUnion::Type::Double:
				transactionInformationSafeArray[safeArrayIterator++] = (*it).getDouble();
				break;
			case TaggedUnion::Type::Int:
				transactionInformationSafeArray[safeArrayIterator++] = (*it).getInt();
				break;
			default:
				assert("Unknown TaggedUnion::Type");
				break;
		}
		
	}
	
	*informationType = currentTransaction.GetTransactionType();

	transactionInformationSafeArray.CopyTo(information);
	

	return S_OK;
}