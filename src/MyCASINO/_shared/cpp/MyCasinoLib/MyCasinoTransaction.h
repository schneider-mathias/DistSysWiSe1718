#pragma once
#include <windows.h>
#include <vector>
#include "TaggedUnion.h"
#include "../IncPub/MyCasinoDefines.h"

class IMyCasinoTransactionInformation
{
public:
	IMyCasinoTransactionInformation(ULONG id,ULONG type):
		m_id(id),m_type(type){}

	virtual std::vector<TaggedUnion> GetInformation() = 0;
	virtual SHORT GetInformationCount() = 0;
	bool operator==(IMyCasinoTransactionInformation& ref)
	{
		return ref.m_id == m_id && ref.m_type == m_type;
	}

private:
	ULONG m_id;
	ULONG m_type;
};


class MyCasinoTransaction
{
public:
	MyCasinoTransaction(ULONG id, DOUBLE startAmount, DOUBLE changeAmount);
	~MyCasinoTransaction();

	ULONG GetId();
	DOUBLE GetStartAmount();
	DOUBLE GetChangeAmount();
	IMyCasinoTransactionInformation* GetTransactionInformation();
	MyCasinoTransactionsTypes GetTransactionType();
	BOOL SetTransactionType(MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* transactionDetails);

private:
	ULONG m_id;
	DOUBLE m_startAmount;
	DOUBLE m_changeAmount;
	MyCasinoTransactionsTypes m_transactionType;
	IMyCasinoTransactionInformation* m_pTransactionDetails;
};