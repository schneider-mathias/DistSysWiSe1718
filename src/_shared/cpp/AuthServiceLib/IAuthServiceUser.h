/**--------------------------------------------------------------------------------------------------
// project:	AuthServiceLib
// file:	IAuthServiceUser.h
//
// summary:	Declares the IAuthServiceUser interface
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#pragma once
#include <windows.h>
#include <string>


/** <summary>	Abstract class for AuthService user.
*				Contains username and passworts. </summary> */
class IAuthServiceUser {
public:
	virtual std::wstring Serialize() = 0;
	virtual BOOL Deserialize(std::wstring in) = 0;

	std::wstring m_username;
	std::wstring m_password;
};

