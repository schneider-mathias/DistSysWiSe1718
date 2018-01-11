#pragma once
#include <string>
#include <windows.h>

// Define conform custom HRESULT code for HRESULT format, 
// for more information refer to 
// https://msdn.microsoft.com/de-de/library/windows/desktop/ms690088(v=vs.85).aspx

#define ERROR_MY_CASINO_USER_NOT_LOGGED_IN 0xA0010001L
#define ERROR_MY_CASINO_USER_PERMISSION_DENIED 0xA0010002L
#define ERROR_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN 0xA0010003L
#define ERROR_MY_CASINO_USER_LOGOUT_FAILED 0xA0010004L
#define ERROR_MY_CASINO_ACCESS_DENIED 0xA0010005L
#define ERROR_MY_CASINO_HAS_ALREADY_OPERATOR 0xA0010006L
#define ERROR_MY_CASINO_NO_OPERATOR 0xA0010007L
#define ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT 0xA0010008L
#define ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNTS 0xA0010009L
#define ERROR_MY_CASINO_CANNOT_SAVE_ACCOUNTS 0xA001000AL
#define ERROR_MY_CASINO_MALFORMED_DATABASE_FILE 0xA001000BL

#define ERROR_MY_CASINO_ACCOUNT_BALANCE_NOT_SUFFICIENT 0xA001000CL
#define ERROR_MY_CASINO_USER_ACCOUNT_BALANCE_NOT_SUFFICIENT 0xA001000DL
#define ERROR_MY_CASINO_OPERATOR_ACCOUNT_BALANCE_NOT_SUFFICIENT 0xA001000EL
#define ERROR_MY_CASINO_INVALID_TRANSACTION_TYPE 0xA001000FL
#define ERROR_MY_CASINO_INVALID_TRANSACTION_TRANSITION 0xA0010010L
#define ERROR_MY_CASINO_ACCOUNT_DEPOSIT_FAILED 0xA0010011L
#define ERROR_MY_CASINO_INVALID_CHANGE_AMOUNT 0xA0010012L

#define ERROR_MY_CASINO_BET_INVALID_NUMBER 0xA0010013L
#define ERROR_MY_CASINO_BET_ALREADY_TAKEN 0xA0010014L
#define ERROR_MY_CASINO_BET_CANNOT_DELETE 0xA0010015L

#define ERROR_MY_CASINO_TRANSACTION_INFOMRATION_NOT_AVAILABLE 0xA0010016L
#define ERROR_MY_CASINO_UNKNOWN_TRANSACTION_ID 0xA0010017L


#define INFORMATION_MY_CASINO_USER_HAS_NUMBERS 0x20010000L
#define INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN 0x20010000L

inline std::wstring translate_error_message(unsigned long errorcode)
{
	switch (errorcode)
	{
	case E_ACCESSDENIED:
		return std::wstring(L"Invalid username or password.");
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
	case ERROR_MY_CASINO_ACCESS_DENIED:
		return std::wstring(L"Invalid user name or password.");
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


inline std::wstring resolve_mycasino_code(long infocode)
{
	std::wstring x;
	if (infocode> 0) { x = translate_info_message(infocode); }
	else if (infocode == 0) { x = L""; }
	else { TRANSLATE_MYCASINO_ERRORCODE(x, infocode); }
	return x;
}

#define TRANSLATE_MYCASINO_CODE(x, retVal) \
	x = resolve_mycasino_code(retVal)

	
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


