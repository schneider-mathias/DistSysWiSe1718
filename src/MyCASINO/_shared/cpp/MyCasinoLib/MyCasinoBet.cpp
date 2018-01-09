#include "MyCasinoBet.h"
#include "ScopedLock.h"

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

MyCasinoBet::~MyCasinoBet()
{
}

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

std::wstring MyCasinoBet::GetUsername()
{
	SCOPED_LOCK(m_betMutex);
	return m_username;
}

SHORT MyCasinoBet::GetFirstNumber()
{
	SCOPED_LOCK(m_betMutex);
	return m_firstNumber;
}

SHORT MyCasinoBet::GetSecondNumber()
{
	SCOPED_LOCK(m_betMutex);
	return m_secondNumber;
}

DOUBLE MyCasinoBet::GetSetAmount()
{
	SCOPED_LOCK(m_betMutex);
	return m_setAmount;
}

void MyCasinoBet::SetWager(DOUBLE wager)
{
	if (wager > 0.0)
	{
		SCOPED_LOCK(m_betMutex);
		m_setAmount = wager;
	}
}

BOOL MyCasinoBet::ResultIsDrawn()
{
	SCOPED_LOCK(m_betMutex);
	return m_isDrawn;
}

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

SHORT MyCasinoBet::GetInformationCount()
{
	SCOPED_LOCK(m_betMutex);
	return m_isDrawn? BET_FULL_DETAILS_PROPTERY_COUNT : BET_DETAILS_PROPTERY_COUNT;
}

bool MyCasinoBet::operator==(const MyCasinoBet& ref)
{
	return ref.m_id == m_id;
}

bool MyCasinoBet::operator<(const MyCasinoBet& ref)
{
	return ref.m_id < m_id;
}