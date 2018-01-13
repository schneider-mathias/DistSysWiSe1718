/**--------------------------------------------------------------------------------------------------
// project:	MyCasinoLib
// file:	MyCasinoAccount.cpp
//
// summary:	Implements my casino account class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#include <sstream>

#include "ScopedLock.h"
#include "MyCasinoAccount.h"
#include "MyCasinoTransaction.h"

/**--------------------------------------------------------------------------------------------------
 * <summary>	Constructor. </summary>
 *
 * <param name="balance">	The balance. </param>
 *-----------------------------------------------------------------------------------------------**/

MyCasinoAccount::MyCasinoAccount(DOUBLE balance)
	: m_username(L""),
	m_currentBalance(balance),
	m_preliminaryBalance(0),
	m_transactionIdCounter(0),
	m_currentTransactionIterator(0)
{

}

/** <summary>	Destructor. </summary> */
MyCasinoAccount::~MyCasinoAccount()
{

	// finally delete all transaction objects
	{
		SCOPED_LOCK(transactionsMutex);

		for (auto it = m_transactions.begin(); it != m_transactions.end(); it++)
		{
			delete (*it);
		}
	}
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Serialize this object to the given stream. </summary>
 *
 * <returns>	A std::wstring. </returns>
 *-----------------------------------------------------------------------------------------------**/

std::wstring MyCasinoAccount::Serialize()
{
	std::wstring serialized;
	serialized.append(m_username);
	serialized.append(MY_CASINO_ACCOUNT_SERIALIZER_SEPARATOR_WSTRING);
	serialized.append(std::to_wstring(m_currentBalance));
	return serialized;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Deserialize this object to the given stream. </summary>
 *
 * <param name="in">	The in. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL MyCasinoAccount::Deserialize(std::wstring in)
{
	std::wstring temp;
	std::vector<std::wstring> parts;
	std::wstringstream wss(in);
	while (std::getline(wss, temp, MY_CASINO_ACCOUNT_SERIALIZER_SEPARATOR_CHAR))
		parts.push_back(temp);

	if (parts.size() != MY_CASINO_ACCOUNT_SERIALIZED_PROPERTY_COUNT)
		return E_MY_CASINO_MALFORMED_DATABASE_FILE;

	DOUBLE initialBalance = 0.0;
	try
	{
		initialBalance = stod(parts.at(1));
	}
	catch (...)
	{
		return E_MY_CASINO_MALFORMED_DATABASE_FILE;
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

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets the username. </summary>
 *
 * <returns>	The username. </returns>
 *-----------------------------------------------------------------------------------------------**/

std::wstring MyCasinoAccount::GetUsername()
{
	return m_username;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Creates a transaction. </summary>
 *
 * <param name="changeAmount"> 	The change amount. </param>
 * <param name="type">		   	The type. </param>
 * <param name="information">  	[in,out] If non-null, the information. </param>
 * <param name="infoType">	   	[in,out] If non-null, type of the information. </param>
 * <param name="transactionId">	[in,out] If non-null, identifier for the transaction. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL MyCasinoAccount::CreateTransaction(DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType, ULONG* transactionId)
{
	// apply transaction
	switch (type)
	{
	case MyCasinoTransactionsTypes::DEPOSIT:
		// deposit has to be positive
		if (changeAmount < 0.0)
			return E_MY_CASINO_INVALID_CHANGE_AMOUNT;

		{
			SCOPED_LOCK(balanceMutex);
			m_currentBalance += changeAmount;
		}
		break;
	case MyCasinoTransactionsTypes::WITHDRAWAL:
		if (changeAmount + GetCurrentBalance() < 0)
			return E_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT;

		// withdrawal has to be negativ
		if (changeAmount > 0.0)
			return E_MY_CASINO_INVALID_CHANGE_AMOUNT;
		
		{
			SCOPED_LOCK(balanceMutex);
			m_currentBalance += changeAmount;
		}
		break;
	case MyCasinoTransactionsTypes::BET_WAGER:
		if (changeAmount + GetCurrentBalance() < 0 )
			return E_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT;

		{
			SCOPED_LOCK(balanceMutex);
			m_preliminaryBalance += changeAmount;
		}
		break;
	default:
		return E_MY_CASINO_INVALID_TRANSACTION_TYPE;
	}


	MyCasinoTransaction* transaction = new MyCasinoTransaction(m_transactionIdCounter++, m_currentBalance, changeAmount);
	transaction->SetTransactionType(type, information, infoType);

	{
		SCOPED_LOCK(transactionsMutex);

		// save to transaction list
		m_transactions.push_back(transaction);
	}

	*transactionId = transaction->GetId();

	return TRUE;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Queries if we cancel transaction 'transactionId'. </summary>
 *
 * <param name="transactionId">	Identifier for the transaction. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL MyCasinoAccount::CancelTransaction(ULONG transactionId)
{
	MyCasinoTransaction* transaction = NULL;
	if (!GetTransaction(transactionId, &transaction))
		return FALSE;

	switch (transaction->GetTransactionType())
	{
	case MyCasinoTransactionsTypes::DEPOSIT:
	case MyCasinoTransactionsTypes::WITHDRAWAL:
		{
			SCOPED_LOCK(balanceMutex);
			m_currentBalance -= transaction->GetChangeAmount();
		}
		break;
	case MyCasinoTransactionsTypes::BET_WAGER:
		{
			SCOPED_LOCK(balanceMutex);
			m_preliminaryBalance -= transaction->GetChangeAmount();
		}
		break;
	default:
		return E_MY_CASINO_INVALID_TRANSACTION_TYPE;
	}

	transaction->SetTransactionType(MyCasinoTransactionsTypes::CANCELED, NULL, NULL);
	return TRUE;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets transaction information. </summary>
 *
 * <param name="transactionId">	Identifier for the transaction. </param>
 * <param name="information">  	[in,out] If non-null, the information. </param>
 * <param name="infotype">	   	[in,out] If non-null, the infotype. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL MyCasinoAccount::GetTransactionInformation(ULONG transactionId, IMyCasinoTransactionInformation** information, MyCasinoTransactionsInformationTypes* infotype)
{
	MyCasinoTransaction* transaction = NULL;
	if (!GetTransaction(transactionId, &transaction))
		return FALSE;

	*information = (*transaction).GetTransactionInformation();
	*infotype = *((*transaction).GetTransactionInformationType());

	return (NULL != *information);
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets a transaction. </summary>
 *
 * <param name="transactionId">	Identifier for the transaction. </param>
 * <param name="transaction">  	[in,out] If non-null, the transaction. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL MyCasinoAccount::GetTransaction(ULONG transactionId, MyCasinoTransaction** transaction)
{
	*transaction = NULL;
	SCOPED_LOCK(transactionsMutex);
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

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets a transaction. </summary>
 *
 * <param name="transactionInformation">	[in,out] If non-null, information describing the
 * 											transaction. </param>
 * <param name="transactionId">				[in,out] If non-null, identifier for the transaction. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL MyCasinoAccount::GetTransaction(IMyCasinoTransactionInformation* transactionInformation, ULONG* transactionId)
{
	if (NULL == transactionInformation)
		return FALSE;

	SCOPED_LOCK(transactionsMutex);

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

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets current balance. </summary>
 *
 * <returns>	The current balance. </returns>
 *-----------------------------------------------------------------------------------------------**/

DOUBLE MyCasinoAccount::GetCurrentBalance()
{
	SCOPED_LOCK(balanceMutex);
	return m_currentBalance + m_preliminaryBalance;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Change transaction. </summary>
 *
 * <param name="transactionId">	Identifier for the transaction. </param>
 * <param name="changeAmount"> 	The change amount. </param>
 * <param name="type">		   	The type. </param>
 * <param name="information">  	[in,out] If non-null, the information. </param>
 * <param name="infoType">	   	[in,out] If non-null, type of the information. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL MyCasinoAccount::ChangeTransaction(ULONG transactionId, DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType)
{
	MyCasinoTransaction* transaction = NULL;
	if (!GetTransaction(transactionId, &transaction))
		return FALSE;

	// only BET_WAGER transaction type is allowed to have a transation
	if(transaction->GetTransactionType() != MyCasinoTransactionsTypes::BET_WAGER)
		return E_MY_CASINO_INVALID_TRANSACTION_TRANSITION;

	
	transaction->SetTransactionType(type, information, infoType);	

	double additionalWageDifference = 0.0;
	double previousAmount = transaction->GetChangeAmount();
	
	// same type simply amount changed
	if (type == MyCasinoTransactionsTypes::BET_WAGER)
	{
		additionalWageDifference = -(previousAmount - changeAmount);

		if (GetCurrentBalance() - additionalWageDifference < 0)
			return E_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT;

		{
			SCOPED_LOCK(balanceMutex);
			m_preliminaryBalance += additionalWageDifference;
		}

		additionalWageDifference = changeAmount;
	}
	else if (type == MyCasinoTransactionsTypes::BET_LOSS)
	{
		if (previousAmount > 0.0 && changeAmount < 0.0)
			additionalWageDifference = changeAmount + previousAmount;
		else
			additionalWageDifference = changeAmount;


		// remove amount from preliminary and add to actual balance
		{
			SCOPED_LOCK(balanceMutex);
			if(previousAmount > 0 && m_preliminaryBalance > 0)
				m_preliminaryBalance -= previousAmount;
			else if(previousAmount < 0 && m_preliminaryBalance > 0)
				m_preliminaryBalance += previousAmount;
			else if (previousAmount < 0 && m_preliminaryBalance < 0)
				m_preliminaryBalance -= previousAmount;
			else if (previousAmount > 0 && m_preliminaryBalance < 0)
				m_preliminaryBalance += previousAmount;

			m_currentBalance += additionalWageDifference;
		}
	}
	else if (type == MyCasinoTransactionsTypes::BET_WIN)
	{
		if (previousAmount < 0.0 && changeAmount > 0.0)
			additionalWageDifference = changeAmount + previousAmount;
		else
			additionalWageDifference = changeAmount;


		// remove amount from preliminary and add to actual balance
		{
			SCOPED_LOCK(balanceMutex);
			if (previousAmount > 0 && m_preliminaryBalance > 0)
				m_preliminaryBalance -= previousAmount;
			else if (previousAmount < 0 && m_preliminaryBalance > 0)
				m_preliminaryBalance += previousAmount;
			else if (previousAmount < 0 && m_preliminaryBalance < 0)
				m_preliminaryBalance -= previousAmount;
			else if (previousAmount > 0 && m_preliminaryBalance < 0)
				m_preliminaryBalance += previousAmount;

			m_currentBalance += additionalWageDifference;
		}
	}
	else
	{
		return E_MY_CASINO_INVALID_TRANSACTION_TRANSITION;
	}

	transaction->SetChangeAmount(additionalWageDifference,m_currentBalance);
	return TRUE;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets the next transaction. </summary>
 *
 * <param name="nextTransaction">	[in,out] If non-null, the next transaction. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL MyCasinoAccount::GetNextTransaction(MyCasinoTransaction** nextTransaction)
{
	SCOPED_LOCK(transactionsMutex);

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