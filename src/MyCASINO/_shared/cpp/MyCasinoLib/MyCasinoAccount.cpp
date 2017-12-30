#include "MyCasinoAccount.h"
#include "MyCasinoTransaction.h"

MyCasinoAccount::MyCasinoAccount()
	: m_currentBalance(0),
	m_transactionIdCounter(0)
{

}

MyCasinoAccount::~MyCasinoAccount()
{
}

BOOL MyCasinoAccount::CreateTransaction(DOUBLE changeAmount)
{
	MyCasinoTransaction transaction(m_transactionIdCounter++,m_currentBalance, changeAmount);
	m_transactions.push_back(transaction);

	return TRUE;
}

BOOL MyCasinoAccount::GetTransaction(ULONG transactionId, MyCasinoTransaction* transaction)
{
	transaction = NULL;

	for (auto it = m_transactions.begin(); it < m_transactions.end(); it++)
	{
		if ((*it).GetId() == transactionId)
		{
			transaction = &(*it);
			break;
		}
			
	}

	return (NULL != transaction);
}

DOUBLE MyCasinoAccount::GetCurrentBalance()
{
	return m_currentBalance;
}

const std::vector<MyCasinoTransaction>& MyCasinoAccount::GetTransactions()
{
	return m_transactions;
}