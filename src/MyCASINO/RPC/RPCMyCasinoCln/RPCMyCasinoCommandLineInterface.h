/**---------------------------------------------------------------------
// project:	RPCMyCasinoCln
// file:	RPCMyCasinoCommandLineInterface.h
//
// summary:	Declares the RPC my casino command line interface class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			30.12.2018	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#pragma once
#include <string>
#include "MyCasinoClnLib\MyCasinoCommandLineInterface.h"


class RPCMyCasinoCommandLineInterface : public MyCasinoCommandLineInterface
{
public:
	RPCMyCasinoCommandLineInterface(CmdInterpreter* interpreter);
	~RPCMyCasinoCommandLineInterface();

	bool user(std::wstring user, std::wstring password);
	bool payin(std::wstring user, double amount);
	bool bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber);
	bool showbets();
	bool draw(unsigned short** firstNumberTest, unsigned short** secondNumberTest);
	bool showstatus();
	bool bye();

};
