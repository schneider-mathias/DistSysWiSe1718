#include "MyCasinoAccount.h"
#include "MyCasinoTransaction.h"

MyCasinoAccount::MyCasinoAccount()
	: m_currentBalance(0),
	m_transactionIdCounter(0),
	m_currentTransactionIterator(0)
{

}

MyCasinoAccount::~MyCasinoAccount()
{

	// finally delete all transaction objects
	for (auto it = m_transactions.begin(); it != m_transactions.end(); it++)
	{
		delete (*it);
	}
}

BOOL MyCasinoAccount::CreateTransaction(DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType, ULONG* transactionId)
{
	// apply transaction
	switch (type)
	{
	case MyCasinoTransactionsTypes::DEPOSIT:
	case MyCasinoTransactionsTypes::BET_WIN:

		m_currentBalance += changeAmount;
		break;
	case MyCasinoTransactionsTypes::BET_LOSS:
	case MyCasinoTransactionsTypes::BET_WAGER:
	case MyCasinoTransactionsTypes::WITHDRAWAL:
		if (changeAmount > GetCurrentBalance())
			return ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT;

		m_currentBalance -= changeAmount;
		break;
	default:
		return ERROR_MY_CASINO_INVALID_TRANSACTION_TYPE;
	}
	
	MyCasinoTransaction* transaction = new MyCasinoTransaction(m_transactionIdCounter++, m_currentBalance, changeAmount);
	transaction->SetTransactionType(type, information, infoType);


	// save to transaction list
	m_transactions.push_back(transaction);

	*transactionId = transaction->GetId();

	return TRUE;
}

BOOL MyCasinoAccount::CancelTransaction(ULONG transactionId)
{
	MyCasinoTransaction* transaction = NULL;
	if (!GetTransaction(transactionId, &transaction))
		return FALSE;

	// apply transaction
	switch (transaction->GetTransactionType())
	{
	case MyCasinoTransactionsTypes::DEPOSIT:
		m_currentBalance -= transaction->GetChangeAmount();
		break;
	case MyCasinoTransactionsTypes::WITHDRAWAL:
	case MyCasinoTransactionsTypes::BET_WAGER:
		m_currentBalance += transaction->GetChangeAmount();
		break;
	default:
		return ERROR_MY_CASINO_INVALID_TRANSACTION_TYPE;
	}

	transaction->SetTransactionType(MyCasinoTransactionsTypes::CANCELED, NULL, NULL);
	return TRUE;
}

BOOL MyCasinoAccount::GetTransactionInformation(ULONG transactionId, IMyCasinoTransactionInformation** information, MyCasinoTransactionsInformationTypes* infotype)
{
	MyCasinoTransaction* info = NULL;
	if (!GetTransaction(transactionId, &info))
		return false;

	*information = (*info).GetTransactionInformation();
	*infotype = *((*info).GetTransactionInformationType());

	return (NULL != *information);
}


BOOL MyCasinoAccount::GetTransaction(ULONG transactionId, MyCasinoTransaction** transaction)
{
	*transaction = NULL;

	for (auto it = m_transactions.begin(); it < m_transactions.end(); it++)
	{
		if ((*it)->GetId() == transactionId)
		{
			*transaction = (*it);
			break;
		}
	}

	return (NULL != *transaction);
}


BOOL MyCasinoAccount::GetTransaction(IMyCasinoTransactionInformation* transactionInformation, ULONG* transactionId)
{
	if (NULL == transactionInformation)
		return FALSE;

	for (auto it = m_transactions.begin(); it < m_transactions.end(); it++)
	{
		// ToDo compare informations
		if (NULL != (*it)->GetTransactionInformation() && *((*it)->GetTransactionInformation()) == *(transactionInformation))
		{
			*transactionId = ((*it)->GetId());
			return TRUE;
		}
	}

	return FALSE;
}

DOUBLE MyCasinoAccount::GetCurrentBalance()
{
	return m_currentBalance;
}

const std::vector<MyCasinoTransaction*>& MyCasinoAccount::GetTransactions()
{
	return m_transactions;
}


BOOL MyCasinoAccount::ChangeTransaction(ULONG transactionId, DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType)
{
	MyCasinoTransaction* transaction = NULL;
	if (!GetTransaction(transactionId, &transaction))
		return FALSE;

	return transaction->SetTransactionType(type, information, infoType);
}

BOOL MyCasinoAccount::ChangeTransaction(ULONG transactionId, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType)
{
	MyCasinoTransaction* transaction = NULL;
	if (!GetTransaction(transactionId, &transaction))
		return FALSE;

	return transaction->SetTransactionType(type, information, infoType);
}

BOOL MyCasinoAccount::GetNextTransaction(MyCasinoTransaction** nextTransaction)
{
	for (auto it = m_transactions.begin() + m_currentTransactionIterator; it < m_transactions.end();)
	{
		*nextTransaction = (*it);
		m_currentTransactionIterator++;
		it++;
		bool isFinished = (it == m_transactions.end());

		// reset iterator for getting transactions
		if (isFinished)
			m_currentTransactionIterator = 0;
		
		return isFinished;
	}

	return TRUE;
}