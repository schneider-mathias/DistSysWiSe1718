/**---------------------------------------------------------------------
// project:	MyCasinoLib
// file:	MyCasinoTransaction.cpp
//
// summary:	Implements my casino transaction class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			25.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#include "ScopedLock.h"
#include "MyCasinoTransaction.h"

/**---------------------------------------------------------------------
 * <summary>	Constructor. </summary>
 *
 * <param name="id">		   	The identifier. </param>
 * <param name="resultBalance">	The result balance. </param>
 * <param name="changeAmount"> 	The change amount. </param>
 *-------------------------------------------------------------------**/

MyCasinoTransaction::MyCasinoTransaction(ULONG id, DOUBLE resultBalance, DOUBLE changeAmount)
	:m_id(id),
	m_resultBalance(resultBalance),
	m_changeAmount(changeAmount),
	m_pTransactionInformationType(NULL),
	m_pTransactionDetails(NULL)
{
}

/** <summary>	Destructor. </summary> */
MyCasinoTransaction::~MyCasinoTransaction()
{
	SCOPED_LOCK(m_transactionMutex);

	if (NULL != m_pTransactionInformationType)
	{
		delete m_pTransactionInformationType;
		m_pTransactionInformationType = NULL;
	}

	// ownership has MyCasino so do not delete
	m_pTransactionDetails = NULL;
}

/**---------------------------------------------------------------------
 * <summary>	Gets the identifier. </summary>
 *
 * <returns>	The identifier. </returns>
 *-------------------------------------------------------------------**/

ULONG MyCasinoTransaction::GetId()
{
	return m_id;
}

/**---------------------------------------------------------------------
 * <summary>	Gets result balance. </summary>
 *
 * <returns>	The result balance. </returns>
 *-------------------------------------------------------------------**/

DOUBLE MyCasinoTransaction::GetResultBalance()
{
	SCOPED_LOCK(m_transactionMutex);

	return m_resultBalance;
}

/**---------------------------------------------------------------------
 * <summary>	Gets change amount. </summary>
 *
 * <returns>	The change amount. </returns>
 *-------------------------------------------------------------------**/

DOUBLE MyCasinoTransaction::GetChangeAmount()
{
	SCOPED_LOCK(m_transactionMutex);

	return m_changeAmount;
}

/**---------------------------------------------------------------------
 * <summary>	Sets change amount. </summary>
 *
 * <param name="changeAmount"> 	The change amount. </param>
 * <param name="resultBalance">	The result balance. </param>
 *-------------------------------------------------------------------**/

void MyCasinoTransaction::SetChangeAmount(DOUBLE changeAmount, DOUBLE resultBalance)
{
	SCOPED_LOCK(m_transactionMutex);

	m_changeAmount = changeAmount;
	m_resultBalance = resultBalance;
}

/**---------------------------------------------------------------------
 * <summary>	Gets transaction type. </summary>
 *
 * <returns>	The transaction type. </returns>
 *-------------------------------------------------------------------**/

MyCasinoTransactionsTypes MyCasinoTransaction::GetTransactionType()
{
	return m_transactionType;
}

/**---------------------------------------------------------------------
 * <summary>	Sets transaction type and its correspnding information 
 * 				(and information type). </summary>
 *
 * <param name="type">					The type. </param>
 * <param name="transactionDetails">	[in,out] If non-null, the transaction details. </param>
 * <param name="infoType">				[in,out] If non-null, type of the information. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

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

/**---------------------------------------------------------------------
 * <summary>	Gets transaction information. </summary>
 *
 * <returns>	Null if it fails, else the transaction information. </returns>
 *-------------------------------------------------------------------**/

IMyCasinoTransactionInformation* MyCasinoTransaction::GetTransactionInformation()
{
	SCOPED_LOCK(m_transactionMutex);

	return m_pTransactionDetails;
}

/**---------------------------------------------------------------------
 * <summary>	Gets transaction information type. </summary>
 *
 * <returns>	Null if it fails, else the transaction information type. </returns>
 *-------------------------------------------------------------------**/

MyCasinoTransactionsInformationTypes* MyCasinoTransaction::GetTransactionInformationType()
{
	SCOPED_LOCK(m_transactionMutex);

	return m_pTransactionInformationType;
}