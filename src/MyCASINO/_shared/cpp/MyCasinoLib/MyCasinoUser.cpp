/**---------------------------------------------------------------------
// project:	MyCasinoLib
// file:	MyCasinoUser.cpp
//
// summary:	Implements my casino user class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			23.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#include "MyCasinoUser.h"
#include <vector>
#include <sstream>

/** <summary>	Default constructor. </summary> */
MyCasinoUser::MyCasinoUser()
{
}

/** <summary>	Destructor. </summary> */
MyCasinoUser::~MyCasinoUser()
{
}

/**---------------------------------------------------------------------
 * <summary>	Serialize user object to string </summary>
 *
 * <returns>	A string containing the MyCasino user information. </returns>
 *-------------------------------------------------------------------**/

std::wstring MyCasinoUser::Serialize()
{
	std::wstring serialized;
	serialized.append(std::to_wstring(m_id));
	serialized.append(MY_CASINO_USER_SERIALIZER_SEPARATOR_WSTRING);
	serialized.append(m_username);
	serialized.append(MY_CASINO_USER_SERIALIZER_SEPARATOR_WSTRING);
	serialized.append(m_password);
	serialized.append(MY_CASINO_USER_SERIALIZER_SEPARATOR_WSTRING);
	serialized.append(std::to_wstring(m_userType));
	return serialized;
}

/**---------------------------------------------------------------------
 * <summary>	Deserialize a string and applies its values to this object. </summary>
 *
 * <param name="in">	The serialized string. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

BOOL MyCasinoUser::Deserialize(std::wstring in)
{
	std::wstring temp;
	std::vector<std::wstring> parts;
	std::wstringstream wss(in);
	while (std::getline(wss, temp, MY_CASINO_USER_SERIALIZER_SEPARATOR_CHAR))
		parts.push_back(temp);

	if (parts.size() != MY_CASINO_USER_SERIALIZED_PROPERTY_COUNT)
		return FALSE;
	
	// MyCasino user structure:
	// <id> <username> <password> <userType>
	try
	{
		m_id = std::stoi(parts.at(0));
	}
	catch (...) 
	{
		return FALSE;
	}
	
	m_username = parts.at(1);
	m_password = parts.at(2);
	try
	{
		m_userType = static_cast<MyCasinoUserTypes>(std::stoi(parts.at(3)));
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

/**---------------------------------------------------------------------
 * <summary>	Gets user type. </summary>
 *
 * <returns>	The user type. </returns>
 *-------------------------------------------------------------------**/

MyCasinoUserTypes MyCasinoUser::GetUserType()
{
	return m_userType;
}

/**---------------------------------------------------------------------
 * <summary>	Query if 'userType' is user type. </summary>
 *
 * <param name="userType">	Type of the user. </param>
 *
 * <returns>	True if user type, false if not. </returns>
 *-------------------------------------------------------------------**/

BOOL MyCasinoUser::IsUserType(MyCasinoUserTypes userType)
{
	return userType == m_userType;
}

/**---------------------------------------------------------------------
 * <summary>	Equality operator. </summary>
 *
 * <param name="ref">	The reference. </param>
 *
 * <returns>	True if the parameters are considered equivalent. </returns>
 *-------------------------------------------------------------------**/

bool MyCasinoUser::operator==(const MyCasinoUser& ref) const
{
	return ref.m_id == m_id;
}

 /** <summary>	. </summary> */
bool MyCasinoUser::operator<(const MyCasinoUser& ref) const
{
	return ref.m_id < m_id;
}