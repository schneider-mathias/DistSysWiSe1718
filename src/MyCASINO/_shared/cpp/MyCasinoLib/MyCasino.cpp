#include "MyCasino.h"
#include <utility>
#include <cstdlib>
#include <time.h>

MyCasino::MyCasino():
	m_currentBetId(0),
	m_pOperator(NULL),
	m_pOperatorAccount(NULL)
{
	srand((ULONG)time(NULL));

	InitializeCriticalSection(&m_critSection);
}

MyCasino::~MyCasino()
{
	DeleteCriticalSection(&m_critSection);

	// finally delete all bet objects
	for (auto it = m_formerBets.begin(); it != m_formerBets.end(); it++)
	{
		delete (*it);
	}
}

BOOL MyCasino::Open(MyCasinoUser* user)
{
	// already opened with specified user as operator
	if (IsOperator(*user))
		return TRUE;

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

BOOL MyCasino::LoadAccount(const MyCasinoUser& user, MyCasinoAccount** account)
{
	// TODO
	(*account) = new MyCasinoAccount();

	//if (!IsOpened())
	//	return ERROR_MY_CASINO_NO_OPERATOR;


	return TRUE;
}


// calculate profit for given bet
BOOL MyCasino::CalculateProfit(MyCasinoBet& bet, DOUBLE* rewardForOne, DOUBLE* rewardForTwo)
{
	// check if first and second number are valid for this game
	if (!IsValidBetNumber(bet.GetFirstNumber()) || !IsValidBetNumber(bet.GetSecondNumber()))
		return ERROR_MY_CASINO_BET_INVALID_NUMBER;
	
	// one number gives doouble wager back
	*rewardForOne = bet.GetSetAmount() * MY_CASINO_BET_REWARD_FACTOR_FOR_ONE;
	
	// two number right gives additionally all wagers as reward
	*rewardForTwo = *rewardForOne + bet.GetSetAmount();

	for (std::multimap<MyCasinoUser, MyCasinoBet*>::iterator it = m_currentBets.begin(); it != m_currentBets.end(); it++)
	{
		// skip value with same number (can happen if amount for this bet should be overwritten)
		if (!(bet.GetFirstNumber() == (*it).second->GetFirstNumber()
			&& bet.GetSecondNumber() == (*it).second->GetSecondNumber()))
		{
			(*rewardForTwo) += (*it).second->GetSetAmount();
		}
	}
		

	return TRUE;
}

BOOL MyCasino::Bet(MyCasinoUser& user, SHORT firstNumber, SHORT secondNumber, DOUBLE setAmount)
{
	MyCasinoBet* bet = NULL;
	MyCasinoBet* existingBet = NULL;
	// if bet does not exists yet create a new bet
	if (!GetBet(firstNumber, secondNumber, existingBet))
		bet = new MyCasinoBet(m_currentBetId++, firstNumber, secondNumber, setAmount);

	BOOL resVal = FALSE;
	MyCasinoAccount* account = NULL;
	
	MyCasinoTransactionsInformationTypes infoType = MyCasinoTransactionsInformationTypes::Bet;
	switch (CheckBet(user, *bet))
	{
		case TRUE:
			// new bet, everything fine continue
			
			// check if operator account balance is sufficient for this bet
			// ToDo: do we have to use 
			resVal = CheckOperatorAccount(*bet);
			if (FAILED(resVal))
				return resVal;

			// book transaction on gamer account
			if (!GetAccount(user, &account))
				return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

			ULONG transactionId;
			resVal = (account)->CreateTransaction(bet->GetSetAmount(), MyCasinoTransactionsTypes::BET_WAGER, bet, &infoType, &transactionId);
			if (FAILED(resVal))
				return resVal;

			// book wager income for operator
			resVal = (m_pOperatorAccount)->CreateTransaction(bet->GetSetAmount(), MyCasinoTransactionsTypes::DEPOSIT, bet, &infoType, &transactionId);
			if (FAILED(resVal))
				return resVal;


			// add to current bets
			m_currentBets.insert(std::pair<MyCasinoUser, MyCasinoBet*>(user, bet));
			
			return TRUE;

		case INFORMATION_MY_CASINO_USER_HAS_NUMBERS:
			// user has already bet on this numbers, apply new values on bet and account
			
			// first check if new amount is 0, in this case delete the bet
			if (bet->GetSetAmount() < 0.001)
			{
				if (!DeleteBet(bet->GetFirstNumber(), bet->GetSecondNumber()))
					return ERROR_MY_CASINO_BET_CANNOT_DELETE;


				return TRUE;
			}

			// check if operator account balance is sufficient for this bet
			resVal = CheckOperatorAccount(*bet);
			if (FAILED(resVal))
				return resVal;

			// check if gamer account balance is sufficient


			// overwrite amount in bet object
			existingBet = NULL;
			if (!GetBet(bet->GetFirstNumber(), bet->GetSecondNumber(), existingBet))
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
	for (std::multimap<MyCasinoUser, MyCasinoBet*>::iterator it = m_currentBets.begin(); it != m_currentBets.end(); it++)
	{
		if (firstNumber == (*it).second->GetFirstNumber()
			&& secondNumber == (*it).second->GetSecondNumber()
			)
		{
			m_formerBets.push_back((*it).second);
			m_currentBets.erase(it);
			deleteSuccess = TRUE;
		}
	}

	return deleteSuccess;
}

BOOL MyCasino::GetBet(SHORT firstNumber, SHORT secondNumber, MyCasinoBet* bet)
{
	bet = NULL;
	for (std::multimap<MyCasinoUser, MyCasinoBet*>::iterator it = m_currentBets.begin(); it != m_currentBets.end(); it++)
	{
		if (firstNumber == (*it).second->GetFirstNumber()
			&& secondNumber == (*it).second->GetSecondNumber()
			)
		{
			bet = ((*it).second);
		}
	}

	return (NULL != bet);
}

BOOL MyCasino::GetAccount(const MyCasinoUser& user, MyCasinoAccount** account)
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

	for (std::map<MyCasinoUser, MyCasinoAccount*>::iterator it = m_userAccounts.begin(); it != m_userAccounts.end(); it++)
	{
		if (user == (*it).first)
		{
			*account = ((*it).second);
			return TRUE;
		}
	}

	if (NULL == *account)
	{
		MyCasinoAccount* loadedAccount = NULL;
		if(!LoadAccount(user, &loadedAccount))
			return FALSE;
		*account = loadedAccount;

		m_userAccounts.insert(std::pair<MyCasinoUser, MyCasinoAccount*>(user, *account));
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

	// wager ist not transferred to operators account yet, but in order to determine 
	// whether balance is sufficient also take this additional amount to consideration
	if (rewardForOne > m_pOperatorAccount->GetCurrentBalance() + bet.GetSetAmount() ||
		rewardForTwo > m_pOperatorAccount->GetCurrentBalance() + bet.GetSetAmount())
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

	for (std::multimap<MyCasinoUser, MyCasinoBet*>::iterator it = m_currentBets.begin(); it != m_currentBets.end(); it++)
	{
		if (bet.GetFirstNumber() == (*it).second->GetFirstNumber()
			&& bet.GetSecondNumber() == (*it).second->GetSecondNumber()
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
	return (account)->CreateTransaction(amount, MyCasinoTransactionsTypes::DEPOSIT, NULL, NULL, &transactionId);
}

BOOL MyCasino::Withdraw(MyCasinoUser& user, DOUBLE amount)
{
	return E_NOTIMPL;
}

BOOL MyCasino::Draw(SHORT* firstNumber, SHORT* secondNumber)
{
	if (NULL == firstNumber)
		*firstNumber = GenerateDrawNumber();
	if (NULL == firstNumber)
		*secondNumber = GenerateDrawNumber();

	BOOL resVal = TRUE;
	DOUBLE rewardForOne = 0.0;
	DOUBLE rewardForTwo = 0.0;
	DOUBLE totalReward = 0.0;
	MyCasinoAccount* account;
	ULONG currentTransactionId = 0;
	for (std::multimap<MyCasinoUser, MyCasinoBet*>::iterator it = m_currentBets.begin(); it != m_currentBets.end();)
	{
		totalReward = 0.0;
		rewardForOne = 0.0;
		rewardForTwo = 0.0;

		resVal = CalculateProfit(*(*it).second, &rewardForOne, &rewardForTwo);
		if (FAILED(resVal)) 
			return resVal; // should never happen

		if (*firstNumber == (*it).second->GetFirstNumber() && *secondNumber == (*it).second->GetSecondNumber())
			totalReward = rewardForTwo;
		else if (*firstNumber == (*it).second->GetFirstNumber() || *secondNumber == (*it).second->GetFirstNumber())
			totalReward = rewardForOne;

		// book transaction on gamer account
		GetAccount((*it).first, &account);

		if (!(account)->GetTransaction((*it).second, &currentTransactionId))
			return ERROR_MY_CASINO_UNKNOWN_TRANSACTION_ID;

		MyCasinoTransactionsInformationTypes infoType = MyCasinoTransactionsInformationTypes::Bet;
		// if user has won something create transaction on account
		if (totalReward > 0.01)
		{
			ULONG transactionId;
			resVal = (account)->CreateTransaction(totalReward, MyCasinoTransactionsTypes::BET_WIN, (*it).second, &infoType, &transactionId);
			if (FAILED(resVal))
				return resVal;

			resVal = (m_pOperatorAccount)->CreateTransaction(totalReward, MyCasinoTransactionsTypes::WITHDRAWAL, (*it).second, &infoType, &transactionId);
			if (FAILED(resVal))
				return resVal;
		}
		else 
		{
			resVal = (account)->ChangeTransaction(currentTransactionId, MyCasinoTransactionsTypes::BET_LOSS, (*it).second, &infoType);
			if (FAILED(resVal))
				return resVal;
		}

		// save result
		(*it).second->SetBetResult(*firstNumber, *secondNumber, totalReward);

		// save bets in order to delete them later
		m_formerBets.push_back((*it).second);

		// delete bet entry
		it = m_currentBets.erase(it);
	}

	return TRUE;
}


SHORT MyCasino::GenerateDrawNumber()
{
	return rand() % 5 + 1;
}

std::vector<MyCasinoBet*> MyCasino::GetBets()
{
	std::vector<MyCasinoBet*> betsSnapshot;

	//TODO: locks
	for (auto it = m_currentBets.begin(); it != m_currentBets.end(); it++)
	{
		betsSnapshot.push_back((it->second));
	}

	return betsSnapshot;
}

BOOL MyCasino::GetNextTransaction(MyCasinoUser& user, MyCasinoTransaction** const transaction)
{
	MyCasinoAccount* account = NULL;

	if (!GetAccount(user, &account))
		return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

	return (account)->GetNextTransaction(transaction);
}

BOOL MyCasino::GetTransactionInfomation(MyCasinoUser& user, ULONG transationId, IMyCasinoTransactionInformation** const transactionInformation, MyCasinoTransactionsInformationTypes* type)
{
	MyCasinoAccount* account = NULL;

	if (!GetAccount(user, &account))
		return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

	return (account)->GetTransactionInformation(transationId, transactionInformation, type);
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
