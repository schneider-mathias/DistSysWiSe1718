#pragma once
#include <windows.h>
#include <list>

#include "MyCasinoTransaction.h"


class MyCasinoAccount
{
public:
	MyCasinoAccount();
	~MyCasinoAccount();

	BOOL CreateTransaction(DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, ULONG* transactionId);
	DOUBLE GetCurrentBalance();
	const std::vector<MyCasinoTransaction>& GetTransactions();
	BOOL GetTransaction(ULONG transactionId, MyCasinoTransaction* transaction);
	BOOL GetTransaction(IMyCasinoTransactionInformation* transactionInformation, MyCasinoTransaction* transaction);
	BOOL CancelTransaction(ULONG transactionId);

private:
	DOUBLE CalculatePreliminaryBalance();

	DOUBLE m_currentBalance;
	ULONG m_transactionIdCounter;
	std::vector<MyCasinoTransaction> m_transactions;

};