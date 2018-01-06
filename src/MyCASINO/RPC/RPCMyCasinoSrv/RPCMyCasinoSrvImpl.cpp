#include "MyCasino_i.h"

#include "AuthServiceWrapper.h"
#include "MyCasinoWrapper.h"

#include "CharStringConverter.h"
#include "MyCasinoBet.h"
#include "MyCasinoTransaction.h"

#include "MyCasinoDefines.h"


error_status_t login(unsigned long*  sessionId, unsigned char *username, unsigned char *password, short* userType)
{

	if (!getAuthService()->login(char_to_wstring((char*)username), char_to_wstring((char*)password), sessionId))
	{
		return RPC_E_ACCESS_DENIED;
	}

	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(*sessionId, &user))
	{
		return RPC_E_FAULT;
	}

	*userType = user->GetUserType();
	if (*userType == MyCasinoUserTypes::Operator)
	{
		BOOL retVal = getCasino()->Open(user);
		if (FAILED(retVal))
		{
			return RPC_E_FAULT;
		}
	}

	BOOL resVal = RPC_S_OK;
	if (!getCasino()->IsOpened())
		resVal = INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN;

	return resVal;
}

error_status_t logout(unsigned long sessionId)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
	{
		return ERROR_MY_CASINO_USER_NOT_LOGGED_IN;
	}

	// check if user is operator and close casino if it is the current operator
	if (user->GetUserType() == MyCasinoUserTypes::Operator)
	{
		if (!getCasino()->IsOperator(*user))
			return ERROR_MY_CASINO_USER_NOT_LOGGED_IN;

		getCasino()->Close();
	}

	if (!getAuthService()->logout(sessionId))
	{
		return ERROR_MY_CASINO_USER_LOGOUT_FAILED;
	}

	BOOL resVal = S_OK;
	if (!getCasino()->IsOpened())
		resVal = INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN;

	return resVal;
}


error_status_t deposit(unsigned long sessionId, unsigned char *name, double amountMoney)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return ERROR_MY_CASINO_USER_NOT_LOGGED_IN;

	if (!getCasino()->IsOperator(*user))
		return ERROR_MY_CASINO_USER_PERMISSION_DENIED;

	ULONG sessionIdForDeposit = 0;
	getAuthService()->isLoggedIn(char_to_wstring((char*)name), &sessionIdForDeposit);

	// get user for deposit
	MyCasinoUser* userForDeposit = NULL;
	if (!getAuthService()->isLoggedIn(sessionIdForDeposit, &userForDeposit))
		return ERROR_MY_CASINO_USER_FOR_DEPOSIT_NOT_LOGGED_IN;

	BOOL resVal = getCasino()->Deposit(*userForDeposit, amountMoney);
	if (FAILED(resVal))
		return ERROR_MY_CASINO_ACCOUNT_DEPOSIT_FAILED;

	return RPC_S_OK;
}

error_status_t bet(unsigned long sessionId, double amountMoney, short firstNumber, short secondNumber)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return ERROR_MY_CASINO_USER_NOT_LOGGED_IN;

	if (!getCasino()->IsOpened())
		return ERROR_MY_CASINO_NO_OPERATOR;

	// create the bet
	BOOL retVal = getCasino()->Bet(*user, firstNumber, secondNumber, amountMoney);
	if (FAILED(retVal))
		return retVal;

	return RPC_S_OK;
}


error_status_t calculateProfit(unsigned long sessionId, double amountMoney, short firstNumber, short secondNumber, double* profitForOneMatch, double* profitForTwoMatches)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return ERROR_MY_CASINO_USER_NOT_LOGGED_IN;

	// create a dummy bet object in order to calculate profits
	MyCasinoBet dummyBet(user->m_username, 1, firstNumber, secondNumber, amountMoney);
	getCasino()->CalculateProfit(dummyBet, profitForOneMatch, profitForTwoMatches);

	return RPC_S_OK;
}

error_status_t showbets(unsigned long sessionId, MyCasinoBet_t** bets, unsigned long* count)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return ERROR_MY_CASINO_USER_NOT_LOGGED_IN;

	std::vector<MyCasinoBet*> betsSnapshot = getCasino()->GetBets();
	*count = betsSnapshot.size();

	// midl allocate MyCasinoBet_t array (has to be freed by client)
	(*bets) = (MyCasinoBet_t*)midl_user_allocate((*count) * sizeof(MyCasinoBet_t));

	// fill array
	for (int i = 0; i<(*count); i++)
	{
		(*bets)[i].firstNumber = betsSnapshot.at(i)->GetFirstNumber();
		(*bets)[i].secondNumber = betsSnapshot.at(i)->GetSecondNumber();
		(*bets)[i].wager = betsSnapshot.at(i)->GetSetAmount();

		// allocate memory for String_t which is contained in MyCasinoBet_t
		unsigned short stringSize = strlen((char*)wstring_to_char(betsSnapshot.at(i)->GetUsername())) + 1;

		// copy value
		(*bets)[i].name.str = (unsigned char*)midl_user_allocate(stringSize);
		(*bets)[i].name.size = stringSize;
		(*bets)[i].name.len = stringSize;
		strcpy((char*)(*bets)[i].name.str, (char*)wstring_to_char(betsSnapshot.at(i)->GetUsername()));
	}

	BOOL resVal = S_OK;
	if (!getCasino()->IsOpened())
		resVal = INFORMATION_MY_CASINO_NO_OPERATOR_LOGGED_IN;


	return resVal;
}

// result numbers equal input
error_status_t drawTest(unsigned long sessionId, short firstNumberTest, short secondNumberTest)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return ERROR_MY_CASINO_USER_NOT_LOGGED_IN;

	if (!getCasino()->IsOperator(*user))
		return ERROR_MY_CASINO_USER_PERMISSION_DENIED;

	short *drawnFirstNumber = new short(firstNumberTest);
	short *drawnSecondNumber = new short(secondNumberTest);

	BOOL hr = getCasino()->Draw(&drawnFirstNumber, &drawnSecondNumber);
	if (FAILED(hr))
		return hr;

	delete drawnFirstNumber;
	delete drawnSecondNumber;


	return RPC_S_OK;
}

error_status_t draw(unsigned long sessionId, short* firstNumber, short* secondNumber)
{
	std::wstring errCode;

	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
		return ERROR_MY_CASINO_USER_NOT_LOGGED_IN;

	if (!getCasino()->IsOperator(*user))
		return ERROR_MY_CASINO_USER_PERMISSION_DENIED;

	short *drawnFirstNumber = NULL;
	short *drawnSecondNumber = NULL;

	BOOL hr = getCasino()->Draw(&drawnFirstNumber, &drawnSecondNumber);
	if (FAILED(hr))
		return hr;

	*firstNumber = *drawnFirstNumber;
	*secondNumber = *drawnSecondNumber;

	delete drawnFirstNumber;
	delete drawnSecondNumber;

	return RPC_S_OK;
}

error_status_t getTransactions(unsigned long sessionId, boolean* isFinished, MyCasinoTransaction_t* transaction, unsigned long* transactionType)
{
	return RPC_E_FAULT;
}

error_status_t getTransactionInformation(unsigned long sessionId, unsigned long transactionId, String_t* information, unsigned long* informationType)
{
	return RPC_E_FAULT;
}

