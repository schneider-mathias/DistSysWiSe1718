/**---------------------------------------------------------------------
// project:	MyCasinoLib
// file:	MyCasinoUser.h
//
// summary:	Declares MyCasino user class which stores credentials for login. 
//			Furthermore it implements (de)serialization into a plain .txt.
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			23.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#pragma once
#include <windows.h>
#include "AuthServiceLib\IAuthServiceUser.h"
#include "IncPub/MyCasinoDefines.h"

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