#pragma once
#include <windows.h>
#include <string>

class IAuthServiceUser {
public:
	virtual std::wstring Serialize() = 0;
	virtual BOOL Deserialize(std::wstring in) = 0;

	std::wstring m_username;
	std::wstring m_password;
};

