#pragma once
#include <windows.h>
#include <list>

#include "MyCasinoTransaction.h"


class MyCasinoAccount
{
public:
	MyCasinoAccount();
	~MyCasinoAccount();

	BOOL AddTransaction(MyCasinoTransaction transaction);

private:
	DOUBLE m_currentAmount;
	std::list<MyCasinoTransaction> m_transactions;

};