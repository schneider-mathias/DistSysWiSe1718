#include "MyCasino.h"
#include <utility>

MyCasino::MyCasino()
{
}

MyCasino::~MyCasino()
{
}

BOOL MyCasino::Open(MyCasinoUser& user)
{
	if (IsOpened())
		return ERROR_MY_CASINO_HAS_ALREADY_OPERATOR;

	m_pOperator = &user;
	if (!LoadAccount(*m_pOperator, m_pOperatorAccount))
		return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

	return TRUE;
}

BOOL MyCasino::IsOpened()
{
	return (NULL != m_pOperator);
}

BOOL MyCasino::LoadAccount(MyCasinoUser& user, MyCasinoAccount* account)
{
	account = NULL;

	if (!IsOpened())
		return ERROR_MY_CASINO_NO_OPERATOR;

	// TODO


	return TRUE;
}

BOOL MyCasino::Bet(MyCasinoUser& user, MyCasinoBet& bet)
{
	MyCasinoAccount* account = NULL;

	for (std::map<MyCasinoUser, MyCasinoAccount>::iterator it = m_userAccounts.begin(); it != m_userAccounts.end(); it++)
	{
		if (user == (*it).first)
		{
			account = &((*it).second);
			break;
		}
	}

	if (NULL == account)
	{
		if (!LoadAccount(user, account))
			return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;
	}
	
	account->CreateTransaction(bet.GetSetAmount());

	return E_NOTIMPL;
}

BOOL MyCasino::Deposit(MyCasinoUser& user, DOUBLE amount)
{
	return E_NOTIMPL;
}

BOOL MyCasino::Withdraw(MyCasinoUser& user, DOUBLE amount)
{
	return E_NOTIMPL;
}

BOOL MyCasino::Draw()
{
	return E_NOTIMPL;
}

std::list<MyCasinoBet> MyCasino::CreateSnapshot()
{
	std::list<MyCasinoBet> betsSnapshot;
	return betsSnapshot;
}

BOOL MyCasino::Close()
{
	if (!IsOpened())
		return ERROR_MY_CASINO_NO_OPERATOR;

	delete m_pOperator;
	m_pOperator = NULL;
	delete m_pOperatorAccount;
	m_pOperatorAccount = NULL;

	return TRUE;
}
