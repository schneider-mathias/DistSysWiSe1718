#pragma once
#include <windows.h>
#include "IAuthServiceUser.h"

#define MY_CASINO_SERIALIZED_PROPERTY_COUNT 4
#define MY_CASINO_SERIALIZER_SEPARATOR_WSTRING L" "
#define MY_CASINO_SERIALIZER_SEPARATOR_CHAR L' '

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