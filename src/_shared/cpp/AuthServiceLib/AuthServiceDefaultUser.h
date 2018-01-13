/**--------------------------------------------------------------------------------------------------
// project:	AuthServiceLib
// file:	AuthServiceDefaultUser.h
//
// summary:	Declares the authentication service default user class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
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