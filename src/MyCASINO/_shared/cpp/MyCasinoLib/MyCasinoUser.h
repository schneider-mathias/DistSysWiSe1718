#pragma once
#include <windows.h>
#include "IAuthServiceUser.h"

enum MyCasinoUserTypes {
	Operator = 0,
	Gamer
};


class MyCasinoUser : public IAuthServiceUser
{
public:
	MyCasinoUser();
	~MyCasinoUser();
	std::wstring Serialize();
	BOOL Deserialize(std::wstring in);
	BOOL IsUserType(MyCasinoUserTypes userType);

private:
	int m_id;
	MyCasinoUserTypes m_userType;

};