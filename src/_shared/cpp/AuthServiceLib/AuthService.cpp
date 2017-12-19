/********************************************************
* AuthService.h -- Class for user authentification     *
*													   *
*                                                      *
* Authors:  Mathias, Schneider						   *
*                                                      *
* Purpose:  Implementatio for AuthService class		   *
********************************************************/

#include "AuthService.h"
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <sstream>

CAuthService::CAuthService()
{
	srand((ULONG)time(NULL));

	InitializeCriticalSection(&m_critSection);
}

CAuthService::~CAuthService()
{
	DeleteCriticalSection(&m_critSection);
}


BOOL CAuthService::readRegisteredUser(std::wstring userDatabaseFilePath)
{
	// open file
	std::ifstream infile;
	infile.open(userDatabaseFilePath);
	if (infile.fail())
	{
		return FALSE;
	}

	std::string usernameString, passwordString;

	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		if (!(iss >> usernameString >> passwordString))
		{
			// error while parsing file
			return FALSE;
		}
		
		// convert to wstring
		std::wstring username(usernameString.begin(), usernameString.end());
		std::wstring password(passwordString.begin(), passwordString.end());

		// it should not happen that service is already used while initialization
		// but make it also thread-safe
		EnterCriticalSection(&m_critSection);
		m_registeredUser.insert(std::pair<std::wstring, std::wstring>(username,password));
		LeaveCriticalSection(&m_critSection);
	}

	return TRUE;
}

BOOL CAuthService::login(std::wstring username, std::wstring password, ULONG* sessionId)
{
	// check if credentials are valid
	std::map<std::wstring, std::wstring>::iterator it = m_registeredUser.find(username);
	if (it == m_registeredUser.end())
	{
		// not a valid username
		return FALSE;
	}

	if (it->second.compare(password) != 0)
	{
		// not a valid password
		return FALSE;
	}
	

	// check if already logged in
	if (isLoggedIn(username))
	{ 
		*sessionId = (m_loggedInUsers.find(username)->second);
		return TRUE;
	}

	// generate new session id
	ULONG newSessionId;

	// check if session id is already used, and regenerate if so
	do
	{
		newSessionId = generateSessionId();
	} while (isUsedSessionId(newSessionId));
	
	EnterCriticalSection(&m_critSection);
	// add new session id and assign sessinId value
	m_loggedInUsers.insert(std::pair<std::wstring, ULONG>(username, newSessionId));
	*sessionId = (m_loggedInUsers.find(username)->second);
	LeaveCriticalSection(&m_critSection);

	return TRUE;
}

ULONG CAuthService::generateSessionId()
{
	if (sizeof(int) < sizeof(ULONG))
		// creates nasty warning C4293 but that's something the compiler cannot check
		return (static_cast<ULONG>(rand()) << (sizeof(int) * 8)) |
		rand();

	return rand();
}

BOOL CAuthService::isLoggedIn(std::wstring username)
{
	BOOL isLoggedIn = FALSE;

	// accessing logged-in user list in a thread-safe way
	EnterCriticalSection(&m_critSection);
	
	std::map<std::wstring, ULONG>::iterator it = m_loggedInUsers.find(username);
	if (it != m_loggedInUsers.end())
		isLoggedIn = TRUE;

	LeaveCriticalSection(&m_critSection);

	return isLoggedIn;
}

BOOL CAuthService::isUsedSessionId(ULONG newSessionId)
{
	BOOL sessionIdAlreadyUsed = FALSE;

	EnterCriticalSection(&m_critSection);
	for (std::map<std::wstring, ULONG>::iterator it = m_loggedInUsers.begin(); it != m_loggedInUsers.end(); ++it) {
		if (it->second == newSessionId)
		{
			sessionIdAlreadyUsed = TRUE;
			break;
		}
	}
	LeaveCriticalSection(&m_critSection);

	return sessionIdAlreadyUsed;
}

BOOL CAuthService::isLoggedIn(ULONG sessionId, std::wstring *name)
{
	BOOL isLoggedIn = FALSE;

	// accessing logged-in user list in a thread-safe way
	EnterCriticalSection(&m_critSection);
	for (std::map<std::wstring, ULONG>::iterator it = m_loggedInUsers.begin(); it != m_loggedInUsers.end(); ++it) {
		if (it->second == sessionId) 
		{
			isLoggedIn = TRUE;
			if (NULL != name)
			{
				*name = (it->first);
			}
			break;
		}
	}
	LeaveCriticalSection(&m_critSection);

	return isLoggedIn;
}

BOOL CAuthService::logout(ULONG sessionId)
{
	std::wstring* username = new std::wstring;
	if (!isLoggedIn(sessionId, username))
	{
		// invalid session id, user is not logged-in
		return FALSE;
	}

	EnterCriticalSection(&m_critSection);
	m_loggedInUsers.erase(*username);
	LeaveCriticalSection(&m_critSection);
	
	delete username;

	return TRUE;
}

