/**--------------------------------------------------------------------------------------------------
// project:	CmdInterpreterLib
// file:	CmdInterpreter.h
//
// summary:	Declares the command interpreter class, inspired by
// https://stackoverflow.com/questions/19485536/redirect-output-of-an-function-printing-to-console-to-string
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#pragma once
#include <queue>
#include <atomic>
#include <string>
#include <sstream>

#include "ICommandLineInterface.h"

typedef bool (ICommandLineInterface::*dispatcherMemFunc)(std::vector<std::wstring>);

// Helper define for calling a member function of a given object
// https://isocpp.org/wiki/faq/pointers-to-members
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))


/** <summary>	Values that represent Command modes. </summary> */
enum CmdModes
{
	Reading = 0,
	Writing,
	Stopped
};

class CmdInterpreter
{
public:
	CmdInterpreter(std::wstring defaultSuccessMsg = L"", std::wstring defaultErrorMsg = L"");
	~CmdInterpreter();

	bool registerCmdDispatcher(ICommandLineInterface* dispatcherObj, dispatcherMemFunc func);
	void init();
	void run();
	bool execute(std::wstring command);
	void cout();
	void stop();

private:
	bool splitInArgs(std::vector<std::wstring>& qargs, std::wstring command);
	std::atomic<CmdModes> m_mode;
	std::stringstream m_outBuffer;
	std::streambuf* m_previousBuffer;
	ICommandLineInterface* m_pDispatcherObj;
	dispatcherMemFunc m_dispatcherFunc;
	std::wstring m_defaultSuccessMsg;
	std::wstring m_defaultErrorMsg;
};