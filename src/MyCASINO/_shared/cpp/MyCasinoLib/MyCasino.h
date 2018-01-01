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

	BOOL MyCasino::Open(MyCasinoUser* user);
	BOOL IsOpened();
	BOOL IsOperator(MyCasinoUser& user);
	BOOL Bet(MyCasinoUser& user, SHORT firstNumber, SHORT secondNumber, DOUBLE setAmount);
	BOOL CalculateProfit(MyCasinoBet& bet, DOUBLE* priceForOne, DOUBLE* priceForTwo);
	BOOL Deposit(MyCasinoUser& user, DOUBLE amount);
	BOOL LoadAccount(MyCasinoUser& user, MyCasinoAccount** account);
	BOOL Withdraw(MyCasinoUser& user, DOUBLE amount);
	std::multimap<MyCasinoUser, MyCasinoBet> CreateSnapshot();
	BOOL Close();
	BOOL Draw();

private:

	BOOL CheckOperatorAccount(MyCasinoBet& bet);
	BOOL GetBet(SHORT firstNumber, SHORT secondNumber, MyCasinoBet* existingBet);
	BOOL DeleteBet(SHORT firstNumber, SHORT secondNumber);
	BOOL GetAccount(MyCasinoUser& user, MyCasinoAccount** account);
	BOOL CheckBet(MyCasinoUser& user, MyCasinoBet& bet);
	BOOL IsValidBetNumber(SHORT firstNumber);

	std::map<MyCasinoUser, MyCasinoAccount> m_userAccounts;
	MyCasinoUser* m_pOperator;
	MyCasinoAccount* m_pOperatorAccount;
	std::multimap<MyCasinoUser, MyCasinoBet> m_currentBets;
	ULONG m_currentBetId;

};