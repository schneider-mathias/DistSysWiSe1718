#pragma once
#include <windows.h>
#include "IAuthServiceUser.h"

class AuthServiceDefaultUser : public IAuthServiceUser 
{
public:
	AuthServiceDefaultUser();
	~AuthServiceDefaultUser();
	std::wstring Serialize();
	BOOL Deserialize(std::wstring in);
};