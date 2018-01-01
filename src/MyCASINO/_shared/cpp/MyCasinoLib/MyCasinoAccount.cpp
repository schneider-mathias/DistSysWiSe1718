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

BOOL MyCasinoAccount::CreateTransaction(DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, ULONG* transactionId)
{
	MyCasinoTransaction transaction(m_transactionIdCounter++,m_currentBalance, changeAmount);
	transaction.SetTransactionType(type, information);

	// apply transaction
	switch (transaction.GetTransactionType())
	{
	case MyCasinoTransactionsTypes::DEPOSIT:
		m_currentBalance += transaction.GetChangeAmount();
		break;
	case MyCasinoTransactionsTypes::WITHDRAWAL:
		if (transaction.GetChangeAmount() > GetCurrentBalance())
			return ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT;

		m_currentBalance -= transaction.GetChangeAmount();
		break;
	case MyCasinoTransactionsTypes::PRELIMINARY_WITHDRAWAL:
		if (transaction.GetChangeAmount() > GetCurrentBalance())
			return ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT;
		break;
	default:
		return ERROR_MY_CASINO_INVALID_TRANSACTION_TYPE;
	}
	
	// save to transaction list
	m_transactions.push_back(transaction);

	*transactionId = transaction.GetId();

	return TRUE;
}

BOOL MyCasinoAccount::CancelTransaction(ULONG transactionId)
{
	MyCasinoTransaction* transaction = NULL;
	if (!GetTransaction(transactionId, transaction))
		return FALSE;

	// apply transaction
	switch (transaction->GetTransactionType())
	{
	case MyCasinoTransactionsTypes::PRELIMINARY_WITHDRAWAL:
		if (transaction->GetChangeAmount() > GetCurrentBalance())
			return ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT;
		break;
	default:
		return ERROR_MY_CASINO_INVALID_TRANSACTION_TYPE;
	}

	transaction->SetTransactionType(MyCasinoTransactionsTypes::CANCELED, NULL);
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

BOOL MyCasinoAccount::GetTransaction(IMyCasinoTransactionInformation* transactionInformation, MyCasinoTransaction* transaction)
{
	if (NULL == transactionInformation)
		return FALSE;

	transaction = NULL;

	for (auto it = m_transactions.begin(); it < m_transactions.end(); it++)
	{
		// ToDo compare informations
		if (*((*it).GetTransactionInformation()) == *(transactionInformation))
		{
			transaction = &(*it);
			break;
		}
	}

	return (NULL != transaction);
}

DOUBLE MyCasinoAccount::CalculatePreliminaryBalance()
{
	DOUBLE preliminaryBalance = 0.0;

	for (auto it = m_transactions.begin(); it < m_transactions.end(); it++)
	{
		if ((*it).GetTransactionType() == MyCasinoTransactionsTypes::PRELIMINARY_WITHDRAWAL)
			preliminaryBalance +=(*it).GetChangeAmount();
	}
	return preliminaryBalance;
}

DOUBLE MyCasinoAccount::GetCurrentBalance()
{
	return m_currentBalance - CalculatePreliminaryBalance();
}

const std::vector<MyCasinoTransaction>& MyCasinoAccount::GetTransactions()
{
	return m_transactions;
}