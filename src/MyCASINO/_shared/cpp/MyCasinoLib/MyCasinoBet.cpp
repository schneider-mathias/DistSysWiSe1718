#include "MyCasinoBet.h"

MyCasinoBet::MyCasinoBet(ULONG id,SHORT firstNumber, SHORT secondNumber, DOUBLE amount)
	: IMyCasinoTransactionInformation(id, MyCasinoTransactionsInformationTypes::Bet),
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
	
	m_drawnFirstNumber = drawnFirstNumber;
	m_drawnSecondNumber = drawnSecondNumber;
	m_resultAmount = resultAmount;
	m_isDrawn = TRUE;
	return TRUE;
}

SHORT MyCasinoBet::GetFirstNumber()
{
	return m_firstNumber;
}

SHORT MyCasinoBet::GetSecondNumber()
{
	return m_secondNumber;
}

DOUBLE MyCasinoBet::GetSetAmount()
{
	return m_setAmount;
}

void MyCasinoBet::SetWager(DOUBLE wager)
{
	if(wager > 0.0)
		m_setAmount = wager;
}

BOOL MyCasinoBet::ResultIsDrawn()
{
	return m_isDrawn;
}

std::vector<TaggedUnion> MyCasinoBet::GetInformation()
{
	std::vector<TaggedUnion> betInformation;
	betInformation.push_back(TaggedUnion(m_firstNumber));
	betInformation.push_back(TaggedUnion(m_secondNumber));
	betInformation.push_back(TaggedUnion(m_setAmount));
	betInformation.push_back(TaggedUnion(m_isDrawn));
	if(m_isDrawn)
	{
		betInformation.push_back(TaggedUnion(m_drawnFirstNumber));
		betInformation.push_back(TaggedUnion(m_drawnSecondNumber));
		betInformation.push_back(TaggedUnion(m_resultAmount));
	}

	return betInformation;
}

SHORT MyCasinoBet::GetInformationCount()
{
	return m_isDrawn? 7:4;
}

bool MyCasinoBet::operator==(const MyCasinoBet& ref)
{
	return ref.m_id == m_id;
}

bool MyCasinoBet::operator<(const MyCasinoBet& ref)
{
	return ref.m_id < m_id;
}