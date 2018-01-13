/**--------------------------------------------------------------------------------------------------
// project:	MyCasinoLib
// file:	MyCasinoBet.cpp
//
// summary:	Implements my casino bet class
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			13.01.2018	Mathias Schneider	Created
 *-----------------------------------------------------------------------------------------------**/

#include "MyCasinoBet.h"
#include "ScopedLock.h"

/**--------------------------------------------------------------------------------------------------
 * <summary>	Constructor. </summary>
 *
 * <param name="username">	  	The username. </param>
 * <param name="id">		  	The identifier. </param>
 * <param name="firstNumber"> 	The first number. </param>
 * <param name="secondNumber">	The second number. </param>
 * <param name="amount">	  	The amount. </param>
 *-----------------------------------------------------------------------------------------------**/

MyCasinoBet::MyCasinoBet(std::wstring username, ULONG id,SHORT firstNumber, SHORT secondNumber, DOUBLE amount)
	: IMyCasinoTransactionInformation(id, MyCasinoTransactionsInformationTypes::Bet),
	m_username(username),
	m_id(id),
	m_firstNumber(firstNumber),
	m_secondNumber(secondNumber),
	m_setAmount(amount),
	m_drawnFirstNumber(0),
	m_drawnSecondNumber(0),
	m_resultAmount(0),
	m_isDrawn(FALSE)
{
}

/** <summary>	Destructor. </summary> */
MyCasinoBet::~MyCasinoBet()
{
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Sets bet result. </summary>
 *
 * <param name="drawnFirstNumber"> 	The drawn first number. </param>
 * <param name="drawnSecondNumber">	The drawn second number. </param>
 * <param name="resultAmount">	   	The result amount. </param>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL MyCasinoBet::SetBetResult(SHORT drawnFirstNumber, SHORT drawnSecondNumber, DOUBLE resultAmount)
{
	if (m_isDrawn)
		return FALSE;
	
	SCOPED_LOCK(m_betMutex);

	m_drawnFirstNumber = drawnFirstNumber;
	m_drawnSecondNumber = drawnSecondNumber;
	m_resultAmount = resultAmount;
	m_isDrawn = TRUE;
	return TRUE;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets the username. </summary>
 *
 * <returns>	The username. </returns>
 *-----------------------------------------------------------------------------------------------**/

std::wstring MyCasinoBet::GetUsername()
{
	SCOPED_LOCK(m_betMutex);
	return m_username;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets the first number. </summary>
 *
 * <returns>	The first number. </returns>
 *-----------------------------------------------------------------------------------------------**/

SHORT MyCasinoBet::GetFirstNumber()
{
	SCOPED_LOCK(m_betMutex);
	return m_firstNumber;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets second number. </summary>
 *
 * <returns>	The second number. </returns>
 *-----------------------------------------------------------------------------------------------**/

SHORT MyCasinoBet::GetSecondNumber()
{
	SCOPED_LOCK(m_betMutex);
	return m_secondNumber;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets set amount. </summary>
 *
 * <returns>	The set amount. </returns>
 *-----------------------------------------------------------------------------------------------**/

DOUBLE MyCasinoBet::GetSetAmount()
{
	SCOPED_LOCK(m_betMutex);
	return m_setAmount;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Sets a wager. </summary>
 *
 * <param name="wager">	The wager. </param>
 *-----------------------------------------------------------------------------------------------**/

void MyCasinoBet::SetWager(DOUBLE wager)
{
	if (wager > 0.0)
	{
		SCOPED_LOCK(m_betMutex);
		m_setAmount = wager;
	}
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Determines if we can result is drawn. </summary>
 *
 * <returns>	True if it succeeds, false if it fails. </returns>
 *-----------------------------------------------------------------------------------------------**/

BOOL MyCasinoBet::ResultIsDrawn()
{
	SCOPED_LOCK(m_betMutex);
	return m_isDrawn;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets the information. </summary>
 *
 * <returns>	The information. </returns>
 *-----------------------------------------------------------------------------------------------**/

std::vector<TaggedUnion> MyCasinoBet::GetInformation()
{
	std::vector<TaggedUnion> betInformation;
	SCOPED_LOCK(m_betMutex);

	betInformation.push_back(&m_username);
	betInformation.push_back(m_firstNumber);
	betInformation.push_back(m_secondNumber);
	betInformation.push_back(m_setAmount);
	if(m_isDrawn)
	{
		betInformation.push_back(m_drawnFirstNumber);
		betInformation.push_back(m_drawnSecondNumber);
		betInformation.push_back(m_resultAmount);
	}

	return betInformation;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Gets information count. </summary>
 *
 * <returns>	The information count. </returns>
 *-----------------------------------------------------------------------------------------------**/

SHORT MyCasinoBet::GetInformationCount()
{
	SCOPED_LOCK(m_betMutex);
	return m_isDrawn? BET_FULL_DETAILS_PROPTERY_COUNT : BET_DETAILS_PROPTERY_COUNT;
}

/**--------------------------------------------------------------------------------------------------
 * <summary>	Equality operator. </summary>
 *
 * <param name="ref">	The reference. </param>
 *
 * <returns>	True if the parameters are considered equivalent. </returns>
 *-----------------------------------------------------------------------------------------------**/

bool MyCasinoBet::operator==(const MyCasinoBet& ref)
{
	return ref.m_id == m_id;
}

 /** <summary>	. </summary> */
bool MyCasinoBet::operator<(const MyCasinoBet& ref)
{
	return ref.m_id < m_id;
}