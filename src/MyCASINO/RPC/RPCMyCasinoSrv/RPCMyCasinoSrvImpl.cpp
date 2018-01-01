#include "MyCasino_i.h"

#include "AuthServiceWrapper.h"

#include "CharStringConverter.h"
#include "MyCasinoBet.h"
#include "MyCasinoTransaction.h"


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

	return RPC_S_OK;
}

error_status_t logout(unsigned long sessionId)
{
	MyCasinoUser* user = NULL;
	if (!getAuthService()->isLoggedIn(sessionId, &user))
	{
		return RPC_E_FAULT;
	}

	// TODO: Add your implementation code here
	if (!getAuthService()->logout(sessionId))
	{
		return RPC_E_FAULT;
	}

	return RPC_S_OK;
}


error_status_t deposit(unsigned long sessionId, unsigned char *name, double amountMoney)
{
	return RPC_E_FAULT;
}

error_status_t bet(unsigned long sessionId, double amountMoney, short firstNumber, short secondNumber)
{
	return RPC_E_FAULT;
}


error_status_t calculateProfit(unsigned long sessionId, double amountMoney, double* profitForOneMatch, double* profitForTwoMatches)
{
	return RPC_E_FAULT;
}

error_status_t showbets(unsigned long sessionId, MyCasinoBet_t* bets, unsigned long* count)
{
	return RPC_E_FAULT;
}

// result numbers equal input
error_status_t drawTest(unsigned long sessionId, short firstNumberTest, short secondNumberTest)
{
	return RPC_E_FAULT;
}

error_status_t draw(unsigned long sessionId, short* firstNumber, short* secondNumber)
{
	return RPC_E_FAULT;
}

error_status_t getTransactions(unsigned long sessionId, boolean* isFinished, MyCasinoTransaction_t* transaction, unsigned long* transactionType)
{
	return RPC_E_FAULT;
}

error_status_t getTransactionInformation(unsigned long sessionId, unsigned long transactionId, String_t* information, unsigned long* informationType)
{
	return RPC_E_FAULT;
}

