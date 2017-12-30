#include "MyCasino_i.h"

#include "AuthServiceWrapper.h"

#include "CharStringConverter.h"
#include "MyCasinoBet.h"
#include "MyCasinoTransaction.h"


error_status_t login(long* sessionId, unsigned char *username, unsigned char *password, unsigned short* userType)
{

	if (!getAuthService()->login(char_to_wstring((char*)username), char_to_wstring((char*)password), (ULONG*)sessionId))
	{
		return RPC_E_ACCESS_DENIED;
	}

	MyCasinoUser user;
	if (!getAuthService()->isLoggedIn(*sessionId, &user))
	{
		return RPC_E_FAULT;
	}

	*userType = user.GetUserType();

	return RPC_S_OK;
}

error_status_t logout(long sessionId)
{
	MyCasinoUser user;
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


error_status_t deposit(long sessionId, unsigned char *name, double amountMoney)
{
	return RPC_E_FAULT;
}

error_status_t bet(long sessionId, double amountMoney, unsigned short firstNumber, unsigned short secondNumber)
{
	return RPC_E_FAULT;
}


error_status_t calculateProfit(long sessionId, double amountMoney, double* profitForOneMatch, double* profitForTwoMatches)
{
	return RPC_E_FAULT;
}

error_status_t showbets(long sessionId, MyCasinoBet_t* bets, long* count)
{
	return RPC_E_FAULT;
}

// result numbers equal input
error_status_t drawTest(long sessionId, unsigned short firstNumberTest, unsigned short secondNumberTest)
{
	return RPC_E_FAULT;
}

error_status_t draw(long sessionId, unsigned short* firstNumber, unsigned short* secondNumber)
{
	return RPC_E_FAULT;
}

error_status_t getTransactions(long sessionId, boolean* isFinished, MyCasinoTransaction_t* transaction, long* transactionType)
{
	return RPC_E_FAULT;
}

error_status_t getTransactionInformation(long sessionId, long transactionId, String_t* information, long* informationType)
{
	return RPC_E_FAULT;
}

