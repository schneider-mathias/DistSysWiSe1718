#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

enum ArgumentType
{
	DEFAULT = 0,
	MONEY
};


class ICommandLineInterface {
public:
	virtual bool ProcessCommand(std::vector<std::wstring>) = 0;
protected:
	bool checkNumericArgument(int number, int* minBound = nullptr, int* maxBound = nullptr)
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

		if (!argumentCountIsValid)
		{
			std::wcerr << "Invalid number of parameters for command " << arguments.at(0) << std::endl;
		}

		return argumentCountIsValid;
	}

	template< typename T>
	bool safeArgumentCast(std::vector<std::wstring>& arguments, int position, T* castedValue, int* minBound = nullptr, int* maxBound = nullptr, ArgumentType type = ArgumentType::DEFAULT)
	{
		try
		{
			if (NULL == castedValue)
				return false;

			if (std::is_same<T, unsigned short>::value)
				*castedValue = (unsigned short)stoi(arguments.at(position));
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
			{
				std::wstring boundaries;
				if (NULL != minBound && NULL != maxBound)
				{
					boundaries.append(std::to_wstring(*minBound));
					boundaries.append(L"-");
					boundaries.append(std::to_wstring(*maxBound));
				}
				else if (NULL != minBound && NULL == maxBound)
				{
					boundaries.append(L">=");
					boundaries.append(std::to_wstring(*minBound));
				}
				else if (NULL == minBound && NULL != maxBound)
				{
					boundaries.append(L"<=");
					boundaries.append(std::to_wstring(*maxBound));
				}

				if (!boundaries.empty())
					std::wcerr << "Argument " << arguments.at(position) << " does not fit to boundaries: " << boundaries << std::endl;
			}

			return isInRange;
				
		}
		catch (...)
		{
			std::wcerr << "Argument " << arguments.at(position) << " is malformed and could not be interpreted" << std::endl;
			return false;
		}
		
	}
};
