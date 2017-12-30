#pragma once
#include <string>
#include "MyCasinoCommandLineInterface.h"

#include "../COMMyCasinoSrv/COMMyCasinoSrv_i.h"

class COMMyCasinoCommandLineInterface : public MyCasinoCommandLineInterface
{
public:
	COMMyCasinoCommandLineInterface(ICOMMyCasino* pICOMMyCasinoSrv);
	~COMMyCasinoCommandLineInterface();

	bool user(std::wstring user, std::wstring password);
	bool payin(std::wstring user, double amount);
	bool bet(double setAmount, unsigned short firstNumber, unsigned short secondNumber);
	bool showbets();
	bool draw(unsigned short* firstNumberTest, unsigned short* secondNumberTest);
	bool showstatus();
	bool bye();
private:
	ICOMMyCasino* m_pICOMMyCasinoSrv = NULL;

};