#include <iostream>
#include "CmdInterpreter.h"


CmdInterpreter::CmdInterpreter()
{
	m_previousBuffer = NULL;
	m_mode = CmdModes::Reading;
}

CmdInterpreter::~CmdInterpreter()
{
	std::cout.rdbuf(m_previousBuffer); //reset
}

bool CmdInterpreter::registerCmd()
{
}

void CmdInterpreter::run()
{
	m_previousBuffer = std::cout.rdbuf(m_outBuffer.rdbuf());

	while (m_mode != CmdModes::Stopped)
	{
		if (m_mode == CmdModes::Writing)
		{
			std::cout << "WRITE MODE:" << std::endl;

			cout();
			m_mode = CmdModes::Reading;
		}
		else if (m_mode == CmdModes::Reading)
		{
			std::cout << "READ MODE:" << std::endl;
			std::string scommand;
			while (std::getline(std::cin, scommand) && !scommand.empty())
			{
				std::cout << scommand << std::endl;
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
	std::cout << m_outBuffer.str() << std::endl;
}