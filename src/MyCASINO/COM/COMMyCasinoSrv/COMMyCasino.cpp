// COMMyCasino.cpp : Implementation of CCOMMyCasino

#include "stdafx.h"
#include "COMMyCasino.h"
#include "BstrStringConverter.h"

// CCOMMyCasino
CCOMMyCasino::CCOMMyCasino() 
	: m_AuthService(&std::wstring(L"USERDATA"))
{
	m_AuthService.readRegisteredUser(L"mycasino_user.txt");
}

STDMETHODIMP CCOMMyCasino::login(BSTR username, BSTR password, ULONG* sessionId, BSTR* errMsg)
{
	
	if (!m_AuthService.login(bstr_to_wstr(username), bstr_to_wstr(password), sessionId))
	{
		return E_ACCESSDENIED;
	}
	
	return S_OK;
}

STDMETHODIMP CCOMMyCasino::logout(ULONG sessionId, BSTR* errMsg)
{
	// TODO: Add your implementation code here
	if (!m_AuthService.logout(sessionId))
	{
		return E_FAIL;
	}

	return S_OK;
}


STDMETHODIMP CCOMMyCasino::deposit(ULONG sessionId, BSTR name, DOUBLE amountMoney, BSTR* errMsg)
{
	return S_OK;
}

STDMETHODIMP CCOMMyCasino::bet(ULONG sessionId, DOUBLE amountMoney, SHORT firstNumber, SHORT secondNumber, BSTR* errMsg)
{
	return S_OK;
}

STDMETHODIMP CCOMMyCasino::calculateProfit(ULONG sessionId, DOUBLE amountMoney, DOUBLE* profitForOneMatch, DOUBLE* profitForTwoMatches, BSTR* errMsg)
{
	return S_OK;
}

STDMETHODIMP CCOMMyCasino::showbets(ULONG sessionId, SAFEARR_VAR* bets, ULONG* count, BSTR* errMsg)
{
	return S_OK;
}

STDMETHODIMP CCOMMyCasino::drawTest(ULONG sessionId, SHORT firstNumberTest, SHORT secondNumberTest, BSTR* errMsg)
{
	return S_OK;
}

STDMETHODIMP CCOMMyCasino::draw(ULONG sessionId, SHORT* firstNumber, SHORT* secondNumber, BSTR* errMsg)
{
	return S_OK;
}

STDMETHODIMP CCOMMyCasino::getTransactions(ULONG sessionId, BOOL* isFinished, SAFEARR_VAR* transaction, ULONG* transactionType, BSTR* errMsg)
{
	return S_OK;
}

STDMETHODIMP CCOMMyCasino::getTransactionInformation(ULONG sessionId, ULONG transactionId, SAFEARR_VAR* information, ULONG* informationType, BSTR* errMsg)
{
	return S_OK;
}