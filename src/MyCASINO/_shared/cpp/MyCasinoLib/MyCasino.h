#pragma once
#include <map>
#include <list>
#include "MyCasinoUser.h"
#include "MyCasinoAccount.h"
#include "MyCasinoBet.h"

class MyCasino
{
public:
	MyCasino();
	~MyCasino();

	BOOL Open(MyCasinoUser user);
	BOOL Bet(MyCasinoUser user, MyCasinoBet bet);
	BOOL Deposit(MyCasinoUser user, DOUBLE amount);
	BOOL Withdraw(MyCasinoUser user, DOUBLE amount);
	BOOL Close();

private:
	std::map<MyCasinoUser, MyCasinoAccount> m_userAccounts;
	MyCasinoUser* m_pOperator;
	MyCasinoAccount* m_pOperatorAccount;
	std::list<MyCasinoBet> m_currentBets;

};