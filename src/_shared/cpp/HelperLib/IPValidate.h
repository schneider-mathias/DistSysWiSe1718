#pragma once
#include <string>
#include <winsock2.h>
#include <WS2tcpip.h>

#include "CharStringConverter.h"

// include neecssary Ws2_32.lib dependency for inet_pton method
#pragma comment(lib, "Ws2_32.lib")

// https://stackoverflow.com/questions/318236/how-do-you-validate-that-a-string-is-a-valid-ipv4-address-in-c
bool validateIpAddress(const std::wstring &ipAddress)
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, wstring_to_char(ipAddress), &(sa.sin_addr));
	return result > 0;
}

bool validateIpAddress(char* ipAddress)
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
	return result > 0;
}