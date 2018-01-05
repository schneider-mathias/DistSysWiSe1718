#pragma once
#include <string>

#define _MY_CASINO_TYPEDEF_(_sc) ((long)_sc)

typedef unsigned long MY_CASINO_RES;

#define ERROR_MY_CASINO_USER_NOT_LOGGED_IN 0x80110001L
#define ERROR_MY_CASINO_USER_PERMISSION_DENIED 0x80110002L
#define ERROR_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN 0x80110003L
#define ERROR_MY_CASINO_USER_LOGOUT_FAILED 0x80110004L

#define ERROR_MY_CASINO_HAS_ALREADY_OPERATOR 0x80010001L
#define ERROR_MY_CASINO_NO_OPERATOR 0x80010002L
#define ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT 0x80010003L
#define ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNTS 0x80010004L
#define ERROR_MY_CASINO_CANNOT_SAVE_ACCOUNTS 0x80010005L

#define ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT 0x80020001L
#define ERROR_MY_CASINO_USER_ACCOUNT_BALANCE_NOT_SUFFICIENT 0x80020002L
#define ERROR_MY_CASINO_OPERATOR_ACCOUNT_BALANCE_NOT_SUFFICIENT 0x80020003L
#define ERROR_MY_CASINO_INVALID_TRANSACTION_TYPE 0x80020004L
#define ERROR_MY_CASINO_INVALID_TRANSACTION_TRANSITION 0x80020005L
#define ERROR_MY_CASINO_ACCOUNT_DEPOSIT_FAILED 0x80020006L
#define ERROR_MY_CASINO_INVALID_CHANGE_AMOUNT 0x80020007L

#define ERROR_MY_CASINO_BET_INVALID_NUMBER 0x80030004L
#define ERROR_MY_CASINO_BET_ALREADY_TAKEN 0x80030005L
#define ERROR_MY_CASINO_BET_CANNOT_DELETE 0x80030006L

#define ERROR_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE 0x80040001L
#define ERROR_MY_CASINO_UNKNOWN_TRANSACTION_ID 0x80040002L


#define INFORMATION_MY_CASINO_USER_HAS_NUMBERS 0x00021001
#define INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN 0x00021002

inline std::wstring translate_error_message(unsigned long errorcode)
{
	switch (errorcode)
	{
	case ERROR_MY_CASINO_USER_NOT_LOGGED_IN:
		return std::wstring(L"User is not logged in.");
	case ERROR_MY_CASINO_USER_PERMISSION_DENIED:
		return std::wstring(L"User permissions not sufficient.");
	case ERROR_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN:
		return std::wstring(L"User for deposit is not logged in.");
	case ERROR_MY_CASINO_USER_LOGOUT_FAILED:
		return std::wstring(L"Error during logout.");
	case ERROR_MY_CASINO_HAS_ALREADY_OPERATOR:
		return std::wstring(L"Current casino server already has an logged in operator.");
	case ERROR_MY_CASINO_NO_OPERATOR:
		return std::wstring(L"Current casino server has no logged in operator.");
	case ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT:
		return std::wstring(L"Account could not be loaded for this user.");
	case ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNTS:
		return std::wstring(L"Accounts could not be loaded from database.");
	case ERROR_MY_CASINO_CANNOT_SAVE_ACCOUNTS:
		return std::wstring(L"Accounts could not be saved to database.");
	case ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT:
		return std::wstring(L"Account balance is not sufficient for this bet.");
	case ERROR_MY_CASINO_USER_ACCOUNT_BALANCE_NOT_SUFFICIENT:
		return std::wstring(L"Gamer account balance is not sufficient for this bet.");
	case ERROR_MY_CASINO_OPERATOR_ACCOUNT_BALANCE_NOT_SUFFICIENT:
		return std::wstring(L"Operator account balance is not sufficient for this bet.");
	case ERROR_MY_CASINO_INVALID_TRANSACTION_TYPE:
		return std::wstring(L"Invalid transaction type for this operation.");
	case ERROR_MY_CASINO_INVALID_TRANSACTION_TRANSITION:
		return std::wstring(L"Invalid transaction transition.");
	case ERROR_MY_CASINO_ACCOUNT_DEPOSIT_FAILED:
		return std::wstring(L"Deposit transaction failed.");
	case ERROR_MY_CASINO_BET_INVALID_NUMBER:
		return std::wstring(L"Given numbers for bet are not valid.");
	case ERROR_MY_CASINO_BET_ALREADY_TAKEN:
		return std::wstring(L"Number pair is already taken by another user.");
	case ERROR_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE:
		return std::wstring(L"No transaction details available.");
	case ERROR_MY_CASINO_UNKNOWN_TRANSACTION_ID:
		return std::wstring(L"Transaction id cannot be resolved.");
	default:
		return std::wstring(L"Unknow error: ").append(std::to_wstring(errorcode));
	}
}

inline std::wstring translate_info_message(unsigned long infocode)
{
	switch (infocode)
	{
	case INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN:
		return std::wstring(L"No operator is logged in.");
	default:
		return std::wstring(L"");
	}
}

#define TRANSLATE_MYCASINO_ERRORCODE(x, retVal) \
	x = (FAILED(retVal))? translate_error_message(retVal):L"Success"


#define TRANSLATE_MYCASINO_CODE(x, retVal) \
	if (retVal > 0) { x = translate_info_message(retVal); } else if (retVal == 0) { x = L""; } else { TRANSLATE_MYCASINO_ERRORCODE(x, retVal); }

	
#define BET_DETAILS_PROPTERY_COUNT 4
#define BET_FULL_DETAILS_PROPTERY_COUNT  7

#define TRANSACTION_PROPTERY_COUNT 3

enum MyCasinoTransactionsInformationTypes
{
	Bet = 0
};

enum MyCasinoTransactionsTypes
{
	DEPOSIT = 0,
	WITHDRAWAL,
	CANCELED,
	BET_WAGER,
	BET_WIN,
	BET_LOSS
};

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

enum MyCasinoUserTypes {
	Operator = 0,
	Gamer, 
	Any
};


