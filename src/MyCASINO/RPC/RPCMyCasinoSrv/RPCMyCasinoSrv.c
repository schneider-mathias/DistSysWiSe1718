/**--------------------------------------------------------------------------------------------------
// project:	RPCMyCasinoSrv
// file:	RPCMyCasinoSrv.c
//
// summary:	RPC my casino server class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			28.12.2017	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#include "MyCasino_i.h"
#include "RpcException.h"
#include "Config.h"
#include <stdio.h>

/** <summary>	Main entry-point for this application. </summary> */
void main(void)
{
	RPC_STATUS status;
	unsigned char *protocolSequence = MYCASINO_RPC_PROT_SEQ;
	unsigned char *endpoint = MYCASINO_RPC_ENDPOINT;
	unsigned char *security = NULL;
	unsigned int   minCalls = 1;
	unsigned int   maxCalls = 10;

	// register protocol and endpoint
	status = RpcServerUseProtseqEp(protocolSequence,
		maxCalls,
		endpoint,
		security);
	RPC_STATUS_ASSERT("RpcServerUseProtseqEp", status);

	// register interface
	status = RpcServerRegisterIf(MyCasino_i_v1_0_s_ifspec,
		NULL,                   // MgrTypeUuid
		NULL);                  // MgrEpv
	RPC_STATUS_ASSERT("RpcServerRegisterIf", status);

	// wait for requests
	status = RpcServerListen(minCalls, maxCalls, FALSE);
	RPC_STATUS_ASSERT("RpcServerListen", status);
}