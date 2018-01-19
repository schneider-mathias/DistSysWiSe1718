/**---------------------------------------------------------------------
// project:	MyCasinoClnLib
// file:	MyCasinoCommandLineInterface.cpp
//
// summary:	Implements my casino command line interface class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			29.12.2018	Mathias Schneider	Created
//			XXXXXXXXXX  Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#include "MyCasinoCommandLineInterface.h"
#include "IncPub/MyCasinoDefines.h"
#include <iostream>
#include "comdef.h"
#include "HelperLib/CharStringConverter.h"

/**---------------------------------------------------------------------
 * <summary>	Constructor. </summary>
 *
 * <param name="interpreter">	[in,out] If non-null, the command line 
 * 								interpreter. </param>
 *-------------------------------------------------------------------**/

MyCasinoCommandLineInterface::MyCasinoCommandLineInterface(CmdInterpreter* interpreter)
	:m_pSessionId(NULL),
	m_pUserType(NULL),
	m_pCmdInterpreter(interpreter)
{
}


/** <summary>	Destructor. </summary> */
MyCasinoCommandLineInterface::~MyCasinoCommandLineInterface()
{
	if (NULL != m_pSessionId)
		delete m_pSessionId;
	if (NULL != m_pUserType)
		delete m_pUserType;

	m_pCmdInterpreter = NULL;
}

/**---------------------------------------------------------------------
 * <summary>	Handler, called after a command was executed in order to 
 * 				evaluate the return code and display the corresponding message. </summary>
 *
 * <param name="additionalInformation">	Additional information for the displaying 
 * 										the output. </param>
 * <param name="code">				   	The return code. </param>
 * <param name="msg">				   	[in,out] The message. </param>
 *-------------------------------------------------------------------**/

void MyCasinoCommandLineInterface::resultHandler(std::string additionalInformation, long code, std::string& msg)
{
	if (code > 0)
	{
		std::cout << "[INFO] " << msg << std::endl;
	}
	else if ((long)(code) == 0x800706BA) // COM error 
	{
		std::cerr << "[ERROR] " << "No server connection. Stop client." << std::endl;
		m_pCmdInterpreter->stop();
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

/**---------------------------------------------------------------------
 * <summary>	Dispatcher method in order to process the command described by 
 * 				arguments as defined by the MyCasino specication. Before running 
 * 				the commands which will be implemented in child class, all arguments 
 * 				checks are performed. Also user type is checked. </summary>
 *
 * <param name="arguments">	The arguments. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool MyCasinoCommandLineInterface::ProcessCommand(std::vector<std::wstring> arguments)
{
	std::wstring command(arguments.at(0));

	if (command.compare(L"user") == 0)
	{
		// two arguments {username} {password}
		bool resVal = checkCallArguments(arguments, 2, std::vector<size_t>() = { 2 })
			&& user(arguments.at(1), arguments.at(2));

		if(resVal)
			std::cout << "Logged in, Id: " << (*m_pSessionId) << ", User Type: " << ((*m_pUserType) ? "gamer" : "operator") << std::endl;

		return resVal;
	}
	else if (command.compare(L"payin") == 0)
	{
		double amountMoney;
		double amountMoneyLowerBound = 0;
		double amountMoneyUpperBound = UPPER_MONEY_BOUNDARY;

		// two arguments {username} {amount | money double} 
		// only callable by operator
		bool resVal=checkCallPrerequisites(MyCasinoUserTypes::Operator)
			&& checkCallArguments(arguments, 2, std::vector<size_t>()= { 2 })
			&& safeArgumentCast<double>(arguments, 2, &amountMoney, &amountMoneyLowerBound, &amountMoneyUpperBound, ArgumentType::MONEY)
			&& payin(arguments.at(1), amountMoney);

		if(resVal)
			std::cout << "Payed " << amountMoney << " in for user " << wstring_to_string(arguments.at(1)) << std::endl;

		return resVal;
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

		// three arguments {amount | money double} {firstNumber | && > 1 && < 4} {secondNumber | > firstNumber && > 2 && < 5}
		// only callable by gamer (cheked on server-side)
		bool resVal = checkCallPrerequisites(MyCasinoUserTypes::Any)
			&& checkCallArguments(arguments, 3, std::vector<size_t>() = { 3 })
			&& safeArgumentCast<double>(arguments, 1, &setMoney, &setMoneyLowerBound, &setMoneyUpperBound, ArgumentType::MONEY)
			&& safeArgumentCast<unsigned short>(arguments, 2, &firstNumber, &numberLowerBound, &firstNumberUpperBound)
			&& safeArgumentCast<unsigned short>(arguments, 3, &secondNumber, &(numberLowerBound = (firstNumber + 1)), &numberUpperBound)
			&& bet(setMoney, firstNumber, secondNumber);

		if (resVal)
			std::cout << "Placed bet " << setMoney << " on number " << firstNumber << " and " << secondNumber << std::endl;

		return resVal;
	}
	else if (command.compare(L"showbets") == 0)
	{
		checkCallArguments(arguments, 0, std::vector<size_t>() = { 0 });

		// no arguments
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

		// none or two arguments {firstNumber | && > 1 && < 4} {secondNumber | > firstNumber && > 2 && < 5}
		// only callable by operator
		// if first number pointer is set, it becomes the lower boundary, otherwise set it to 0 (will be ignored anyway)
		bool retVal = checkCallPrerequisites(MyCasinoUserTypes::Operator)
			&& checkCallArguments(arguments, 0, std::vector<size_t>() = { 0, 2 })
			&& safeArgumentCast<unsigned short>(arguments, 1, firstNumberPtrHolder, &numberLowerBound, &firstNumberUpperBound)
			&& safeArgumentCast<unsigned short>(arguments, 2, secondNumberPtrHolder, 
				&(numberLowerBound = (NULL != (*firstNumberPtrHolder))? ((int)(**firstNumberPtrHolder) + 1):0),
				&numberUpperBound)
			&& draw(firstNumberPtrHolder, secondNumberPtrHolder);


		if(retVal)
			std::cout << "First number: " << **firstNumberPtrHolder << ", second number: " << **secondNumberPtrHolder << std::endl;

		delete *firstNumberPtrHolder;
		delete *secondNumberPtrHolder;

		return retVal;
	}
	else if (command.compare(L"showstatus") == 0)
	{
		// no arguments
		return checkCallPrerequisites(MyCasinoUserTypes::Any)
			&& checkCallArguments(arguments, 0, std::vector<size_t>() = { 0 })
			&& showstatus();
	}
	else if (command.compare(L"bye") == 0)
	{
		// no arguments
		checkCallArguments(arguments, 0, std::vector<size_t>() = { 0 });

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
		m_pCmdInterpreter->stop();

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
		std::cout << "[ERROR] Invalid command " << wstring_to_string(arguments.at(0)) << std::endl;
	}

	return false;
}

/**---------------------------------------------------------------------
 * <summary>	Check call prerequisites (logged in and permission level of user type). </summary>
 *
 * <param name="minRequiredUserType">	Type of the minimum required user. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

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

/**---------------------------------------------------------------------
 * <summary>	Query if a client is already logged in. </summary>
 *
 * <returns>	True if logged in, false if not. </returns>
 *-------------------------------------------------------------------**/

bool MyCasinoCommandLineInterface::isLoggedIn()
{
	return NULL != m_pSessionId;
}

/**---------------------------------------------------------------------
 * <summary>	Check whether the user fullfills permission level. </summary>
 *
 * <param name="minRequiredUserType">	Type of the minimum required user. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-------------------------------------------------------------------**/

bool MyCasinoCommandLineInterface::fullfillsPermissionLevel(short minRequiredUserType)
{
	return (*m_pUserType) <= minRequiredUserType;
}
