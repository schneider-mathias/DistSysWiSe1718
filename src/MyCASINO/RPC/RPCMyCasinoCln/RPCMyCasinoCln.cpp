/**---------------------------------------------------------------------
// project:	RPCMyCasinoCln
// file:	RPCMyCasinoCln.cpp
//
// summary:	Implements the RPC my casino cln class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			28.12.2018	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#include "IPValidate.h"
#include "MyCasino_i.h"    
#include "Config.h"
#include "RpcException.h"

#include "RPCMyCasinoCommandLineInterface.h"
#include "CmdInterpreter.h"


static void Bind(char*);
static void UnBind(void);
static void rpcCalls(void);
void startCommandLineInterface();

/**---------------------------------------------------------------------
 * <summary>	Main entry-point for this application. </summary>
 *
 * <param name="argc">	The number of command-line arguments provided. </param>
 * <param name="argv">	An array of command-line argument strings. </param>
 *-------------------------------------------------------------------**/

void main(int argc, char**argv)
{
	char* srvAdress = NULL;

	if (argc > 1)
	{
		srvAdress =  (char*)malloc((strlen(argv[1]) + 1) * sizeof(char));
		srvAdress = _strdup(argv[1]);

		if (!validateIpAddress(srvAdress))
		{
			std::cout << "Invalid format for IP address parameter: " << srvAdress << std::endl;
			return;
		}
	}

	try
	{
		Bind(srvAdress);
		try
		{
			rpcCalls();
		}
		catch (...)
		{
			UnBind();
			throw;
		}
		// free RPC binding
		UnBind();
	}
	catch (RpcException& e)
	{
		if (e.GetStatus() == RPC_S_SERVER_UNAVAILABLE)
		{
			printf("\t[Error]: No server connection. Stop client.\n");
		}
		else
		{
			printf("Unknown error\nstat=0x%x, text=%s, type=%s\n",
				(int)e.GetStatus(), e.GetErrorText(), e.GetErrorType());
		}
	}

	if (NULL != srvAdress)
		free(srvAdress);

	printf("Please press a button.\n");
	getchar();
}

/** <summary>	RPC calls. </summary> */
void rpcCalls(void)
{
	// save current stream buffer in order to restore it 
	// in case of an exception
	auto defaultCoutBuffer = std::cout.rdbuf();

	RpcTryExcept
	{
		// start command line interface
		startCommandLineInterface();
	}
	RpcExcept(1)
	{
		// Reset buffer because destructor of CmdInterpreter is not called 
		// when execption is thrown. Because __try can't be used in function that 
		// require object unwinding it is not possible to create CmdInterpreter object
		// on stack before RpcTryExcept (in this case the destructor would clean up buffer)
		std::cout.rdbuf(defaultCoutBuffer);
		RpcException::Raise(RpcExceptionCode(), "RPC call failed", "Remote Procedure Call");
	}
	RpcEndExcept
}

/**---------------------------------------------------------------------
 * <summary>	Binds the given remote netw address. </summary>
 *
 * <exception cref="RpcException">	Thrown when a RPC error condition occurs. </exception>
 *
 * <param name="remoteNetwAddr">	[in,out] If non-null, the remote netw address. </param>
 *-------------------------------------------------------------------**/

void Bind(char* remoteNetwAddr)
{
	RPC_STATUS status;
	unsigned char *protocolSequence = (UCHAR*)MYCASINO_RPC_PROT_SEQ;
	unsigned char *endpoint = (UCHAR*)MYCASINO_RPC_ENDPOINT;
	unsigned char *netwAddr = (UCHAR*)"127.0.0.1";

	if (NULL != remoteNetwAddr)
		netwAddr = (UCHAR*)remoteNetwAddr;

	unsigned char *stringBinding = NULL;


	// create string presentation of binding handle
	status = RpcStringBindingCompose(NULL,      //object UUID
		protocolSequence,						// server connection infomation
		netwAddr,
		endpoint,
		NULL,									// no options for TCP/IP
		&stringBinding);
	if (status)
	{
		throw(RpcException(status, (string)"RpcStringBindingCompose failed: Network Address = " +
			(char*)netwAddr,
			"RPC Runtime Error"));
	}

	// create binding handle
	status = RpcBindingFromStringBinding(stringBinding,
		&hMyCasino_i);
	if (status)
	{
		throw(RpcException(status, (string)"RpcBindingFromStringBinding failed: String = " +
			(char*)stringBinding,
			"RPC Runtime Error"));
	}

	// free string presentation of binding handle
	status = RpcStringFree(&stringBinding);
	if (status)
	{
		throw(RpcException(status, "RpcStringFree failed", "RPC Error"));
	}
}

/**---------------------------------------------------------------------
 * <summary>	Un bind. </summary>
 *
 * <exception cref="RpcException">	Thrown when a RPC error condition occurs. </exception>
 *-------------------------------------------------------------------**/

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

/** <summary>	Starts command line interface. </summary> */
void startCommandLineInterface()
{
	std::cout << "--- MY CASINO ---" << std::endl;

	CmdInterpreter interpreter;
	RPCMyCasinoCommandLineInterface myCasinoCLI(&interpreter);
	dispatcherMemFunc p = &(ICommandLineInterface::ProcessCommand);
	interpreter.registerCmdDispatcher(&myCasinoCLI, p);
	interpreter.run();
}