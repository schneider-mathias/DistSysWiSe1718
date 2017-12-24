#pragma once
#include <windows.h>

enum MyCasinoTransactionsTypes
{
	DEPOSIT = 0,
	WITHDRAWAL,
	BET
};


class MyCasinoTransaction
{
public:
	MyCasinoTransaction(DOUBLE startAmount, DOUBLE changeAmount);
	~MyCasinoTransaction();

	BOOL SetTransactionType(MyCasinoTransactionsTypes type, void* transactionDetails);
private:
	MyCasinoTransactionsTypes m_transactionType;
	void* m_transactionDetails;

};