/**--------------------------------------------------------------------------------------------------
// project:	MyCasinoLib
// file:	MyCasinoUser.h
//
// summary:	Declares my casino user class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#pragma once
#include <windows.h>
#include "IAuthServiceUser.h"
#include "../IncPub/MyCasinoDefines.h"

#define MY_CASINO_USER_SERIALIZED_PROPERTY_COUNT 4
#define MY_CASINO_USER_SERIALIZER_SEPARATOR_WSTRING L" "
#define MY_CASINO_USER_SERIALIZER_SEPARATOR_CHAR L' '


class MyCasinoUser : public IAuthServiceUser
{
public:
	MyCasinoUser();
	~MyCasinoUser();
	std::wstring Serialize();
	BOOL Deserialize(std::wstring in);
	BOOL IsUserType(MyCasinoUserTypes userType);
	MyCasinoUserTypes GetUserType();
	bool operator==(const MyCasinoUser&) const;
	bool operator<(const MyCasinoUser&) const;

private:
	int m_id;
	MyCasinoUserTypes m_userType;

};