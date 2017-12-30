#pragma once
#include <windows.h>
#include <vector>
#include "MyCasinoTransaction.h"
#include <OAIdl.h>


class MyCasinoBet : public IMyCasinoTransactionInformation
{
public:
	MyCasinoBet(SHORT firstNumber, SHORT secondNumber, DOUBLE amount);
	~MyCasinoBet();

	BOOL SetBetResult(SHORT drawnFirstNumber, SHORT drawnSecondNumber, DOUBLE resultAmount);
	SHORT GetFirstNumber();
	SHORT GetSecondNumber();
	DOUBLE GetSetAmount();
	std::vector<TaggedUnion> GetInformation();
	SHORT GetInformationCount();
	BOOL ResultIsDrawn();

private:
	SHORT m_firstNumber;
	SHORT m_secondNumber;
	DOUBLE m_setAmount;
	SHORT m_drawnFirstNumber;
	SHORT m_drawnSecondNumber;
	DOUBLE m_resultAmount;
	BOOL m_isDrawn;
};