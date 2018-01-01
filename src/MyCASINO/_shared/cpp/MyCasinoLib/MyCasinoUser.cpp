#include "MyCasinoUser.h"
#include <vector>
#include <sstream>

MyCasinoUser::MyCasinoUser()
{
}

MyCasinoUser::~MyCasinoUser()
{
}

std::wstring MyCasinoUser::Serialize()
{
	std::wstring serialized;
	serialized.append(std::to_wstring(m_id));
	serialized.append(MY_CASINO_SERIALIZER_SEPARATOR_WSTRING);
	serialized.append(m_username);
	serialized.append(MY_CASINO_SERIALIZER_SEPARATOR_WSTRING);
	serialized.append(m_password);
	serialized.append(MY_CASINO_SERIALIZER_SEPARATOR_WSTRING);
	serialized.append(std::to_wstring(m_userType));
	return serialized;
}

BOOL MyCasinoUser::Deserialize(std::wstring in)
{
	std::wstring temp;
	std::vector<std::wstring> parts;
	std::wstringstream wss(in);
	while (std::getline(wss, temp, MY_CASINO_SERIALIZER_SEPARATOR_CHAR))
		parts.push_back(temp);

	if (parts.size() != MY_CASINO_SERIALIZED_PROPERTY_COUNT)
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

MyCasinoUserTypes MyCasinoUser::GetUserType()
{
	return m_userType;
}

BOOL MyCasinoUser::IsUserType(MyCasinoUserTypes userType)
{
	return userType == m_userType;
}

bool MyCasinoUser::operator==(const MyCasinoUser& ref) const
{
	return ref.m_id == m_id;
}

bool MyCasinoUser::operator<(const MyCasinoUser& ref) const
{
	return ref.m_id < m_id;
}