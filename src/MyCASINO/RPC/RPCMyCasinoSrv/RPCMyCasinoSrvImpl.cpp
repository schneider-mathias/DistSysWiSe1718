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
