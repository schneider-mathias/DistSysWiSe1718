#pragma once
#include <windows.h>
#include <list>

#include "MyCasinoTransaction.h"


class MyCasinoAccount
{
public:
	MyCasinoAccount();
	~MyCasinoAccount();

	BOOL CreateTransaction(DOUBLE changeAmount);
	DOUBLE GetCurrentBalance();
	const std::vector<MyCasinoTransaction>& GetTransactions();
	BOOL GetTransaction(ULONG transactionId, MyCasinoTransaction* transaction);

private:
	DOUBLE m_currentBalance;
	ULONG m_transactionIdCounter;
	std::vector<MyCasinoTransaction> m_transactions;

};