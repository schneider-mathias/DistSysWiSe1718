#include "AuthServiceDefaultUser.h"
#include <vector>
#include <sstream>

AuthServiceDefaultUser::AuthServiceDefaultUser()
{
}

AuthServiceDefaultUser::~AuthServiceDefaultUser()
{
}


std::wstring AuthServiceDefaultUser::Serialize()
{
	std::wstring serialized;
	serialized.append(m_username);
	serialized.append(L" ");
	serialized.append(m_password);
	return serialized;
}
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