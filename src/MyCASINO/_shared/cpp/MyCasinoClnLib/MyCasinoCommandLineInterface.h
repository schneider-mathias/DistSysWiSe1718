#pragma once
#include "ICommandLineInterface.h"

class MyCasinoCommandLineInterface : public ICommandLineInterface
{
public:
	MyCasinoCommandLineInterface();
	~MyCasinoCommandLineInterface();

	bool ProcessCommand(std::vector<std::wstring>);

	virtual bool user(std::wstring user, std::wstring password) = 0;
	virtual bool payin(std::wstring user, double amount) = 0;
	virtual bool bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber) = 0;
	virtual bool showbets() = 0;
	virtual bool draw(unsigned short* firstNumberTest = nullptr, unsigned short* secondNumberTest = nullptr) = 0;
	virtual bool showstatus() = 0;
	virtual bool bye() = 0;

protected:
	unsigned long* m_pSessionId;
	short* m_pUserType;
};

