#pragma once
#include "windows.h"


class MyCasinoBet
{
public:
	MyCasinoBet(SHORT firstNumber, SHORT secondNumber, DOUBLE amount);
	~MyCasinoBet();

	BOOL SetBetResult(SHORT drawnFirstNumber, SHORT drawnSecondNumber);


private:
	SHORT m_firstNumber;
	SHORT m_secondNumber;
	DOUBLE m_setAmount;
	SHORT m_drawnFirstNumber;
	SHORT m_drawnSecondNumber;
	DOUBLE m_resultAmount;
};