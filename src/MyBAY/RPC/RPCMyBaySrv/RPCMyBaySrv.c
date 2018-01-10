/************************************************************/
/*                                                          */
/* Inhalt:    Initialisierung des Servers		            */
/*                                                          */
/* Autor:	  Johannes Sauer		                        */
/* Stand:     23. Jan 2018                                  */
/*															*/
/************************************************************/

#include "MyBay_i_h.h"
#include "RpcException.h"
#include "Config.h"
#include <stdio.h>

void main(void)
{
	RPC_STATUS status;
	unsigned char *protocolSequence = MYBAY_RPC_PROT_SEQ;
	unsigned char *endpoint = MYBAY_RPC_ENDPOINT;
	unsigned char *security = NULL;             // Keine Sicherheit
	unsigned int   minCalls = 1;
	unsigned int   maxCalls = 10;


	// Protokoll und Endpunkt registrieren
	status = RpcServerUseProtseqEp(protocolSequence,
		maxCalls,
		endpoint,
		security);
	RPC_STATUS_ASSERT("RpcServerUseProtseqEp", status);

	// Schnittstelle registrieren
	status = RpcServerRegisterIf(MyBay_i_v1_0_s_ifspec, // Zu reg. Schnittstelle
		NULL,                   // MgrTypeUuid
		NULL);                  // MgrEpv
	RPC_STATUS_ASSERT("RpcServerRegisterIf", status);

	// Auf Anforderungen warten...
	status = RpcServerListen(minCalls, maxCalls, FALSE);
	RPC_STATUS_ASSERT("RpcServerListen", status);
}