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
class CAuthServiceTemplate
{
public:
	static_assert(std::is_base_of<IAuthServiceUser, TAuthServiceUser>::value, "T must inherit from IAuthServiceUser");

	CAuthServiceTemplate(std::wstring* userDataDirRootEnv = nullptr);
	~CAuthServiceTemplate();
	BOOL readRegisteredUser(std::wstring filename);
	BOOL login(std::wstring username, std::wstring password, ULONG* sessinId);
	BOOL isLoggedIn(ULONG sessionId, TAuthServiceUser** user = nullptr);
	BOOL isLoggedIn(std::wstring username, ULONG* newSessionId);
	BOOL logout(ULONG sessionId);

private:
	BOOL isUsedSessionId(ULONG newSessionId);
	ULONG generateSessionId();

	std::wstring m_userDataDirRoot;
	std::vector< TAuthServiceUser > m_registeredUser;
	std::map<ULONG, TAuthServiceUser*> m_loggedInUsers;
	CRITICAL_SECTION m_critSection;
};

template <class TAuthServiceUser>
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
CAuthServiceTemplate<TAuthServiceUser>::~CAuthServiceTemplate()
{
	EnterCriticalSection(&m_critSection);

	for (std::map<ULONG, TAuthServiceUser*>::iterator it = m_loggedInUsers.begin(); it != m_loggedInUsers.end(); ++it) {
		delete (it->second);
	}

	LeaveCriticalSection(&m_critSection);
	DeleteCriticalSection(&m_critSection);

}

template <class TAuthServiceUser>
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
		if (user.Deserialize(wLine))
		{
			EnterCriticalSection(&m_critSection);
			m_registeredUser.push_back(user);
			LeaveCriticalSection(&m_critSection);
		}
	}

	infile.close();

	return TRUE;
}

template <class TAuthServiceUser>
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
ULONG CAuthServiceTemplate<TAuthServiceUser>::generateSessionId()
{
	if (sizeof(int) < sizeof(ULONG))
		// creates nasty warning C4293 but that's something the compiler cannot check
		return (static_cast<ULONG>(rand()) << (sizeof(int) * 8)) |
		rand();

	return rand();
}

template <class TAuthServiceUser>
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

