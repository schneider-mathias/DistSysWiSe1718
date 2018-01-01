#include "MyCasino.h"
#include <utility>

MyCasino::MyCasino():
	m_currentBetId(0),
	m_pOperator(NULL),
	m_pOperatorAccount(NULL)
{
}

MyCasino::~MyCasino()
{
}

BOOL MyCasino::Open(MyCasinoUser* user)
{
	if (IsOpened())
		return ERROR_MY_CASINO_HAS_ALREADY_OPERATOR;

	m_pOperator = user;
	MyCasinoAccount* account = NULL;
	if (!LoadAccount(*m_pOperator, &account))
		return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;
	m_pOperatorAccount = account;

	return TRUE;
}

BOOL MyCasino::IsOpened()
{
	return (NULL != m_pOperator);
}

BOOL MyCasino::IsOperator(MyCasinoUser& user)
{
	return IsOpened() && (*m_pOperator) == user;
}

BOOL MyCasino::LoadAccount(MyCasinoUser& user, MyCasinoAccount** account)
{
	// TODO
	(*account) = new MyCasinoAccount();

	//if (!IsOpened())
	//	return ERROR_MY_CASINO_NO_OPERATOR;


	return TRUE;
}

BOOL MyCasino::CalculateProfit(MyCasinoBet& bet, DOUBLE* rewardForOne, DOUBLE* rewardForTwo)
{
	// check if first and second number are valid for this game
	if (!IsValidBetNumber(bet.GetFirstNumber()) || !IsValidBetNumber(bet.GetSecondNumber()))
		return ERROR_MY_CASINO_BET_INVALID_NUMBER;
	
	*rewardForOne = bet.GetSetAmount() * MY_CASINO_BET_REWARD_FACTOR_FOR_ONE;
	*rewardForTwo = bet.GetSetAmount();

	for (std::map<MyCasinoUser, MyCasinoBet>::iterator it = m_currentBets.begin(); it != m_currentBets.end(); it++)
	{
		// skip value with same number (can happen if amount for this bet should be overwritten)
		if (bet.GetFirstNumber() != (*it).second.GetFirstNumber()
			&& bet.GetSecondNumber() != (*it).second.GetSecondNumber())
		{
			(*rewardForTwo) += (*it).second.GetSetAmount();
		}
	}
		

	return TRUE;
}

BOOL MyCasino::Bet(MyCasinoUser& user, SHORT firstNumber, SHORT secondNumber, DOUBLE setAmount)
{
	MyCasinoBet bet(m_currentBetId++,firstNumber,secondNumber,setAmount);
	BOOL resVal = FALSE;
	MyCasinoAccount* account = NULL;
	MyCasinoBet* existingBet = NULL;

	switch (CheckBet(user, bet))
	{
		case TRUE:
			// new bet, everything fine continue
			
			// check if operator account balance is sufficient for this bet
			resVal = CheckOperatorAccount(bet);
			if (FAILED(resVal))
				return resVal;

			// book transaction on gamer account
			if (!GetAccount(user, &account))
				return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

			ULONG transactionId;
			resVal = (account)->CreateTransaction(bet.GetSetAmount(), MyCasinoTransactionsTypes::PRELIMINARY_WITHDRAWAL, &bet, &transactionId);
			if (FAILED(resVal))
				return resVal;
			
			return TRUE;

		case INFORMATION_MY_CASINO_USER_HAS_NUMBERS:
			// user has already bet on this numbers, apply new values on bet and account
			
			// first check if new amount is 0, in this case delete the bet
			if (bet.GetSetAmount() < 0.001)
			{
				if (!DeleteBet(bet.GetFirstNumber(), bet.GetSecondNumber()))
					return ERROR_MY_CASINO_BET_CANNOT_DELETE;


				return TRUE;
			}

			// check if operator account balance is sufficient for this bet
			resVal = CheckOperatorAccount(bet);
			if (FAILED(resVal))
				return resVal;

			// check if gamer account balance is sufficient


			// overwrite amount in bet object
			existingBet = NULL;
			if (!GetBet(bet.GetFirstNumber(), bet.GetSecondNumber(), existingBet))
				return FALSE; // should never happen

			// overwrite amount in bet object

			return TRUE;
		case ERROR_MY_CASINO_BET_INVALID_NUMBER:
			return ERROR_MY_CASINO_BET_INVALID_NUMBER;
		case ERROR_MY_CASINO_BET_ALREADY_TAKEN:
			// same bet by different users is not allowed
			return ERROR_MY_CASINO_BET_ALREADY_TAKEN;
	}

	return FALSE;
}

BOOL MyCasino::DeleteBet(SHORT firstNumber, SHORT secondNumber)
{
	BOOL deleteSuccess = FALSE;
	for (std::map<MyCasinoUser, MyCasinoBet>::iterator it = m_currentBets.begin(); it != m_currentBets.end(); it++)
	{
		if (firstNumber == (*it).second.GetFirstNumber()
			&& secondNumber == (*it).second.GetSecondNumber()
			)
		{
			m_currentBets.erase(it);
			deleteSuccess = TRUE;
		}
	}

	return deleteSuccess;
}

BOOL MyCasino::GetBet(SHORT firstNumber, SHORT secondNumber, MyCasinoBet* bet)
{
	bet = NULL;
	for (std::map<MyCasinoUser, MyCasinoBet>::iterator it = m_currentBets.begin(); it != m_currentBets.end(); it++)
	{
		if (firstNumber == (*it).second.GetFirstNumber()
			&& secondNumber == (*it).second.GetSecondNumber()
			)
		{
			bet = &((*it).second);
		}
	}

	return (NULL != bet);
}

BOOL MyCasino::GetAccount(MyCasinoUser& user, MyCasinoAccount** account)
{
	*account = NULL;

	// operator account is not stored in user list
	if (user == *m_pOperator)
	{
		if (NULL == m_pOperatorAccount)
			return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

		*account = m_pOperatorAccount;
		return TRUE;
	}

	for (std::map<MyCasinoUser, MyCasinoAccount>::iterator it = m_userAccounts.begin(); it != m_userAccounts.end(); it++)
	{
		if (user == (*it).first)
		{
			*account = &((*it).second);
			return TRUE;
		}
	}

	if (NULL == *account)
	{
		MyCasinoAccount* loadedAccount = NULL;
		if(!LoadAccount(user, &loadedAccount))
			return FALSE;
		*account = loadedAccount;
	}
		
	return TRUE;
}

BOOL MyCasino::CheckOperatorAccount(MyCasinoBet& bet)
{
	double rewardForOne = 0.0;
	double rewardForTwo = 0.0;
	BOOL resVal = CalculateProfit(bet, &rewardForOne, &rewardForTwo);
	if (FAILED(resVal))
		return resVal;

	if (rewardForOne > m_pOperatorAccount->GetCurrentBalance() ||
		rewardForTwo > m_pOperatorAccount->GetCurrentBalance())
		return ERROR_MY_CASINO_OPERATOR_ACCOUNT_BALANCE_NOT_SUFFICIENT;

	return TRUE;
}


BOOL MyCasino::IsValidBetNumber(SHORT firstNumber)
{
	return (firstNumber >= MY_CASINO_BET_MIN_NUMBER && firstNumber <= MY_CASINO_BET_MAX_NUMBER);
}


BOOL MyCasino::CheckBet(MyCasinoUser& user, MyCasinoBet& bet)
{
	BOOL resVal = TRUE;

	// check if first and second number are valid for this game
	if (!IsValidBetNumber(bet.GetFirstNumber()) || !IsValidBetNumber(bet.GetSecondNumber()))
		return ERROR_MY_CASINO_BET_INVALID_NUMBER;

	for (std::map<MyCasinoUser, MyCasinoBet>::iterator it = m_currentBets.begin(); it != m_currentBets.end(); it++)
	{
		if (bet.GetFirstNumber() == (*it).second.GetFirstNumber()
			&& bet.GetSecondNumber() == (*it).second.GetSecondNumber()
			)
		{
			if (user == (*it).first)
				resVal = INFORMATION_MY_CASINO_USER_HAS_NUMBERS;
			else
				resVal = ERROR_MY_CASINO_BET_ALREADY_TAKEN;
			break;
		}
	}
	return resVal;
}


BOOL MyCasino::Deposit(MyCasinoUser& user, DOUBLE amount)
{
	MyCasinoAccount* account = NULL;

	if (!GetAccount(user, &account))
		return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

	ULONG transactionId;
	return (account)->CreateTransaction(amount, MyCasinoTransactionsTypes::DEPOSIT, NULL, &transactionId);
}

BOOL MyCasino::Withdraw(MyCasinoUser& user, DOUBLE amount)
{
	return E_NOTIMPL;
}

BOOL MyCasino::Draw()
{
	return E_NOTIMPL;
}

std::multimap<MyCasinoUser, MyCasinoBet> MyCasino::CreateSnapshot()
{
	std::list<MyCasinoBet> betsSnapshot;
	return m_currentBets;
}

BOOL MyCasino::Close()
{
	if (!IsOpened())
		return ERROR_MY_CASINO_NO_OPERATOR;

	// operator will be deleted by AuthService
	m_pOperator = NULL;
	delete m_pOperatorAccount;
	m_pOperatorAccount = NULL;

	return TRUE;
}
