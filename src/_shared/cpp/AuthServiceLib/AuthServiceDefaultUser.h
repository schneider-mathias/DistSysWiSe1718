/**--------------------------------------------------------------------------------------------------
// project:	AuthServiceLib
// file:	AuthServiceDefaultUser.h
//
// summary:	Declares the authentication service default user class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			21.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-----------------------------------------------------------------------------------------------**/

#pragma once
#include <windows.h>
#include "IAuthServiceUser.h"

class AuthServiceDefaultUser : public IAuthServiceUser 
{
public:
	AuthServiceDefaultUser() {};
	~AuthServiceDefaultUser() {};
	std::wstring Serialize();
	BOOL Deserialize(std::wstring in);
};