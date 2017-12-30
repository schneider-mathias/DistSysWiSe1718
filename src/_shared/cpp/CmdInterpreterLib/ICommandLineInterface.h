#pragma once
#include <vector>
#include <string>

class ICommandLineInterface {
public:
	virtual bool ProcessCommand(std::vector<std::wstring>) = 0;
};
