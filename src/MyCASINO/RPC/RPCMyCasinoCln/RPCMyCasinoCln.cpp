#include "MyCasino_i.h"    
#include "Config.h"
#include "RpcException.h"
#include <iostream>

#include "RPCMyCasinoCommandLineInterface.h"
#include "CmdInterpreter.h"


static void Bind(void);
static void UnBind(void);
static void rpcCalls(void);
void startCommandLineInterface();

void main(void)
{
	try
	{
		Bind();
		try
		{
			rpcCalls();
		}
		catch (...)
		{
			UnBind();
			throw;
		}
		/* RPC-Bindung freigeben */
		UnBind();
	}
	catch (RpcException& e)
	{
		if (e.GetStatus() == RPC_S_SERVER_UNAVAILABLE)
		{
			std::cerr << "Fehler: Der Server antwortet nicht!\nDer Client wird beendet..." << std::endl;
		}
		else
		{
			printf("Unbekannter Fehler\nstat=0x%x, text=%s, type=%s\n",
				(int)e.GetStatus(), e.GetErrorText(), e.GetErrorType());
		}
	}

	printf("Enter druecken!\n");
	getchar();
}

void rpcCalls(void)
{
	RpcTryExcept
	{
		startCommandLineInterface();
	}
	RpcExcept(1)
	{
		RpcException::Raise(RpcExceptionCode(), "RPC call failed", "Remote Procedure Call");
	}
	RpcEndExcept
}

void Bind(void)
{
	RPC_STATUS status;
	unsigned char *protocolSequence = (UCHAR*)MYCASINO_RPC_PROT_SEQ;
	unsigned char *endpoint = (UCHAR*)MYCASINO_RPC_ENDPOINT;
	unsigned char *netwAddr = (UCHAR*)MYCASINO_RPC_DEF_NETWADDR;

	unsigned char *stringBinding = NULL;


	// Erzeugung der Stringdarstellung des Binding-Handles 
	status = RpcStringBindingCompose(NULL,             // Objekt UUID
		protocolSequence, // Server-/Verbindungsdaten
		netwAddr,
		endpoint,
		NULL,             // Keine Optionen fuer TCP/IP
		&stringBinding);
	if (status)
	{
		throw(RpcException(status, (string)"RpcStringBindingCompose failed: Network Address = " +
			(char*)netwAddr,
			"RPC Runtime Error"));
	}

	// Erzeugung des Binding-Handles
	status = RpcBindingFromStringBinding(stringBinding,
		&hMyCasino_i);
	if (status)
	{
		throw(RpcException(status, (string)"RpcBindingFromStringBinding failed: String = " +
			(char*)stringBinding,
			"RPC Runtime Error"));
	}

	// Freigabe der Stringdarstellung des Binding-Handles 
	status = RpcStringFree(&stringBinding);
	if (status)
	{
		throw(RpcException(status, "RpcStringFree failed", "RPC Error"));
	}
}

void UnBind(void)
{
	RPC_STATUS status;

	// Freigabe des Binding-Handles 
	status = RpcBindingFree(&hMyCasino_i);
	if (status)
	{
		throw(RpcException(status, "RpcBindingFree failed", "RPC Error"));
	}
}

void startCommandLineInterface()
{
	std::cout << "--- MY CASINO ---" << std::endl;

	CmdInterpreter interpreter;
	RPCMyCasinoCommandLineInterface myCasinoCLI;
	dispatcherMemFunc p = &(ICommandLineInterface::ProcessCommand);
	interpreter.registerCmdDispatcher(&myCasinoCLI, p);
	interpreter.run();
}