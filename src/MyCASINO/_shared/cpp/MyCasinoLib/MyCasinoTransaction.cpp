#include "MyCasinoTransaction.h"


MyCasinoTransaction::MyCasinoTransaction(ULONG id, DOUBLE startAmount, DOUBLE changeAmount)
	:m_id(id),
	m_startAmount(startAmount),
	m_changeAmount(changeAmount)
{
}

MyCasinoTransaction::~MyCasinoTransaction()
{
	m_pTransactionDetails = NULL;
}

DOUBLE MyCasinoTransaction::GetId()
{
	return m_id;
}

DOUBLE MyCasinoTransaction::GetStartAmount()
{
	return m_startAmount;
}

DOUBLE MyCasinoTransaction::GetChangeAmount()
{
	return m_changeAmount;
}


MyCasinoTransactionsTypes MyCasinoTransaction::GetTransactionType()
{
	return m_transactionType;
}

BOOL MyCasinoTransaction::SetTransactionType(MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* transactionDetails)
{
	m_transactionType = type;
	m_pTransactionDetails = transactionDetails;
	return TRUE;
}
