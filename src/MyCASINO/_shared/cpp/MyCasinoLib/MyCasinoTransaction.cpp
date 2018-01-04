#include "MyCasinoTransaction.h"


MyCasinoTransaction::MyCasinoTransaction(ULONG id, DOUBLE resultBalance, DOUBLE changeAmount)
	:m_id(id),
	m_resultBalance(resultBalance),
	m_changeAmount(changeAmount),
	m_pTransactionInformationType(NULL),
	m_pTransactionDetails(NULL)
{
}

MyCasinoTransaction::~MyCasinoTransaction()
{
	if (NULL != m_pTransactionInformationType)
	{
		delete m_pTransactionInformationType;
		m_pTransactionInformationType = NULL;
	}

	// ownership has MyCasino
	m_pTransactionDetails = NULL;
}

ULONG MyCasinoTransaction::GetId()
{
	return m_id;
}

DOUBLE MyCasinoTransaction::GetResultBalance()
{
	return m_resultBalance;
}

DOUBLE MyCasinoTransaction::GetChangeAmount()
{
	return m_changeAmount;
}

void MyCasinoTransaction::SetChangeAmount(DOUBLE changeAmount, DOUBLE resultBalance)
{
	m_changeAmount = changeAmount;
	m_resultBalance = resultBalance;
}

MyCasinoTransactionsTypes MyCasinoTransaction::GetTransactionType()
{
	return m_transactionType;
}

BOOL MyCasinoTransaction::SetTransactionType(MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* transactionDetails, MyCasinoTransactionsInformationTypes* infoType)
{
	m_transactionType = type;
	
	// clear information type
	if (NULL != m_pTransactionInformationType)
	{
		delete m_pTransactionInformationType;
		m_pTransactionInformationType = NULL;
	}

	if (NULL != infoType)
	{
		m_pTransactionInformationType = new MyCasinoTransactionsInformationTypes(*infoType);
		m_pTransactionDetails = transactionDetails;
	}

	return TRUE;
}

IMyCasinoTransactionInformation* MyCasinoTransaction::GetTransactionInformation()
{
	return m_pTransactionDetails;
}

MyCasinoTransactionsInformationTypes* MyCasinoTransaction::GetTransactionInformationType()
{
	return m_pTransactionInformationType;
}