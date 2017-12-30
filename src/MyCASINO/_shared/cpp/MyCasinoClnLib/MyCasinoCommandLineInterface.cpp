#include "MyCasinoCommandLineInterface.h"
#include <iostream>

MyCasinoCommandLineInterface::MyCasinoCommandLineInterface()
{
}

MyCasinoCommandLineInterface::~MyCasinoCommandLineInterface()
{

}

bool MyCasinoCommandLineInterface::ProcessCommand(std::vector<std::wstring> arguments)
{
	std::wstring command(arguments.at(0));

	// using std::invoke would be cool but does not work because arguments have to match the interface types

	if (command.compare(L"user") == 0)
	{
		if (arguments.size() != 3)
		{
			std::cerr << "Invalid number of parameters for command 'user'" << std::endl;
			return false;
		}

		return user(arguments.at(1), arguments.at(2));
	}

	else if (command.compare(L"bye") == 0)
	{
		if (arguments.size() > 1)
		{
			std::cerr << "[Warning] - Ignored redudant arguments for command 'bye'" << std::endl;
			return false;
		}
		return bye();
	}
	else
	{
		std::wcerr << "Invalid command " << arguments.at(0) << std::endl;
	}

	return false;
}
