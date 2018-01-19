/**---------------------------------------------------------------------
// project:	HelperLib
// file:	IPValidate.h
//
// summary:	Method to validate a given IP address. Adapted version from
// https://stackoverflow.com/questions/318236/how-do-you-validate-that-a-string-is-a-valid-ipv4-address-in-c
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			12.01.2018	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#pragma once
#include <string>
#include <winsock2.h>
#include <WS2tcpip.h>

#include "CharStringConverter.h"

// include neecssary Ws2_32.lib dependency for inet_pton method
#pragma comment(lib, "Ws2_32.lib")

/**---------------------------------------------------------------------
 * <summary>	Validates the IP address described by ipAddress. </summary>
 *
 * <param name="ipAddress">	The IP address. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool validateIpAddress(const std::wstring &ipAddress)
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, wstring_to_string(ipAddress).c_str(), &(sa.sin_addr));
	return result > 0;
}

/**---------------------------------------------------------------------
 * <summary>	Validates the IP address described by ipAddress. </summary>
 *
 * <param name="ipAddress">	[in,out] If non-null, the IP address. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool validateIpAddress(char* ipAddress)
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
	return result > 0;
}