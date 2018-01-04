#pragma once
#include <string>
#include "MyCasinoCommandLineInterface.h"


class RPCMyCasinoCommandLineInterface : public MyCasinoCommandLineInterface
{
public:
	RPCMyCasinoCommandLineInterface(CmdInterpreter* interpreter);
	~RPCMyCasinoCommandLineInterface();

	bool user(std::wstring user, std::wstring password);
	bool payin(std::wstring user, double amount);
	bool bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber);
	bool showbets();
	bool draw(unsigned short* firstNumberTest, unsigned short* secondNumberTest);
	bool showstatus();
	bool bye();

};
