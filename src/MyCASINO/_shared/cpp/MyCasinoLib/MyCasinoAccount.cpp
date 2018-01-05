#include <sstream>

#include "MyCasinoAccount.h"
#include "MyCasinoTransaction.h"


MyCasinoAccount::MyCasinoAccount(DOUBLE balance)
	: m_username(L""),
	m_currentBalance(balance),
	m_preliminaryBalance(0),
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


std::wstring MyCasinoAccount::Serialize()
{
	std::wstring serialized;
	serialized.append(m_username);
	serialized.append(MY_CASINO_ACCOUNT_SERIALIZER_SEPARATOR_WSTRING);
	serialized.append(std::to_wstring(m_currentBalance));
	return serialized;
}
BOOL MyCasinoAccount::Deserialize(std::wstring in)
{
	std::wstring temp;
	std::vector<std::wstring> parts;
	std::wstringstream wss(in);
	while (std::getline(wss, temp, MY_CASINO_ACCOUNT_SERIALIZER_SEPARATOR_CHAR))
		parts.push_back(temp);

	if (parts.size() != MY_CASINO_ACCOUNT_SERIALIZED_PROPERTY_COUNT)
		return FALSE;
	DOUBLE initialBalance = 0.0;
	try
	{
		initialBalance = stod(parts.at(1));
	}
	catch (...)
	{
		return FALSE;
	}

	// deposit inital value on account
	if (initialBalance > 0.001)
	{
		ULONG transactionId = 0;
		CreateTransaction(initialBalance, MyCasinoTransactionsTypes::DEPOSIT, NULL, NULL, &transactionId);
	}

	m_username = parts.at(0);


	return TRUE;
}

std::wstring MyCasinoAccount::GetUsername()
{
	return m_username;
}

BOOL MyCasinoAccount::CreateTransaction(DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType, ULONG* transactionId)
{
	// apply transaction
	switch (type)
	{
	case MyCasinoTransactionsTypes::DEPOSIT:
		// deposit has to be positive
		if(changeAmount < 0.0)
			return ERROR_MY_CASINO_INVALID_CHANGE_AMOUNT;
		m_currentBalance += changeAmount;
		break;
	case MyCasinoTransactionsTypes::WITHDRAWAL:
		if (changeAmount > GetCurrentBalance())
			return ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT;

		// withdrawal has to be negativ
		if (changeAmount > 0.0)
			return ERROR_MY_CASINO_INVALID_CHANGE_AMOUNT;
		m_currentBalance += changeAmount;
		break;
	case MyCasinoTransactionsTypes::BET_WAGER:
		if (changeAmount > GetCurrentBalance())
			return ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT;

		m_preliminaryBalance += changeAmount;
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

	switch (transaction->GetTransactionType())
	{
	case MyCasinoTransactionsTypes::DEPOSIT:
	case MyCasinoTransactionsTypes::WITHDRAWAL:
		m_currentBalance -= transaction->GetChangeAmount();
		break;
	case MyCasinoTransactionsTypes::BET_WAGER:
		m_preliminaryBalance -= transaction->GetChangeAmount();
		break;
	default:
		return ERROR_MY_CASINO_INVALID_TRANSACTION_TYPE;
	}

	transaction->SetTransactionType(MyCasinoTransactionsTypes::CANCELED, NULL, NULL);
	return TRUE;
}

BOOL MyCasinoAccount::GetTransactionInformation(ULONG transactionId, IMyCasinoTransactionInformation** information, MyCasinoTransactionsInformationTypes* infotype)
{
	MyCasinoTransaction* transaction = NULL;
	if (!GetTransaction(transactionId, &transaction))
		return FALSE;

	*information = (*transaction).GetTransactionInformation();
	*infotype = *((*transaction).GetTransactionInformationType());

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
		if (NULL != (*it)->GetTransactionInformation() 
			&& *((*it)->GetTransactionInformation()) == *(transactionInformation))
		{
			*transactionId = ((*it)->GetId());
			return TRUE;
		}
	}

	return FALSE;
}

DOUBLE MyCasinoAccount::GetCurrentBalance()
{
	return m_currentBalance + m_preliminaryBalance;
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

	// only BET_WAGER transaction type is allowed to have a transation
	if(transaction->GetTransactionType() != MyCasinoTransactionsTypes::BET_WAGER)
		return ERROR_MY_CASINO_INVALID_TRANSACTION_TRANSITION;

	
	transaction->SetTransactionType(type, information, infoType);	

	double additionalWageDifference = 0.0;
	double previousAmount = transaction->GetChangeAmount();
	
	// same type simply amount changed
	if (type == MyCasinoTransactionsTypes::BET_WAGER)
	{
		additionalWageDifference = -(previousAmount - changeAmount);

		if (GetCurrentBalance() - additionalWageDifference < 0)
			return ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT;
		m_preliminaryBalance += additionalWageDifference;

		additionalWageDifference = changeAmount;
	}
	else if (type == MyCasinoTransactionsTypes::BET_LOSS)
	{
		additionalWageDifference = changeAmount;
		// remove amount from preliminary and add to actual balance
		m_preliminaryBalance -= changeAmount;

		m_currentBalance += changeAmount;
	}
	else if (type == MyCasinoTransactionsTypes::BET_WIN)
	{
		if (previousAmount < 0.0 && changeAmount > 0.0)
			additionalWageDifference = changeAmount + previousAmount;
		else
			additionalWageDifference = changeAmount;


		// remove amount from preliminary and add to actual balance
		m_preliminaryBalance -= additionalWageDifference;

		m_currentBalance += additionalWageDifference;
	}
	else
	{
		return ERROR_MY_CASINO_INVALID_TRANSACTION_TRANSITION;
	}

	transaction->SetChangeAmount(additionalWageDifference,m_currentBalance);
	return TRUE;
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