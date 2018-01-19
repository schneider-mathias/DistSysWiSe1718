/**---------------------------------------------------------------------
// project:	CmdInterpreterLib
// file:	ICommandLineInterface.h
//
// summary:	Declares the ICommandLineInterface interface
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			29.12.2018	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <math.h>

#include "CharStringConverter.h"

/** <summary>	Values that represent argument types. </summary> */
enum ArgumentType
{
	DEFAULT = 0,
	MONEY
};


class ICommandLineInterface {
public:

	/**---------------------------------------------------------------------
	 * <summary>	Process the command described by commandList. </summary>
	 *
	 * <param name="commandList">	Command as a vector that should be processed. </param>
	 *
	 * <returns>	True if it succeeds, false if it fails. </returns>
	 *-------------------------------------------------------------------**/

	virtual bool ProcessCommand(std::vector<std::wstring> commandList) = 0;
private:

	/**---------------------------------------------------------------------
	 * <summary>	Check if numeric value is between boundaries. </summary>
	 *
	 * <param name="number">  	Number to check. </param>
	 * <param name="minBound">	[in,out] (Optional) If non-null, the minimum bound. </param>
	 * <param name="maxBound">	[in,out] (Optional) If non-null, the maximum bound. </param>
	 *
	 * <returns>	True if it succeeds, false if it fails. </returns>
	 *-------------------------------------------------------------------**/

	bool checkNumericArgument(int number, double* minBound = nullptr, double* maxBound = nullptr)
	{
		if (NULL == minBound && NULL == maxBound)
			return true;
		else if (NULL != minBound && NULL == maxBound)
			return number >= *minBound;
		else if (NULL == minBound && NULL != maxBound)
			return number <= *maxBound;
		else
			return number >= *minBound && number <= *maxBound;
	}

	/**---------------------------------------------------------------------
	 * <summary>	Double to scientific string with precision. </summary>
	 *
	 * <param name="value">	   	The value to convert. </param>
	 * <param name="precision">	(Optional) The precision for the 
	 * 							double to string conversion. </param>
	 *
	 * <returns>	A std::wstring. </returns>
	 *-------------------------------------------------------------------**/

	std::wstring doubleToScientificWString(double value,int precision=2)
	{
		std::wstringstream str;
		str << std::setprecision(precision) << value;
		return str.str();
	}

	/**---------------------------------------------------------------------
	 * <summary>	Fill a string which contains information whether 
	 * 				an argument is within a range. </summary>
	 *
	 * <param name="argument">	[in,out] The argument. </param>
	 * <param name="minBound">	[in,out] If non-null, the minimum bound. </param>
	 * <param name="maxBound">	[in,out] If non-null, the maximum bound. </param>
	 *-------------------------------------------------------------------**/

	void outputNotInRange(std::wstring& argument, double* minBound, double* maxBound)
	{
		std::wstring boundaries;
		if (NULL != minBound && NULL != maxBound)
		{
			boundaries.append(doubleToScientificWString(*minBound));
			boundaries.append(L"-");
			boundaries.append(doubleToScientificWString(*maxBound));
		}
		else if (NULL != minBound && NULL == maxBound)
		{
			boundaries.append(L">=");
			boundaries.append(doubleToScientificWString(*minBound));
		}
		else if (NULL == minBound && NULL != maxBound)
		{
			boundaries.append(L"<=");
			boundaries.append(doubleToScientificWString(*maxBound));
		}

		if (!boundaries.empty())
			std::cerr << "Argument " << wstring_to_string(argument) << " does not fit to boundaries: "  << wstring_to_string(boundaries) << std::endl;
	}

protected:

	/**---------------------------------------------------------------------
	 * <summary>	Check if arguments size is valid </summary>
	 *
	 * <param name="arguments">			 	[in,out] The arguments. </param>
	 * <param name="minArgumentsCount">  	Number of minimum arguments. </param>
	 * <param name="validArgumentsCount">	[in,out] Number of valid arguments. </param>
	 *
	 * <returns>	True if it succeeds, false if it fails. </returns>
	 *-------------------------------------------------------------------**/

	bool checkCallArguments(std::vector<std::wstring>& arguments, size_t minArgumentsCount, std::vector<size_t>& validArgumentsCount)
	{
		bool argumentCountIsValid = true;

		size_t argumentsCountWithoutCommand = arguments.size() - 1;

		if (validArgumentsCount.size() > 0)
		{
			argumentCountIsValid = false;
			for (std::vector<size_t>::iterator it = validArgumentsCount.begin(); it != validArgumentsCount.end(); ++it)
			{
				if ((*it) == argumentsCountWithoutCommand)
				{
					argumentCountIsValid = true;
					break;
				}
			}
		}

		argumentCountIsValid &= (minArgumentsCount <= argumentsCountWithoutCommand);

		size_t maxSize = 0;
		for (std::vector<size_t>::iterator it = validArgumentsCount.begin(); it != validArgumentsCount.end(); ++it)
		{
			if (maxSize < (*it))
				maxSize = (*it);
		}

		if (argumentsCountWithoutCommand > maxSize)
		{
			argumentCountIsValid = true;
			std::cerr << "[Warning] Too many parameters for command " << wstring_to_string(arguments.at(0)) << " - ignored last " << std::to_string(argumentsCountWithoutCommand-maxSize) << " arguments" << std::endl;
		}
		else if (!argumentCountIsValid)
		{
			std::cerr << "[Error] Invalid number of parameters for command " << wstring_to_string(arguments.at(0)) << std::endl;
		}

		return argumentCountIsValid;
	}

	/**---------------------------------------------------------------------
	 * <summary>	Safe argument cast for pointer types. </summary>
	 *
	 * <typeparam name="T">	Generic type parameter. </typeparam>
	 * <param name="arguments">  	[in,out] The arguments. </param>
	 * <param name="position">   	The position that should be casted. </param>
	 * <param name="castedValue">	[in,out] If non-null, the casted value. </param>
	 * <param name="minBound">   	[in,out] (Optional) If non-null, the minimum bound. </param>
	 * <param name="maxBound">   	[in,out] (Optional) If non-null, the maximum bound. </param>
	 * <param name="type">		 	(Optional) The detailed type for conversion. </param>
	 *
	 * <returns>	True if it succeeds, false if it fails. </returns>
	 *-------------------------------------------------------------------**/

	template< typename T>
	bool safeArgumentCast(std::vector<std::wstring>& arguments, int position, T** castedValue, double* minBound = nullptr, double* maxBound = nullptr, ArgumentType type = ArgumentType::DEFAULT)
	{
		try
		{
			if (std::is_same<T, unsigned short>::value)
			{
				// first "argument" is command
				if (position > arguments.size() - 1)
				{
					delete *castedValue;
					*castedValue = nullptr;
					return true;
				}
				else
				{
					**castedValue = (unsigned short)stoi(arguments.at(position));
				}
			}
			else
			{
				std::cerr << "Method safeArgumentCast is unable to cast this type." << std::endl;
				return false;
			}

			bool isInRange = checkNumericArgument(**castedValue, minBound, maxBound);
			if (!isInRange)
				outputNotInRange(arguments.at(position), minBound, maxBound);

			return isInRange;

		}
		catch (...)
		{
			std::cerr << "Argument " << wstring_to_string(arguments.at(position)) << " is malformed and could not be interpreted" << std::endl;
			return false;
		}

	}

	/**---------------------------------------------------------------------
	 * <summary>	Safe argument cast for value types. </summary>
	 *
	 * <typeparam name="T">	Generic type parameter. </typeparam>
	 * <param name="arguments">  	[in,out] The arguments. </param>
	 * <param name="position">   	The position that should be casted. </param>
	 * <param name="castedValue">	[in,out] If non-null, the casted value. </param>
	 * <param name="minBound">   	[in,out] (Optional) If non-null, the minimum bound. </param>
	 * <param name="maxBound">   	[in,out] (Optional) If non-null, the maximum bound. </param>
	 * <param name="type">		 	(Optional) The detailed type for conversion. </param>
	 *
	 * <returns>	True if it succeeds, false if it fails. </returns>
	 *-------------------------------------------------------------------**/

	template< typename T>
	bool safeArgumentCast(std::vector<std::wstring>& arguments, int position, T* castedValue, double* minBound = nullptr, double* maxBound = nullptr, ArgumentType type = ArgumentType::DEFAULT)
	{
		try
		{
			if (NULL == castedValue)
				return false;

			if (std::is_same<T, unsigned short>::value)
			{
				// first "argument" is command
				if (position > arguments.size() - 1)
				{
					delete castedValue;
					castedValue = nullptr;
					return true;
				}
				else
				{
					*castedValue = (unsigned short)stoi(arguments.at(position));
				}
			}
			else if (std::is_same<T, double>::value)
			{
				*castedValue = stod(arguments.at(position));
				if(type==ArgumentType::MONEY)
					*castedValue = floorf(*castedValue * 100) / 100;

			}
			else
			{
				std::cerr << "Method safeArgumentCast is unable to cast this type." << std::endl;
				return false;
			}
			
			bool isInRange = checkNumericArgument(*castedValue, minBound, maxBound);
			if (!isInRange)
				outputNotInRange(arguments.at(position), minBound, maxBound);

			return isInRange;
				
		}
		catch (...)
		{
			std::cerr << "Argument " << wstring_to_string(arguments.at(position)) << " is malformed and could not be interpreted" << std::endl;
			return false;
		}
		
	}
};
