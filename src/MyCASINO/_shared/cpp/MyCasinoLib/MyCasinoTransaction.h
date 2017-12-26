#pragma once
#include <windows.h>
#include <vector>
#include "TaggedUnion.h"

enum MyCasinoTransactionsTypes
{
	DEPOSIT = 0,
	WITHDRAWAL,
	BET
};


class IMyCasinoTransactionInformation
{
public:
	virtual std::vector<TaggedUnion> GetInformation() = 0;
	virtual SHORT GetInformationCount() = 0;
};


class MyCasinoTransaction
{
public:
	MyCasinoTransaction(ULONG id, DOUBLE startAmount, DOUBLE changeAmount);
	~MyCasinoTransaction();

	DOUBLE GetId();
	DOUBLE GetStartAmount();
	DOUBLE GetChangeAmount();
	MyCasinoTransactionsTypes GetTransactionType();
	BOOL SetTransactionType(MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* transactionDetails);

private:
	ULONG m_id;
	DOUBLE m_startAmount;
	DOUBLE m_changeAmount;
	MyCasinoTransactionsTypes m_transactionType;
	IMyCasinoTransactionInformation* m_pTransactionDetails;
};