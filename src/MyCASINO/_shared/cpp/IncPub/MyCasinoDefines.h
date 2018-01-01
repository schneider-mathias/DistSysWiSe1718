#pragma once
#include <string>

#define _MY_CASINO_TYPEDEF_(_sc) ((long)_sc)

typedef unsigned long MY_CASINO_RES;

#define ERROR_MY_CASINO_USER_NOT_LOGGED_IN 0x80110001L
#define ERROR_MY_CASINO_USER_PERMISSION_DENIED 0x80110002L
#define ERROR_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN 0x80110003L

#define ERROR_MY_CASINO_HAS_ALREADY_OPERATOR 0x80010001L
#define ERROR_MY_CASINO_NO_OPERATOR 0x80010002L
#define ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT 0x80010003L

#define ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT 0x80020001L
#define ERROR_MY_CASINO_USER_ACCOUNT_BALANCE_NOT_SUFFICIENT 0x80020002L
#define ERROR_MY_CASINO_OPERATOR_ACCOUNT_BALANCE_NOT_SUFFICIENT 0x80020003L
#define ERROR_MY_CASINO_INVALID_TRANSACTION_TYPE 0x80020004L
#define ERROR_MY_CASINO_ACCOUNT_DEPOSIT_FAILED 0x80020005L

#define ERROR_MY_CASINO_BET_INVALID_NUMBER 0x80030004L
#define ERROR_MY_CASINO_BET_ALREADY_TAKEN 0x80030005L
#define ERROR_MY_CASINO_BET_CANNOT_DELETE 0x80030006L

#define INFORMATION_MY_CASINO_USER_HAS_NUMBERS 0x00021001

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
	case ERROR_MY_CASINO_HAS_ALREADY_OPERATOR:
		return std::wstring(L"Current casino server already has an logged in operator.");
	case ERROR_MY_CASINO_NO_OPERATOR:
		return std::wstring(L"Current casino server has no logged in operator.");
	case ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT:
		return std::wstring(L"Account could not be loaded for this user.");
	case ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT:
		return std::wstring(L"Account balance is not sufficient for this bet.");
	case ERROR_MY_CASINO_USER_ACCOUNT_BALANCE_NOT_SUFFICIENT:
		return std::wstring(L"Gamer account balance is not sufficient for this bet.");
	case ERROR_MY_CASINO_OPERATOR_ACCOUNT_BALANCE_NOT_SUFFICIENT:
		return std::wstring(L"Operator account balance is not sufficient for this bet.");
	case ERROR_MY_CASINO_INVALID_TRANSACTION_TYPE:
		return std::wstring(L"Invalid transaction type for this operation.");
	case ERROR_MY_CASINO_ACCOUNT_DEPOSIT_FAILED:
		return std::wstring(L"Deposit transaction failed.");
	case ERROR_MY_CASINO_BET_INVALID_NUMBER:
		return std::wstring(L"Given numbers for bet are not valid.");
	case ERROR_MY_CASINO_BET_ALREADY_TAKEN:
		return std::wstring(L"Number pair is already taken by another user.");
	default:
		return std::wstring(L"Unknow error: ").append(std::to_wstring(errorcode));
	}
}

#define TRANSLATE_MYCASINO_ERRORCODE(x, retVal) \
	x = (FAILED(retVal))? translate_error_message(retVal):L"Success"

	
#define BET_DETAILS_PROPTERY_COUNT 3
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
	PRELIMINARY_WITHDRAWAL,
	BET
};

enum MyCasinoUserTypes {
	Operator = 0,
	Gamer
};


