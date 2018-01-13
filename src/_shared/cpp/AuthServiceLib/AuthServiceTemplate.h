/**--------------------------------------------------------------------------------------------------
// project:	AuthServiceLib
// file:	AuthServiceTemplate.h
//
// summary:	Declares the authentication service template class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#pragma once

#include <string>
#include <windows.h>
#include <WinBase.h>
#include <vector>
#include <map>
#include <type_traits>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <sstream>

#include "IAuthServiceUser.h"


template<typename TAuthServiceUser>
/** <summary>	An authentication service template. </summary> */
class CAuthServiceTemplate
{
public:

	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Constructor. </summary>
	 *
	 * <param name="parameter1">	   	The first parameter. </param>
	 * <param name="IAuthServiceUser"">	Zero-based index of the authentication service user". </param>
	 *-----------------------------------------------------------------------------------------------**/

	static_assert(std::is_base_of<IAuthServiceUser, TAuthServiceUser>::value, "T must inherit from IAuthServiceUser");

	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Constructor. </summary>
	 *
	 * <param name="userDataDirRootEnv">	[in,out] (Optional) If non-null, the user data dir root
	 * 										environment. </param>
	 *-----------------------------------------------------------------------------------------------**/

	CAuthServiceTemplate(std::wstring* userDataDirRootEnv = nullptr);
	/** <summary>	Destructor. </summary> */
	~CAuthServiceTemplate();

	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Reads registered user. </summary>
	 *
	 * <param name="filename">	Filename of the file. </param>
	 *
	 * <returns>	True if it succeeds, false if it fails. </returns>
	 *-----------------------------------------------------------------------------------------------**/

	BOOL readRegisteredUser(std::wstring filename);

	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Login. </summary>
	 *
	 * <param name="username">	The username. </param>
	 * <param name="password">	The password. </param>
	 * <param name="sessinId">	[in,out] If non-null, identifier for the sessin. </param>
	 *
	 * <returns>	True if it succeeds, false if it fails. </returns>
	 *-----------------------------------------------------------------------------------------------**/

	BOOL login(std::wstring username, std::wstring password, ULONG* sessinId);

	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Query if 'sessionId' is logged in. </summary>
	 *
	 * <param name="sessionId">	Identifier for the session. </param>
	 * <param name="user">	   	[in,out] (Optional) If non-null, the user. </param>
	 *
	 * <returns>	True if logged in, false if not. </returns>
	 *-----------------------------------------------------------------------------------------------**/

	BOOL isLoggedIn(ULONG sessionId, TAuthServiceUser** user = nullptr);

	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Query if 'username' is logged in. </summary>
	 *
	 * <param name="username">	  	The username. </param>
	 * <param name="newSessionId">	[in,out] If non-null, identifier for the new session. </param>
	 *
	 * <returns>	True if logged in, false if not. </returns>
	 *-----------------------------------------------------------------------------------------------**/

	BOOL isLoggedIn(std::wstring username, ULONG* newSessionId);

	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Logout. </summary>
	 *
	 * <param name="sessionId">	Identifier for the session. </param>
	 *
	 * <returns>	True if it succeeds, false if it fails. </returns>
	 *-----------------------------------------------------------------------------------------------**/

	BOOL logout(ULONG sessionId);

private:

	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Query if 'newSessionId' is used session identifier. </summary>
	 *
	 * <param name="newSessionId">	Identifier for the new session. </param>
	 *
	 * <returns>	True if used session identifier, false if not. </returns>
	 *-----------------------------------------------------------------------------------------------**/

	BOOL isUsedSessionId(ULONG newSessionId);

	/**--------------------------------------------------------------------------------------------------
	 * <summary>	Generates a session identifier. </summary>
	 *
	 * <returns>	The session identifier. </returns>
	 *-----------------------------------------------------------------------------------------------**/

	ULONG generateSessionId();

	/** <summary>	The user data dir root. </summary> */
	std::wstring m_userDataDirRoot;
	/** <summary>	The registered user. </summary> */
	std::vector< TAuthServiceUser > m_registeredUser;
	/** <summary>	The logged in users. </summary> */
	std::map<ULONG, TAuthServiceUser*> m_loggedInUsers;
	/** <summary>	The crit section. </summary> */
	CRITICAL_SECTION m_critSection;
};

template <class TAuthServiceUser>

/**--------------------------------------------------------------------------------------------------
 * <summary>	Constructor. </summary>
 *
 * <typeparam name="TAuthServiceUser">	Type of the authentication service user. </typeparam>
 * <param name="userDataDirRootEnv">	[in,out] If non-null, the user data dir root environment. </param>
 *-----------------------------------------------------------------------------------------------**/

CAuthServiceTemplate<TAuthServiceUser>::CAuthServiceTemplate(std::wstring* userDataDirRootEnv)
{
	// set root directory for authentification 
	// data using environment variable
	m_userDataDirRoot = L"";
	if (NULL != userDataDirRootEnv)
	{
		wchar_t *buf = nullptr;
		size_t sz = 0;
		if (_wdupenv_s(&buf, &sz, (*userDataDirRootEnv).c_str()) == 0 && buf != nullptr)
		{
			m_userDataDirRoot = std::wstring(buf);
			free(buf);
		}
	}


	srand((ULONG)time(NULL));

	InitializeCriticalSection(&m_critSection);
}

template <class TAuthServiceUser>

/**--------------------------------------------------------------------------------------------------
 * <summary>	Destructor. </summary>
 *
 * <typeparam name="TAuthServiceUser">	Type of the authentication service user. </typeparam>
 *-----------------------------------------------------------------------------------------------**/

CAuthServiceTemplate<TAuthServiceUser>::~CAuthServiceTemplate()
{
	EnterCriticalSection(&m_critSection);

	for (std::map<ULONG, TAuthServiceUser*>::iterator it = m_loggedInUsers.begin(); it != m_loggedInUsers.end(); ++it) {
		if(NULL != (it->second))
			delete (it->second);
	}

	LeaveCriticalSection(&m_critSection);
	DeleteCriticalSection(&m_critSection);

}

template <class TAuthServiceUser>

/**--------------------------------------------------------------------------------------------------
 * <summary>	Reads registered user. </summary>
 *
 * <typeparam name="TAuthServiceUser">	Type of the authentication service user. </typeparam>
 * <param name="filename">	Filename of the file. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL CAuthServiceTemplate<TAuthServiceUser>::readRegisteredUser(std::wstring filename)
{
	// open file
	std::wifstream infile;

	std::wstring filePath(m_userDataDirRoot);
	filePath.append(filename);

	infile.open(filePath);
	if (infile.fail())
	{
		return FALSE;
	}

	std::wstring wLine;
	while (std::getline(infile, wLine))
	{
		TAuthServiceUser user;
		if (!user.Deserialize(wLine))
		{
			return FALSE;
		}

		EnterCriticalSection(&m_critSection);
		m_registeredUser.push_back(user);
		LeaveCriticalSection(&m_critSection);
	}

	infile.close();

	return TRUE;
}

template <class TAuthServiceUser>

/**--------------------------------------------------------------------------------------------------
 * <summary>	Login. </summary>
 *
 * <typeparam name="TAuthServiceUser">	Type of the authentication service user. </typeparam>
 * <param name="username"> 	The username. </param>
 * <param name="password"> 	The password. </param>
 * <param name="sessionId">	[in,out] If non-null, identifier for the session. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL CAuthServiceTemplate<TAuthServiceUser>::login(std::wstring username, std::wstring password, ULONG* sessionId)
{
	// check if credentials are valid
	BOOL isValidUserName = FALSE;
	BOOL isValidPassword = FALSE;
	TAuthServiceUser* userAccount;
	for (std::vector<TAuthServiceUser>::iterator it = m_registeredUser.begin(); it != m_registeredUser.end(); ++it)
	{
		if((*it).m_username == username)
			isValidUserName = TRUE;
		if ((*it).m_password == password)
			isValidPassword = TRUE;

		if (isValidUserName && isValidPassword)
		{
			userAccount = (&(*it));
			break;
		}
			
	}
		
	if (!isValidUserName)
		return FALSE;
	if (!isValidPassword)
		return FALSE;


	// check if already logged in
	if (isLoggedIn(username, sessionId))
	{
		return TRUE;
	}

	// generate new session id

	// check if session id is already used, and regenerate if so
	do
	{
		*sessionId = generateSessionId();
	} while (isUsedSessionId(*sessionId));


	EnterCriticalSection(&m_critSection);
	// add new session id and assign sessinId value
	m_loggedInUsers.insert(std::pair<ULONG, TAuthServiceUser*>(*sessionId, userAccount));
	LeaveCriticalSection(&m_critSection);

	return TRUE;
}

template <class TAuthServiceUser>

/**--------------------------------------------------------------------------------------------------
 * <summary>	Generates a session identifier. </summary>
 *
 * <typeparam name="TAuthServiceUser">	Type of the authentication service user. </typeparam>
 *
 * <returns>	The session identifier. </returns>
 *-----------------------------------------------------------------------------------------------**/

ULONG CAuthServiceTemplate<TAuthServiceUser>::generateSessionId()
{
	if (sizeof(int) < sizeof(ULONG))
		// creates nasty warning C4293 but that's something the compiler cannot check
		return (static_cast<ULONG>(rand()) << (sizeof(int) * 8)) |
		rand();

	return rand();
}

template <class TAuthServiceUser>

/**--------------------------------------------------------------------------------------------------
 * <summary>	Query if 'username' is logged in. </summary>
 *
 * <typeparam name="TAuthServiceUser">	Type of the authentication service user. </typeparam>
 * <param name="username"> 	The username. </param>
 * <param name="sessionId">	[in,out] If non-null, identifier for the session. </param>
 *
 * <returns>	True if logged in, false if not. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL CAuthServiceTemplate<TAuthServiceUser>::isLoggedIn(std::wstring username, ULONG* sessionId)
{
	BOOL isLoggedIn = FALSE;

	// accessing logged-in user list in a thread-safe way
	EnterCriticalSection(&m_critSection);

	for (std::map<ULONG, TAuthServiceUser*>::iterator it = m_loggedInUsers.begin(); it != m_loggedInUsers.end(); ++it) {
		if ((*it->second).m_username == username)
		{
			isLoggedIn = TRUE;
			if (NULL != sessionId)
			{
				*sessionId = it->first;
			}
			break;
		}
	}

	LeaveCriticalSection(&m_critSection);

	return isLoggedIn;
}

template <class TAuthServiceUser>

/**--------------------------------------------------------------------------------------------------
 * <summary>	Query if 'newSessionId' is used session identifier. </summary>
 *
 * <typeparam name="TAuthServiceUser">	Type of the authentication service user. </typeparam>
 * <param name="newSessionId">	Identifier for the new session. </param>
 *
 * <returns>	True if used session identifier, false if not. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL CAuthServiceTemplate<TAuthServiceUser>::isUsedSessionId(ULONG newSessionId)
{
	BOOL sessionIdAlreadyUsed = FALSE;

	EnterCriticalSection(&m_critSection);

	std::map< ULONG, TAuthServiceUser*>::iterator it = m_loggedInUsers.find(newSessionId);
	if (it != m_loggedInUsers.end())
		sessionIdAlreadyUsed = TRUE;

	LeaveCriticalSection(&m_critSection);

	return sessionIdAlreadyUsed;
}

template <class TAuthServiceUser>

/**--------------------------------------------------------------------------------------------------
 * <summary>	Query if 'sessionId' is logged in. </summary>
 *
 * <typeparam name="TAuthServiceUser">	Type of the authentication service user. </typeparam>
 * <param name="sessionId">	Identifier for the session. </param>
 * <param name="user">	   	[in,out] If non-null, the user. </param>
 *
 * <returns>	True if logged in, false if not. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL CAuthServiceTemplate<TAuthServiceUser>::isLoggedIn(ULONG sessionId, TAuthServiceUser** user)
{
	BOOL isLoggedIn = FALSE;

	// accessing logged-in user list in a thread-safe way
	EnterCriticalSection(&m_critSection);
	for (std::map<ULONG, TAuthServiceUser*>::iterator it = m_loggedInUsers.begin(); it != m_loggedInUsers.end(); ++it) {
		if (it->first == sessionId)
		{
			isLoggedIn = TRUE;
			if(NULL != user)
				*user = &(*it->second);
			break;
		}
	}
	LeaveCriticalSection(&m_critSection);

	return isLoggedIn;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Logout. </summary>
 *
 * <typeparam name="TAuthServiceUser">	Type of the authentication service user. </typeparam>
 * <param name="sessionId">	Identifier for the session. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

template <class TAuthServiceUser>
BOOL CAuthServiceTemplate<TAuthServiceUser>::logout(ULONG sessionId)
{
	if (!isLoggedIn(sessionId, NULL))
	{
		// invalid session id, user is not logged-in
		return FALSE;
	}

	EnterCriticalSection(&m_critSection);
	m_loggedInUsers.erase(sessionId);
	LeaveCriticalSection(&m_critSection);

	return TRUE;
}

