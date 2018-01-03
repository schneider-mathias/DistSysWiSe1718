#pragma once
#include <windows.h>
#include <list>

#include "MyCasinoTransaction.h"


class MyCasinoAccount
{
public:
	MyCasinoAccount();
	~MyCasinoAccount();

	BOOL CreateTransaction(DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType, ULONG* transactionId);
	DOUBLE GetCurrentBalance();
	const std::vector<MyCasinoTransaction*>& GetTransactions();
	
	BOOL GetTransaction(IMyCasinoTransactionInformation* transactionInformation, ULONG* transactionId);

	BOOL ChangeTransaction(ULONG transactionId, DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType);
	BOOL ChangeTransaction(ULONG transactionId, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType);
	BOOL GetTransactionInformation(ULONG transactionId, IMyCasinoTransactionInformation** information, MyCasinoTransactionsInformationTypes* infotype);
	BOOL CancelTransaction(ULONG transactionId);
	
	BOOL GetNextTransaction(MyCasinoTransaction** nextTransaction);
	

private:
	BOOL GetTransaction(ULONG transactionId, MyCasinoTransaction** transaction);
	DOUBLE m_currentBalance;
	ULONG m_transactionIdCounter;
	int m_currentTransactionIterator;
	std::vector<MyCasinoTransaction*> m_transactions;

};