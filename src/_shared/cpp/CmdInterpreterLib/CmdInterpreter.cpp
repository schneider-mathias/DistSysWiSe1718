#include <iostream>
#include "CmdInterpreter.h"


CmdInterpreter::CmdInterpreter(std::wstring defaultSuccessMsg, std::wstring defaultErrorMsg)
	: m_previousBuffer(NULL),
	m_dispatcherFunc(NULL),
	m_mode(CmdModes::Reading),
	m_defaultSuccessMsg(defaultSuccessMsg),
	m_defaultErrorMsg(defaultErrorMsg)
{
}

CmdInterpreter::~CmdInterpreter()
{
	std::cout.rdbuf(m_previousBuffer); //reset
}

bool CmdInterpreter::registerCmdDispatcher(ICommandLineInterface* dispatcherObj, dispatcherMemFunc func)
{
	m_pDispatcherObj = dispatcherObj;
	m_dispatcherFunc = func;
	return false;
}

//https://stackoverflow.com/questions/18675364/c-tokenize-a-string-with-spaces-and-quotes
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

void CmdInterpreter::init()
{
	m_previousBuffer = std::cout.rdbuf(m_outBuffer.rdbuf());
}

void CmdInterpreter::run()
{
	m_previousBuffer = std::cout.rdbuf(m_outBuffer.rdbuf());

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

bool CmdInterpreter::execute(std::wstring command)
{
	if (!command.empty())
	{
		std::vector<std::wstring> commandArguments;
		if (!splitInArgs(commandArguments, command))
		{
			std::cerr << "Command ends with opened quotes" << std::endl;
		}

		if (commandArguments.size() > 0)
		{
			if (NULL != m_dispatcherFunc && !CALL_MEMBER_FN(*m_pDispatcherObj, m_dispatcherFunc)(commandArguments))
			{
				if (!m_defaultErrorMsg.empty())
					std::wcout << m_defaultErrorMsg << std::endl;
				return false;
			}
			if (!m_defaultSuccessMsg.empty())
				std::wcout << m_defaultSuccessMsg << std::endl;
			
		}
		return true;
	}
	return false;
}

void CmdInterpreter::stop()
{
	//empty buffer


	//reset stream buffer
	if (NULL != m_previousBuffer)
	{
		std::cout.rdbuf(m_previousBuffer);
	}

	m_mode = CmdModes::Stopped;
}


void CmdInterpreter::cout()
{
	std::cout.rdbuf(m_previousBuffer);
	std::cout << m_outBuffer.str();

	// clear the buffer:
	// https://stackoverflow.com/questions/20731/how-do-you-clear-a-stringstream-variable
	m_outBuffer.str("");

	m_previousBuffer = std::cout.rdbuf(m_outBuffer.rdbuf());
}