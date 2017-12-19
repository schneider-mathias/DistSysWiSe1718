/********************************************************
* AuthService.h -- Class for user authentification     *
*													   *
*                                                      *
* Authors:  Mathias, Schneider						   *
*                                                      *
* Purpose:  A common class to authentificate a user.
*			User information are looked up in 
			an external file and a session id 
			is created if user is successfully
			logged in. All operations are thread-safe.
*                                                      *
* Usage:                                               *
*		Link this static library and create 
		instance of authentification service		   *
********************************************************/

#pragma once
#include <string>
#include <windows.h>
#include <WinBase.h>
#include <map>

class CAuthService
{
public:
	CAuthService(std::wstring* userDataDirRootEnv = nullptr);
	~CAuthService();
	BOOL readRegisteredUser(std::wstring filename);
	BOOL login(std::wstring username, std::wstring password, ULONG* sessinId);
	BOOL isLoggedIn(ULONG sessionId, std::wstring* name = nullptr);
	BOOL logout(ULONG sessionId);

private:
	BOOL isLoggedIn(std::wstring username);
	BOOL isUsedSessionId(ULONG newSessionId);
	ULONG generateSessionId();

	std::wstring m_userDataDirRoot;
	std::map< std::wstring, std::wstring > m_registeredUser;
	std::map<std::wstring, ULONG> m_loggedInUsers;
	CRITICAL_SECTION m_critSection;
};