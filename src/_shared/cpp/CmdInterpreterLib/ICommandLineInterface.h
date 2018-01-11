#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <math.h>

enum ArgumentType
{
	DEFAULT = 0,
	MONEY
};


class ICommandLineInterface {
public:
	virtual bool ProcessCommand(std::vector<std::wstring>) = 0;
private:
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

	std::wstring doubleToScientificWString(double value,int precision=2)
	{
		std::wstringstream str;
		str << std::setprecision(precision) << value;
		return str.str();
	}

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
			std::wcerr << "Argument " << argument << " does not fit to boundaries: "  << boundaries << std::endl;
	}

protected:
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

		size_t maxSize=0;
		for (std::vector<size_t>::iterator it = validArgumentsCount.begin(); it != validArgumentsCount.end(); ++it)
			if (maxSize < (*it))
				maxSize = (*it);

		if (argumentsCountWithoutCommand > maxSize)
		{
			argumentCountIsValid = true;
			std::wcerr << "[Warning] Too many parameters for command " << arguments.at(0) << " - ignored last " << std::to_wstring(argumentsCountWithoutCommand-maxSize) << " arguments" << std::endl;
		}

		return argumentCountIsValid;
	}


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
			std::wcerr << "Argument " << arguments.at(position) << " is malformed and could not be interpreted" << std::endl;
			return false;
		}

	}


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
			std::wcerr << "Argument " << arguments.at(position) << " is malformed and could not be interpreted" << std::endl;
			return false;
		}
		
	}
};
