/**--------------------------------------------------------------------------------------------------
// project:	-
// file:	MyCasinoDefines.h
//
// summary:	Common defines and macros which are shared between client 
//			and server.
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#pragma once
#include <string>
#include <windows.h>

// Define conform custom HRESULT code for HRESULT format, 
// for more information refer to 
// https://msdn.microsoft.com/de-de/library/windows/desktop/ms690088(v=vs.85).aspx

#define E_MY_CASINO_USER_NOT_LOGGED_IN 0xA0010001L
#define E_MY_CASINO_USER_PERMISSION_DENIED 0xA0010002L
#define E_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN 0xA0010003L
#define E_MY_CASINO_USER_LOGOUT_FAILED 0xA0010004L
#define E_MY_CASINO_ACCESS_DENIED 0xA0010005L
#define E_MY_CASINO_HAS_ALREADY_OPERATOR 0xA0010006L
#define E_MY_CASINO_NO_OPERATOR 0xA0010007L
#define E_MY_CASINO_CANNOT_LOAD_ACCOUNT 0xA0010008L
#define E_MY_CASINO_CANNOT_LOAD_ACCOUNTS 0xA0010009L
#define E_MY_CASINO_CANNOT_SAVE_ACCOUNTS 0xA001000AL
#define E_MY_CASINO_MALFORMED_DATABASE_FILE 0xA001000BL
#define E_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT 0xA001000CL
#define E_MY_CASINO_USER_ACCOUNT_BALANCE_NOT_SUFFICIENT 0xA001000DL
#define E_MY_CASINO_OPERATOR_ACCOUNT_BALANCE_NOT_SUFFICIENT 0xA001000EL
#define E_MY_CASINO_INVALID_TRANSACTION_TYPE 0xA001000FL
#define E_MY_CASINO_INVALID_TRANSACTION_TRANSITION 0xA0010010L
#define E_MY_CASINO_ACCOUNT_DEPOSIT_FAILED 0xA0010011L
#define E_MY_CASINO_INVALID_CHANGE_AMOUNT 0xA0010012L
#define E_MY_CASINO_BET_INVALID_NUMBER 0xA0010013L
#define E_MY_CASINO_BET_ALREADY_TAKEN 0xA0010014L
#define E_MY_CASINO_BET_CANNOT_DELETE 0xA0010015L
#define E_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE 0xA0010016L
#define E_MY_CASINO_UNKNOWN_TRANSACTION_ID 0xA0010017L
#define E_MY_CASINO_INTERNAL_AUTH_SERVICE_ERROR 0xA0010018L
#define E_MY_CASINO_INVALID_OPERATOR 0xA0010019L
#define E_MY_CASINO_INTERNAL_LOGOUT_ERROR 0xA001001AL
#define S_MY_CASINO_USER_HAS_NUMBERS 0x20010000L
#define S_MY_CASINO_NO_OPERATOR_LOGGED_IN 0x20010001L

/**--------------------------------------------------------------------------------------------------
 * <summary>	Translate MyCasino error code to message. </summary>
 *
 * <param name="errorcode">	The errorcode. </param>
 *
 * <returns>	A verbose explanation for the given errorcode. </returns>
 *-----------------------------------------------------------------------------------------------**/

inline std::wstring translate_error_message(unsigned long errorcode)
{
	switch (errorcode)
	{
	case E_MY_CASINO_USER_NOT_LOGGED_IN:
		return std::wstring(L"User is not logged in.");
	case E_MY_CASINO_USER_PERMISSION_DENIED:
		return std::wstring(L"User permissions not sufficient.");
	case E_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN:
		return std::wstring(L"User for deposit is not logged in.");
	case E_MY_CASINO_USER_LOGOUT_FAILED:
		return std::wstring(L"Error during logout.");
	case E_MY_CASINO_HAS_ALREADY_OPERATOR:
		return std::wstring(L"Current casino server already has an logged in operator.");
	case E_MY_CASINO_ACCESS_DENIED:
		return std::wstring(L"Invalid user name or password.");
	case E_MY_CASINO_NO_OPERATOR:
		return std::wstring(L"Current casino server has no logged in operator.");
	case E_MY_CASINO_CANNOT_LOAD_ACCOUNT:
		return std::wstring(L"Account could not be loaded for this user.");
	case E_MY_CASINO_CANNOT_LOAD_ACCOUNTS:
		return std::wstring(L"Accounts could not be loaded from database.");
	case E_MY_CASINO_CANNOT_SAVE_ACCOUNTS:
		return std::wstring(L"Accounts could not be saved to database.");
	case E_MY_CASINO_MALFORMED_DATABASE_FILE:
		return std::wstring(L"Database is corrupted.");
	case E_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT:
		return std::wstring(L"Account balance is not sufficient for this bet.");
	case E_MY_CASINO_USER_ACCOUNT_BALANCE_NOT_SUFFICIENT:
		return std::wstring(L"Gamer account balance is not sufficient for this bet.");
	case E_MY_CASINO_OPERATOR_ACCOUNT_BALANCE_NOT_SUFFICIENT:
		return std::wstring(L"Operator account balance is not sufficient for this bet.");
	case E_MY_CASINO_INVALID_TRANSACTION_TYPE:
		return std::wstring(L"Invalid transaction type for this operation.");
	case E_MY_CASINO_INVALID_TRANSACTION_TRANSITION:
		return std::wstring(L"Invalid transaction transition.");
	case E_MY_CASINO_ACCOUNT_DEPOSIT_FAILED:
		return std::wstring(L"Deposit transaction failed.");
	case E_MY_CASINO_INVALID_CHANGE_AMOUNT:
		return std::wstring(L"Change amount is not valid.");
	case E_MY_CASINO_BET_INVALID_NUMBER:
		return std::wstring(L"Given numbers for bet are not valid.");
	case E_MY_CASINO_BET_ALREADY_TAKEN:
		return std::wstring(L"Number pair is already taken by another user.");
	case E_MY_CASINO_BET_CANNOT_DELETE:
		return std::wstring(L"Bet cannot be deleted.");
	case E_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE:
		return std::wstring(L"No transaction details available.");
	case E_MY_CASINO_UNKNOWN_TRANSACTION_ID:
		return std::wstring(L"Transaction id cannot be resolved.");
	case E_MY_CASINO_INTERNAL_AUTH_SERVICE_ERROR:
		return std::wstring(L"Internal server error in getAuthService().");
	case E_MY_CASINO_INVALID_OPERATOR:
		return std::wstring(L"Invalid operator user wants to interact.");
	case E_MY_CASINO_INTERNAL_LOGOUT_ERROR:
		return std::wstring(L"Internal server error during logout.");
	default:
		return std::wstring(L"Unknow error: ").append(std::to_wstring(errorcode));
	}
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Translate MyCasino information code to message. </summary>
 *
 * <param name="infocode">	The infocode. </param>
 *
 * <returns>	A verbose explanation for the given info code.</returns>
 *-----------------------------------------------------------------------------------------------**/

inline std::wstring translate_info_message(unsigned long infocode)
{
	switch (infocode)
	{
	case S_MY_CASINO_USER_HAS_NUMBERS:
		return std::wstring(L"User already bets on this number pair.");
	case S_MY_CASINO_NO_OPERATOR_LOGGED_IN:
		return std::wstring(L"No operator is logged in.");
	default:
		return std::wstring(L"");
	}
}

#define TRANSLATE_MYCASINO_ERRORCODE(x, retVal) \
	x = (FAILED(retVal))? translate_error_message(retVal):L"Success"

/**--------------------------------------------------------------------------------------------------
 * <summary>	Resolve any MyCasino code. </summary>
 *
 * <param name="infocode">	The code to resolve. </param>
 *
 * <returns>	A verbose explanation for the given code. </returns>
 *-----------------------------------------------------------------------------------------------**/

inline std::wstring resolve_mycasino_code(long infocode)
{
	std::wstring x;
	if (infocode> 0) { x = translate_info_message(infocode); }
	else if (infocode == 0) { x = L""; }
	else { TRANSLATE_MYCASINO_ERRORCODE(x, infocode); }
	return x;
}

// macro for translating MyCasino codes
#define TRANSLATE_MYCASINO_CODE(x, retVal) \
	x = resolve_mycasino_code(retVal)


// number of properties of bet class
#define BET_DETAILS_PROPTERY_COUNT 4
// number of detailed properties of bet class
#define BET_FULL_DETAILS_PROPTERY_COUNT  7

// number of properties of transaction class
#define TRANSACTION_PROPTERY_COUNT 3

// upper limit for money numbers
#define UPPER_MONEY_BOUNDARY 1e12

/** <summary>	Values that represent my casino transactions information types. </summary> */
enum MyCasinoTransactionsInformationTypes
{
	Bet = 0
};

/** <summary>	Values that represent my casino transactions types. </summary> */
enum MyCasinoTransactionsTypes
{
	DEPOSIT = 0,
	WITHDRAWAL,
	CANCELED,
	BET_WAGER,
	BET_WIN,
	BET_LOSS
};

/**--------------------------------------------------------------------------------------------------
 * <summary>	Resolve transaction type. </summary>
 *
 * <param name="type">	The type as enum </param>
 *
 * <returns>	A string version of enumeration. </returns>
 *-----------------------------------------------------------------------------------------------**/

inline std::wstring resolve_transaction_type(MyCasinoTransactionsTypes type)
{
	switch (type)
	{
	case MyCasinoTransactionsTypes::DEPOSIT:
		return std::wstring(L"Deposit");
	case MyCasinoTransactionsTypes::WITHDRAWAL:
		return std::wstring(L"Withdrawal");
	case MyCasinoTransactionsTypes::CANCELED:
		return std::wstring(L"Canceled");
	case MyCasinoTransactionsTypes::BET_WAGER:
		return std::wstring(L"Bet wager");
	case MyCasinoTransactionsTypes::BET_WIN:
		return std::wstring(L"Bet won");
	case MyCasinoTransactionsTypes::BET_LOSS:
		return std::wstring(L"Bet loss");
	default:
		return std::wstring(L"Unknown transaction type");
	}
}

/** <summary>	Values that represent my casino user types. </summary> */
enum MyCasinoUserTypes {
	Operator = 0,
	Gamer, 
	Any
};


