/**---------------------------------------------------------------------
// project:	CmdInterpreterLib
// file:	CmdInterpreter.cpp
//
// summary:	Implements the command interpreter class. The class gets commands 
//			from the commandline and passes it to the registered dispatcher method.
//			It collects the cout and cerr stream into its own stream and displays it
//			after a command was run in a formatted way.
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			29.12.2018	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#include <iostream>
#include "CmdInterpreter.h"
#include "HelperLib/CharStringConverter.h"

/**---------------------------------------------------------------------
 * <summary>	Constructor. </summary>
 *
 * <param name="linePrefix">	   	The line prefix before any output message. </param>
 * <param name="defaultSuccessMsg">	The default success message after a command succedded. </param>
 * <param name="defaultErrorMsg">  	The default error message after a command failed. </param>
 *-------------------------------------------------------------------**/

CmdInterpreter::CmdInterpreter(std::wstring linePrefix, std::wstring defaultSuccessMsg, std::wstring defaultErrorMsg)
	: m_previousBufferOut(NULL),
	m_dispatcherFunc(NULL),
	m_mode(CmdModes::Reading),
	m_linePrefix(linePrefix),
	m_defaultSuccessMsg(defaultSuccessMsg),
	m_defaultErrorMsg(defaultErrorMsg)
{
}

/** <summary>	Destructor. </summary> */
CmdInterpreter::~CmdInterpreter()
{
	// reset stream
	std::cout.rdbuf(m_previousBufferOut); 
}

/**---------------------------------------------------------------------
 * <summary>	Registers the command dispatcher. </summary>
 *
 * <param name="dispatcherObj">	[in,out] If non-null, the dispatcher object which contains the
 * 								logic for processing a command string. </param>
 * <param name="func">		   	The processing member funcion of the dispatcher object. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool CmdInterpreter::registerCmdDispatcher(ICommandLineInterface* dispatcherObj, dispatcherMemFunc func)
{
	m_pDispatcherObj = dispatcherObj;
	m_dispatcherFunc = func;
	return true;
}

/**---------------------------------------------------------------------
 * <summary>
 * Splits an command string to its parameters (takes quotes into account). 
 * Code from https://stackoverflow.com/questions/18675364/c-tokenize-a-string-with-spaces-and-quotes.
 * </summary>
 *
 * <param name="qargs">  	[in,out] The split arguments. </param>
 * <param name="command">	The command string that should be split. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool CmdInterpreter::splitInArgs(std::vector<std::wstring>& qargs, std::wstring command)
{
	int len = command.length();
	bool qot = false, sqot = false;
	int arglen;
	for (int i = 0; i < len; i++) {
		int start = i;
		if (command[i] == '\"') {
			qot = true;
		}
		else if (command[i] == '\'') sqot = true;

		if (qot) {
			i++;
			start++;
			while (i<len && command[i] != '\"')
				i++;
			if (i<len)
				qot = false;
			arglen = i - start;
			i++;
		}
		else if (sqot) {
			i++;
			while (i<len && command[i] != '\'')
				i++;
			if (i<len)
				sqot = false;
			arglen = i - start;
			i++;
		}
		else {
			while (i<len && command[i] != ' ')
				i++;
			arglen = i - start;
		}
		qargs.push_back(command.substr(start, arglen));
	}
	if (qot || sqot)
		return false;

	return true;
}

/** <summary>	Initializes the commandline interpreter. </summary> */
void CmdInterpreter::init()
{
	m_previousBufferOut = std::cout.rdbuf(m_outBuffer.rdbuf());
}

/** <summary>	Runs the commandline interpreter. </summary> */
void CmdInterpreter::run()
{
	m_previousBufferOut = std::cout.rdbuf(m_outBuffer.rdbuf());

	// toggle between reading and writing
	while (m_mode != CmdModes::Stopped)
	{
		if (m_mode == CmdModes::Writing)
		{
			cout();
			m_mode = CmdModes::Reading;
		}
		else if (m_mode == CmdModes::Reading)
		{
			std::wstring scommand;
			std::getline(std::wcin, scommand);
			execute(scommand);
			if (m_mode == CmdModes::Stopped)
				break;

			m_mode = CmdModes::Writing;
		}
	}
}

/**---------------------------------------------------------------------
 * <summary>	Executes the given command. </summary>
 *
 * <param name="command">	The command as a string that should be executed. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool CmdInterpreter::execute(std::wstring command)
{
	if (!command.empty())
	{
		// split comand to its parts
		std::vector<std::wstring> commandArguments;
		if (!splitInArgs(commandArguments, command))
		{
			std::cerr << "Command ends with opened quotes" << std::endl;
			return false;
		}

		// execute command by passing it to the dispatcher object method for processing the command parameters
		if (commandArguments.size() > 0)
		{
			if (NULL != m_dispatcherFunc && !CALL_MEMBER_FN(*m_pDispatcherObj, m_dispatcherFunc)(commandArguments))
			{
				if (!m_defaultErrorMsg.empty())
					std::cout << wstring_to_string(m_defaultErrorMsg) << std::endl;
				return false;
			}
			if (!m_defaultSuccessMsg.empty())
				std::cout << wstring_to_string(m_defaultSuccessMsg) << std::endl;
			
		}
		return true;
	}
	return false;
}

/** <summary>	Stops this the commandline interpreter. </summary> */
void CmdInterpreter::stop()
{
	cout();

	//reset stream buffer
	if (NULL != m_previousBufferOut)
	{
		std::cout.rdbuf(m_previousBufferOut);
	}

	m_mode = CmdModes::Stopped;
}


/** <summary>	Outputs the collected buffer to console in a formatted way. </summary> */
void CmdInterpreter::cout()
{
	if (m_outBuffer.str().empty())
		return;

	std::cout.rdbuf(m_previousBufferOut);
	std::cerr.rdbuf(m_previousBufferErrorOut);


	auto outputLinewise = splitString(m_outBuffer.str(), "\n");
	for (auto it = outputLinewise.begin(); it != outputLinewise.end(); it++)
		std::cout << wstring_to_string(m_linePrefix) << (*it) << std::endl;

	// clear the buffer:
	// https://stackoverflow.com/questions/20731/how-do-you-clear-a-stringstream-variable
	m_outBuffer.str("");

	m_previousBufferOut = std::cout.rdbuf(m_outBuffer.rdbuf());
	m_previousBufferErrorOut = std::cerr.rdbuf(m_outBuffer.rdbuf());
}

/**---------------------------------------------------------------------
 * <summary>	Split a string by a delimter into a vector.
 * 				https://stackoverflow.com/questions/13172158/c-split-string-by-line. </summary>
 *
 * <param name="str">	   	The string whch should be split. </param>
 * <param name="delimiter">	The delimiter for splitting. </param>
 *
 * <returns>	A std::vector&lt;std::string&gt; </returns>
 *-------------------------------------------------------------------**/

std::vector<std::string> CmdInterpreter::splitString(const std::string& str, const std::string& delimiter)
{
	std::vector<std::string> strings;

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = str.find(delimiter, prev)) != std::string::npos)
	{
		strings.push_back(str.substr(prev, pos - prev));
		prev = pos + delimiter.size();
	}

	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(prev));

	return strings;
}