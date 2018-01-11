#include "MyCasinoCommandLineInterface.h"
#include "MyCasinoDefines.h"
#include <iostream>
#include "comdef.h"

MyCasinoCommandLineInterface::MyCasinoCommandLineInterface(CmdInterpreter* interpreter)
	:m_pSessionId(NULL),
	m_pUserType(NULL),
	m_pInterpreter(interpreter)
{
}

MyCasinoCommandLineInterface::~MyCasinoCommandLineInterface()
{
	if (NULL != m_pSessionId)
		delete m_pSessionId;
	if (NULL != m_pUserType)
		delete m_pUserType;

	m_pInterpreter = NULL;
}

void MyCasinoCommandLineInterface::resultHandler(std::string additionalInformation, int code, std::string& msg)
{
	if (code > 0)
	{
		std::cout << "[INFO] " << msg << std::endl;
	}
	else if (code == 0x800706BA) // COM error 
	{
		std::cerr << "[ERROR] " << "Lost server connection. Stop client." << std::endl;
		m_pInterpreter->stop();
	}
	else if(FAILED(code))
	{
		if (msg.empty())
		{
			//https://msdn.microsoft.com/en-us/library/windows/desktop/dd542645(v=vs.85).aspx
			_com_error err(code);
			LPCTSTR comError = err.ErrorMessage();
			std::cerr << "[ERROR] " << additionalInformation << ": " << comError << std::endl;
		}
		else
			std::cerr << "[ERROR] " << additionalInformation << ": " << msg  << std::endl;
	}
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
		double amountMoneyLowerBound = 0;
		double amountMoneyUpperBound = UPPER_MONEY_BOUNDARY;
		return checkCallPrerequisites(MyCasinoUserTypes::Operator)
			&& checkCallArguments(arguments, 2, std::vector<size_t>()= { 2 })
			&& safeArgumentCast<double>(arguments, 2, &amountMoney, &amountMoneyLowerBound, &amountMoneyUpperBound, ArgumentType::MONEY)
			&& payin(arguments.at(1), amountMoney);
	}
	else if (command.compare(L"bet") == 0)
	{
		double setMoney;
		double setMoneyLowerBound = 0;
		double setMoneyUpperBound = UPPER_MONEY_BOUNDARY;

		unsigned short firstNumber;
		unsigned short secondNumber;

		double numberLowerBound = 1;
		double firstNumberUpperBound = 4;
		double numberUpperBound = 5;

		return checkCallPrerequisites(MyCasinoUserTypes::Any)
			&& checkCallArguments(arguments, 3, std::vector<size_t>() = { 3 })
			&& safeArgumentCast<double>(arguments, 1, &setMoney, &setMoneyLowerBound, &setMoneyUpperBound, ArgumentType::MONEY)
			&& safeArgumentCast<unsigned short>(arguments, 2, &firstNumber, &numberLowerBound, &firstNumberUpperBound)
			&& safeArgumentCast<unsigned short>(arguments, 3, &secondNumber, &(numberLowerBound = (firstNumber + 1)), &numberUpperBound)
			&& bet(setMoney, firstNumber, secondNumber);
	}
	else if (command.compare(L"showbets") == 0)
	{
		return checkCallPrerequisites(MyCasinoUserTypes::Any)
			&& checkCallArguments(arguments, 0, std::vector<size_t>() = { 0 })
			&& showbets();
	}
	else if (command.compare(L"draw") == 0)
	{
		unsigned short* firstNumberPtr = new unsigned short(0);
		unsigned short* secondNumberPtr = new unsigned short(0);
		unsigned short** firstNumberPtrHolder = &firstNumberPtr;
		unsigned short** secondNumberPtrHolder = &secondNumberPtr;

		double numberLowerBound = 1;
		double firstNumberUpperBound = 4;
		double numberUpperBound = 5;

		// if first number pointer is set, it becomes the lower boundary, otherwise set it to 0 (will be ignored anyway)
		bool retVal = checkCallPrerequisites(MyCasinoUserTypes::Operator)
			&& checkCallArguments(arguments, 0, std::vector<size_t>() = { 0, 2 })
			&& safeArgumentCast<unsigned short>(arguments, 1, firstNumberPtrHolder, &numberLowerBound, &firstNumberUpperBound)
			&& safeArgumentCast<unsigned short>(arguments, 2, secondNumberPtrHolder, 
				&(numberLowerBound = (NULL != (*firstNumberPtrHolder))? ((int)(**firstNumberPtrHolder) + 1):0),
				&numberUpperBound)
			&& draw(*firstNumberPtrHolder, *secondNumberPtrHolder);

		delete firstNumberPtr;
		delete secondNumberPtr;

		return retVal;
	}
	else if (command.compare(L"showstatus") == 0)
	{
		return checkCallPrerequisites(MyCasinoUserTypes::Any)
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

		bool retVal = true;
		
		// if a user is logged in log out on server
		if (NULL != m_pSessionId)
			retVal &= bye();

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

		// shutdown interpreter
		m_pInterpreter->stop();

		return retVal;
	}
	else if (command.compare(L"help") == 0)
	{
		std::cout << "--------------------------------------------------------" << std::endl;
		std::cout << "Commands:" << std::endl;
		std::cout << "user <name> <passwort>" << std::endl;
		if(NULL == m_pUserType || MyCasinoUserTypes::Operator == *m_pUserType)
			std::cout << "payin* <name> <amount>" << std::endl;
		if (NULL == m_pUserType || MyCasinoUserTypes::Gamer == *m_pUserType)
			std::cout << "bet <amount> <first number> <second number>" << std::endl;
		std::cout << "showbets" << std::endl;
		if (NULL == m_pUserType || MyCasinoUserTypes::Operator == *m_pUserType)
			std::cout << "draw* [<first number> <second number>]" << std::endl;
		std::cout << "showstatus" << std::endl;
		std::cout << "help" << std::endl;
		std::cout << "bye" << std::endl;
		std::cout << "--------------------------------------------------------" << std::endl;

		return true;
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
