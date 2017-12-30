#include <iostream>
#include "CmdInterpreter.h"


CmdInterpreter::CmdInterpreter()
{
	m_previousBuffer = NULL;
	m_dispatcherFunc = NULL;
	m_mode = CmdModes::Reading;
}

CmdInterpreter::~CmdInterpreter()
{
	std::cout.rdbuf(m_previousBuffer); //reset
}

bool CmdInterpreter::registerCmdDispatcher(ICommandLineInterface* dispatcherObj, dispatcherMemFunc func)
{
	m_dispatcherObj = dispatcherObj;
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
			if (!scommand.empty())
			{
				std::vector<std::wstring> commandArguments;
				if (!splitInArgs(commandArguments, scommand))
				{
					std::cerr << "Command ends with opened quotes" << std::endl;
				}

				if (commandArguments.size() > 0)
				{
					if (NULL != m_dispatcherFunc && !CALL_MEMBER_FN(*m_dispatcherObj,m_dispatcherFunc)(commandArguments))
					{
						std::wcerr << "Error while running command " << commandArguments.at(0) << std::endl;
					}
				}

				
			}
			m_mode = CmdModes::Writing;

		}
	}
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