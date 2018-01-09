#include "ScopedLock.h"
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
	SCOPED_LOCK(m_transactionMutex);

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
	SCOPED_LOCK(m_transactionMutex);

	return m_resultBalance;
}

DOUBLE MyCasinoTransaction::GetChangeAmount()
{
	SCOPED_LOCK(m_transactionMutex);

	return m_changeAmount;
}

void MyCasinoTransaction::SetChangeAmount(DOUBLE changeAmount, DOUBLE resultBalance)
{
	SCOPED_LOCK(m_transactionMutex);

	m_changeAmount = changeAmount;
	m_resultBalance = resultBalance;
}

MyCasinoTransactionsTypes MyCasinoTransaction::GetTransactionType()
{
	return m_transactionType;
}

BOOL MyCasinoTransaction::SetTransactionType(MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* transactionDetails, MyCasinoTransactionsInformationTypes* infoType)
{
	SCOPED_LOCK(m_transactionMutex);

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
	SCOPED_LOCK(m_transactionMutex);

	return m_pTransactionDetails;
}

MyCasinoTransactionsInformationTypes* MyCasinoTransaction::GetTransactionInformationType()
{
	SCOPED_LOCK(m_transactionMutex);

	return m_pTransactionInformationType;
}