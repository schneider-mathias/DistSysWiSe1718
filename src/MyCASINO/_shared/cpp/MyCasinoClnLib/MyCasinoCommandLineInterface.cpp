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
		return checkCallArguments(arguments, 2, std::vector<size_t>() = { 2 })
			&& user(arguments.at(1), arguments.at(2));
	}
	else if (command.compare(L"payin") == 0)
	{
		double amountMoney;
		return checkCallPrerequisites(USER_TYPE_OPERATOR)
			&& checkCallArguments(arguments, 2, std::vector<size_t>()= { 2 })
			&& safeArgumentCast<double>(arguments, 2, &amountMoney)
			&& payin(arguments.at(1), amountMoney);
	}
	else if (command.compare(L"bet") == 0)
	{
		double setMoney;
		int setMoneyLowerBound = 0;

		unsigned short firstNumber;
		unsigned short secondNumber;

		int numberLowerBound = 1;
		int numberUpperBound = 5;

		return checkCallPrerequisites(USER_TYPE_ANY)
			&& checkCallArguments(arguments, 3, std::vector<size_t>() = { 3 })
			&& safeArgumentCast<double>(arguments, 1, &setMoney, &setMoneyLowerBound, NULL, ArgumentType::MONEY)
			&& safeArgumentCast<unsigned short>(arguments, 2, &firstNumber, &numberLowerBound, &numberUpperBound)
			&& safeArgumentCast<unsigned short>(arguments, 3, &secondNumber, &numberLowerBound, &numberUpperBound)
			&& bet(setMoney, firstNumber, secondNumber);
	}
	else if (command.compare(L"showbets") == 0)
	{
		return checkCallPrerequisites(USER_TYPE_ANY)
			&& checkCallArguments(arguments, 0, std::vector<size_t>() = { 0 })
			&& showbets();
	}
	else if (command.compare(L"draw") == 0)
	{
		unsigned short* firstNumberPtr = new unsigned short(0);
		unsigned short* secondNumberPtr = new unsigned short(0);
		unsigned short** firstNumberPtrHolder = &firstNumberPtr;
		unsigned short** secondNumberPtrHolder = &secondNumberPtr;

		int numberLowerBound = 1;
		int numberUpperBound = 5;

		bool retVal=checkCallPrerequisites(USER_TYPE_OPERATOR)
			&& checkCallArguments(arguments, 0, std::vector<size_t>() = { 0, 2 })
			&& safeArgumentCast<unsigned short>(arguments, 1, firstNumberPtrHolder, &numberLowerBound, &numberUpperBound)
			&& safeArgumentCast<unsigned short>(arguments, 2, secondNumberPtrHolder, &numberLowerBound, &numberUpperBound)
			&& draw(*firstNumberPtrHolder, *secondNumberPtrHolder);

		delete firstNumberPtr;
		delete secondNumberPtr;

		return retVal;
	}
	else if (command.compare(L"showstatus") == 0)
	{
		return checkCallPrerequisites(USER_TYPE_ANY)
			&& checkCallArguments(arguments, 0, std::vector<size_t>() = { 0 })
			&& showstatus();
	}
	else if (command.compare(L"bye") == 0)
	{
		if (arguments.size() > 1)
		{
			std::cerr << "[Warning] - Ignored redudant arguments for command 'bye'" << std::endl;
			return false;
		}

		bool retVal = checkCallPrerequisites(USER_TYPE_ANY) && bye();

		if (retVal)
		{
			// reset user information
			if (NULL != m_pSessionId)
			{
				delete m_pSessionId;
				m_pSessionId = NULL;
			}
			if (NULL != m_pUserType)
			{
				delete m_pUserType;
				m_pUserType = NULL;
			}
		}

		return retVal;
	}
	else
	{
		std::wcerr << "Invalid command " << arguments.at(0) << std::endl;
	}

	return false;
}


bool MyCasinoCommandLineInterface::checkCallPrerequisites(short minRequiredUserType)
{
	if (!isLoggedIn())
	{
		std::cerr << "Please login first before running another command." << std::endl;
		return false;
	}

	if (!fullfillsPermissionLevel(minRequiredUserType))
	{
		std::cerr << "Invalid command for this type of user." << std::endl;
		return false;
	}
	return true;
}

bool MyCasinoCommandLineInterface::isLoggedIn()
{
	return NULL != m_pSessionId;
}

bool MyCasinoCommandLineInterface::fullfillsPermissionLevel(short minRequiredUserType)
{
	return (*m_pUserType) <= minRequiredUserType;
}
