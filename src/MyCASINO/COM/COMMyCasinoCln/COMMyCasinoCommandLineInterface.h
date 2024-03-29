/**---------------------------------------------------------------------
// project:	COMMyCasinoCln
// file:	COMMyCasinoCommandLineInterface.h
//
// summary:	Declares the com my casino command line interface class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			29.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#pragma once
#include <string>
#include "MyCasinoClnLib\MyCasinoCommandLineInterface.h"
#include "CmdInterpreterLib\CmdInterpreter.h"

#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.h"

class COMMyCasinoCommandLineInterface : public MyCasinoCommandLineInterface
{
public:
	COMMyCasinoCommandLineInterface(CmdInterpreter* interpreter, ICOMMyCasino* pICOMMyCasinoSrv);
	~COMMyCasinoCommandLineInterface();

	bool user(std::wstring user, std::wstring password);
	bool payin(std::wstring user, double amount);
	bool bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber);
	bool showbets();
	bool draw(unsigned short** firstNumberTest, unsigned short** secondNumberTest);
	bool showstatus();
	bool bye();
private:
	ICOMMyCasino* m_pICOMMyCasinoSrv = NULL;
};