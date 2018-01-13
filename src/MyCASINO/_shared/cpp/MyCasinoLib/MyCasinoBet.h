/**--------------------------------------------------------------------------------------------------
// project:	MyCasinoLib
// file:	MyCasinoBet.h
//
// summary:	Declares my casino bet class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#pragma once
#include <windows.h>
#include <vector>
#include "MyCasinoTransaction.h"
#include <OAIdl.h>

#define MY_CASINO_BET_REWARD_FACTOR_FOR_ONE 2

#define MY_CASINO_BET_MIN_NUMBER 1
#define MY_CASINO_BET_MAX_NUMBER 5

class MyCasinoBet : public IMyCasinoTransactionInformation
{
public:
	MyCasinoBet(std::wstring username, ULONG id, SHORT firstNumber, SHORT secondNumber, DOUBLE amount);
	~MyCasinoBet();

	BOOL SetBetResult(SHORT drawnFirstNumber, SHORT drawnSecondNumber, DOUBLE resultAmount);
	std::wstring GetUsername();
	SHORT GetFirstNumber();
	SHORT GetSecondNumber();
	DOUBLE GetSetAmount();
	std::vector<TaggedUnion> GetInformation();
	SHORT GetInformationCount();
	BOOL ResultIsDrawn();
	bool operator==(const MyCasinoBet&);
	bool operator<(const MyCasinoBet&);
	void SetWager(DOUBLE wager);

private:
	std::wstring m_username;
	ULONG m_id;
	SHORT m_firstNumber;
	SHORT m_secondNumber;
	DOUBLE m_setAmount;
	SHORT m_drawnFirstNumber;
	SHORT m_drawnSecondNumber;
	DOUBLE m_resultAmount;
	BOOL m_isDrawn;
	std::mutex m_betMutex;
};