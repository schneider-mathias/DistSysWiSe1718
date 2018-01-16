/**--------------------------------------------------------------------------------------------------
// project:	AuthServiceLib
// file:	AuthServiceDefaultUser.cpp
//
// summary:	Implements the authentication service default user class 
//			which simply contains a username and a password.
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			21.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX  Mathias Schneider	Changed
 *-----------------------------------------------------------------------------------------------**/

#include "AuthServiceDefaultUser.h"
#include <vector>
#include <sstream>

/**--------------------------------------------------------------------------------------------------
 * <summary>	Serialize this object to a string. </summary>
 *
 * <returns>	A wstring containing the erialized object information. </returns>
 *-----------------------------------------------------------------------------------------------**/

std::wstring AuthServiceDefaultUser::Serialize()
{
	std::wstring serialized;
	serialized.append(m_username);
	serialized.append(L" ");
	serialized.append(m_password);
	return serialized;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Deserialize string and apply data to this object. </summary>
 *
 * <param name="in">	Serialized input string. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL AuthServiceDefaultUser::Deserialize(std::wstring in)
{
	std::wstring temp;
	std::vector<std::wstring> parts;
	std::wstringstream wss(in);
	while (std::getline(wss, temp, L' '))
		parts.push_back(temp);

	if (parts.size() != 2)
		return FALSE;

	m_username = parts.at(0);
	m_password = parts.at(1);

	return TRUE;
}