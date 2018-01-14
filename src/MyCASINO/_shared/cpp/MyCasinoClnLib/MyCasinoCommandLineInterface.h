/**--------------------------------------------------------------------------------------------------
// project:	MyCasinoClnLib
// file:	MyCasinoCommandLineInterface.h
//
// summary:	Declares my casino command line interface class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#pragma once
#include <type_traits>

#include "CmdInterpreter.h"
#include "ICommandLineInterface.h"


class MyCasinoCommandLineInterface : public ICommandLineInterface
{
public:
	MyCasinoCommandLineInterface(CmdInterpreter* interpreter);
	~MyCasinoCommandLineInterface();

	bool ProcessCommand(std::vector<std::wstring>);

	virtual bool user(std::wstring user, std::wstring password) = 0;
	virtual bool payin(std::wstring user, double amount) = 0;
	virtual bool bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber) = 0;
	virtual bool showbets() = 0;
	virtual bool draw(unsigned short** firstNumberTest = nullptr, unsigned short** secondNumberTest = nullptr) = 0;
	virtual bool showstatus() = 0;
	virtual bool bye() = 0;

private:
	bool isLoggedIn();
	bool fullfillsPermissionLevel(short minRequiredUserType);
	bool checkCallPrerequisites(short minRequiredUserType);

protected:
	void resultHandler(std::string additionalInformation, int code, std::string& msg);
	unsigned long* m_pSessionId;
	short* m_pUserType;
	CmdInterpreter* m_pInterpreter;
};

