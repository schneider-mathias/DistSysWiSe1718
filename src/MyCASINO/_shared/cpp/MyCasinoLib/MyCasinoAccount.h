/**--------------------------------------------------------------------------------------------------
// project:	MyCasinoLib
// file:	MyCasinoAccount.h
//
// summary:	Declares thread-safe account class which keeps track of 
//			user account balance. Balance can be serialized to file, single 
//			transactions are not persistent. Consistency has to be ensured by
//			user of this class.
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			24.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-----------------------------------------------------------------------------------------------**/

#pragma once
#include <windows.h>
#include <list>
#include <mutex>
#include <atomic>

#include "MyCasinoTransaction.h"

#define MY_CASINO_ACCOUNT_SERIALIZED_PROPERTY_COUNT 2
#define MY_CASINO_ACCOUNT_SERIALIZER_SEPARATOR_WSTRING L" "
#define MY_CASINO_ACCOUNT_SERIALIZER_SEPARATOR_CHAR L' '


class MyCasinoAccount
{
public:
	MyCasinoAccount(DOUBLE balance = 0.0);
	~MyCasinoAccount();
	std::wstring Serialize();
	BOOL Deserialize(std::wstring in);

	std::wstring GetUsername();

	BOOL CreateTransaction(DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType, ULONG* transactionId);
	DOUBLE GetCurrentBalance();
	
	BOOL GetTransaction(IMyCasinoTransactionInformation* transactionInformation, ULONG* transactionId);

	BOOL ChangeTransaction(ULONG transactionId, DOUBLE changeAmount, MyCasinoTransactionsTypes type, IMyCasinoTransactionInformation* information, MyCasinoTransactionsInformationTypes* infoType);
	BOOL GetTransactionInformation(ULONG transactionId, IMyCasinoTransactionInformation** information, MyCasinoTransactionsInformationTypes* infotype);
	BOOL CancelTransaction(ULONG transactionId);
	
	BOOL GetNextTransaction(MyCasinoTransaction** nextTransaction);

private:
	BOOL GetTransaction(ULONG transactionId, MyCasinoTransaction** transaction);
	std::wstring m_username;
	DOUBLE m_currentBalance;
	DOUBLE m_preliminaryBalance;
	std::atomic<ULONG> m_transactionIdCounter;
	int m_currentTransactionIterator;
	std::vector<MyCasinoTransaction*> m_transactions;
	std::vector<MyCasinoTransaction*> m_sortedTransactions;
	std::mutex transactionsMutex;
	std::mutex balanceMutex;

};