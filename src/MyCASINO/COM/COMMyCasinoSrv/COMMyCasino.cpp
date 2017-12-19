// COMMyCasino.cpp : Implementation of CCOMMyCasino

#include "stdafx.h"
#include "COMMyCasino.h"
#include "BstrStringConverter.h"

// CCOMMyCasino
CCOMMyCasino::CCOMMyCasino() 
	: m_AuthService(&std::wstring(L"USERDATA"))
{
	m_AuthService.readRegisteredUser(L"test.txt");
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
