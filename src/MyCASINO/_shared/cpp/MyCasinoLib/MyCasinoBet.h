/**---------------------------------------------------------------------
// project:	MyCasinoLib
// file:	MyCasinoBet.h
//
// summary:	Declares a thread-safe bet class storing all information about a bet including
//			wager, chosen numbers. After a bet was drawn it also provides infomation
//			about drawn numbers and resulting reward. Implements IMyCasinoTransactionInformation
//			interface so that it can be used as a detailed transaction information object.
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			24.12.2017	Mathias Schneider	Created
//			XXXXXXXXXX	Mathias Schneider	Changed
 *-------------------------------------------------------------------**/

#pragma once
#include <windows.h>
#include <vector>
#include "MyCasinoTransaction.h"

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