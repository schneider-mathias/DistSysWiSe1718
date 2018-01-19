/**--------------------------------------------------------------------------------------------------
// project:	COMMyCasinoCln
// file:	COMMyCasinoCln.cpp
//
// summary:	Implements the com my casino cln class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			19.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-----------------------------------------------------------------------------------------------**/

#include "IPValidate.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <atlsafe.h>

#import "COMMyCasinoSrv.dll"

#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.c"
#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.h"

#include "BstrStringConverter.h"
#include "COMMyCasinoCommandLineInterface.h"
#include "CmdInterpreter.h"

/**--------------------------------------------------------------------------------------------------
 * <summary>	Main entry-point for this application. </summary>
 *
 * <param name="argc">	The number of command-line arguments provided. </param>
 * <param name="argv">	An array of command-line argument strings. </param>
 *
 * <returns>	Exit-code for the process - 0 for success, else an error code. </returns>
 *-----------------------------------------------------------------------------------------------**/

int main(int argc, char**argv)
{
	CoInitialize(NULL);
	IClassFactory *pCF = NULL;
	
	ICOMMyCasino *pICOMMyCasinoSrv = NULL;
	HRESULT hr;

	// Create COM object (remotely if IP is provided)
	COSERVERINFO srvInfo = { 0, L"127.0.0.1", 0, 0 };
	LPWSTR lpwstrSrvName = NULL;
	
	// adjust IP if provided by command line
	if (argc > 1)
	{
		std::wstring wstrSrvName(argv[1], argv[1] + strlen(argv[1]) + 1);
		if (!validateIpAddress(wstrSrvName))
		{
			std::wcerr << "[ERROR] Invalid format for IP address parameter: " << wstrSrvName << std::endl;
			return FALSE;
		}

		lpwstrSrvName = new WCHAR[wstrSrvName.length()];
		StrCpyNW(lpwstrSrvName, wstrSrvName.c_str(), wstrSrvName.length());
		std::copy(argv[1], argv[1] + strlen(argv[1]), lpwstrSrvName);

		srvInfo.pwszName = lpwstrSrvName;
	}
	
	MULTI_QI multiQi = { &IID_ICOMMyCasino, 0, 0 };


	// create remote COM object
	hr = CoCreateInstanceEx(CLSID_COMMyCasino, NULL, CLSCTX_REMOTE_SERVER,
		&srvInfo, 1, &multiQi);

	if (FAILED(hr))
	{
		std::cout << "[ERROR] CoCreateInstanceEx - " << std::hex << hr << std::endl;
		return FALSE;
	}

	pICOMMyCasinoSrv = (ICOMMyCasino*)multiQi.pItf;

	std::cout << "--- MY CASINO ---" << std::endl;

	// start MyCasino commandline interpreter
	CmdInterpreter interpreter;
	COMMyCasinoCommandLineInterface myCasinoCLI(&interpreter, pICOMMyCasinoSrv);
	dispatcherMemFunc p = &(ICommandLineInterface::ProcessCommand);
	interpreter.registerCmdDispatcher(&myCasinoCLI, p);
	interpreter.run();

	// clean up COM object
	pICOMMyCasinoSrv->Release();
	CoUninitialize();

	if (NULL != lpwstrSrvName)
		delete[] lpwstrSrvName;

	return S_OK;
}