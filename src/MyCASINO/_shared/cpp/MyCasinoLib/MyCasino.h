#pragma once
#include <map>
#include <list>
#include "MyCasinoUser.h"
#include "MyCasinoAccount.h"
#include "MyCasinoBet.h"

class MyCasino
{
public:
	MyCasino(std::wstring* userDataDirRootEnv);
	~MyCasino();

	BOOL MyCasino::Open(MyCasinoUser* user);
	BOOL IsOpened();
	BOOL IsOperator(MyCasinoUser& user);
	BOOL LoadAccounts(std::wstring filename);
	BOOL GetNextTransaction(MyCasinoUser& user, MyCasinoTransaction** const transaction);
	BOOL GetTransactionInfomation(MyCasinoUser& user, ULONG transationId, IMyCasinoTransactionInformation** const transactionInformation, MyCasinoTransactionsInformationTypes* type);
	BOOL Bet(MyCasinoUser& user, SHORT firstNumber, SHORT secondNumber, DOUBLE setAmount);
	BOOL CalculateProfit(MyCasinoBet& bet, DOUBLE* priceForOne, DOUBLE* priceForTwo);
	BOOL Deposit(MyCasinoUser& user, DOUBLE amount);
	BOOL Withdraw(MyCasinoUser& user, DOUBLE amount);
	std::vector<MyCasinoBet*> GetBets();
	BOOL Close();
	BOOL Draw(SHORT** firstNumber=NULL, SHORT** secondNumber=NULL);

private:
	SHORT GenerateDrawNumber(SHORT* firstNumber);
	BOOL CheckOperatorAccount(MyCasinoBet& bet);
	BOOL GetBet(SHORT firstNumber, SHORT secondNumber, MyCasinoBet** bet);
	BOOL CloseBet(const MyCasinoUser& user, MyCasinoBet& bet);
	BOOL DeleteBet(SHORT firstNumber, SHORT secondNumber);
	BOOL LoadAccount(const MyCasinoUser& user, MyCasinoAccount** account);
	BOOL SaveAccounts();
	BOOL GetAccount(const MyCasinoUser& user, MyCasinoAccount** account);
	BOOL CheckBet(MyCasinoUser& user, MyCasinoBet& bet);
	BOOL IsValidBetNumber(SHORT firstNumber);

	std::wstring m_userDataDirRoot;
	std::wstring m_accountDataFilename;
	std::map<MyCasinoUser, MyCasinoAccount*> m_loadedUserAccounts;
	std::vector<MyCasinoAccount*> m_userAccounts;
	MyCasinoUser* m_pOperator;
	MyCasinoAccount* m_pOperatorAccount;
	std::multimap<MyCasinoUser, MyCasinoBet*> m_currentBets;
	std::vector<MyCasinoBet*> m_formerBets;
	ULONG m_currentBetId;
	CRITICAL_SECTION m_critSection;

};