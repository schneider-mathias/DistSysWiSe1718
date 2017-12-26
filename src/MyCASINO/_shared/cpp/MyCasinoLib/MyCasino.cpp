#include "MyCasino.h"


MyCasino::MyCasino()
{
}

MyCasino::~MyCasino()
{
}

BOOL MyCasino::Open(MyCasinoUser user)
{
	return E_NOTIMPL;
}

BOOL MyCasino::Bet(MyCasinoUser user, MyCasinoBet bet)
{
	return E_NOTIMPL;
}

BOOL MyCasino::Deposit(MyCasinoUser user, DOUBLE amount)
{
	return E_NOTIMPL;
}

BOOL MyCasino::Withdraw(MyCasinoUser user, DOUBLE amount)
{
	return E_NOTIMPL;
}

std::list<MyCasinoBet> MyCasino::CreateSnapshot()
{
	std::list<MyCasinoBet> betsSnapshot;
	return betsSnapshot;
}

BOOL MyCasino::Close()
{
	return E_NOTIMPL;
}
