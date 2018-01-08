#include "MyCasino.h"
#include <utility>
#include <cstdlib>
#include <time.h>
#include <fstream>

MyCasino::MyCasino(std::wstring* userDataDirRootEnv) :
	m_currentBetId(0),
	m_pOperator(NULL),
	m_pOperatorAccount(NULL)
{
	// set root directory for user data using environment variable
	m_userDataDirRoot = L"";
	if (NULL != userDataDirRootEnv)
	{
		wchar_t *buf = nullptr;
		size_t sz = 0;
		if (_wdupenv_s(&buf, &sz, (*userDataDirRootEnv).c_str()) == 0 && buf != nullptr)
		{
			m_userDataDirRoot = std::wstring(buf);
			free(buf);
		}
	}

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

	// delete all account objects
	for (auto it = m_userAccounts.begin(); it != m_userAccounts.end(); it++)
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

BOOL MyCasino::LoadAccounts(std::wstring filename)
{
	std::wifstream m_accountDataFileStream;

	m_accountDataFilename = filename;
	std::wstring accountFile = m_userDataDirRoot;
	accountFile.append(m_accountDataFilename);

	m_accountDataFileStream.open(accountFile);
	if (m_accountDataFileStream.fail())
	{
		return FALSE;
	}

	std::wstring wLine;
	while (std::getline(m_accountDataFileStream, wLine))
	{
		MyCasinoAccount* loadedAccount = new MyCasinoAccount();
		BOOL resVal = loadedAccount->Deserialize(wLine);
		if(FAILED(resVal))
		{
			delete loadedAccount;
			return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNTS;
		}

		m_userAccounts.push_back(loadedAccount);
	}

	m_accountDataFileStream.close();

	return TRUE;
}


BOOL MyCasino::LoadAccount(const MyCasinoUser& user, MyCasinoAccount** account)
{
	for (auto it = m_userAccounts.begin(); it != m_userAccounts.end(); it++)
	{
		if (user.m_username.compare((*it)->GetUsername()) == 0)
		{
			(*account) = (*it);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL MyCasino::SaveAccounts()
{
	std::wofstream m_accountDataFileStream;

	std::wstring accountFile = m_userDataDirRoot;
	accountFile.append(m_accountDataFilename);

	m_accountDataFileStream.open(accountFile);
	if (m_accountDataFileStream.fail())
	{
		return FALSE;
	}

	for (auto it = m_userAccounts.begin(); it != m_userAccounts.end(); it++)
	{
		m_accountDataFileStream << (*it)->Serialize() << std::endl;
	}

	m_accountDataFileStream.close();

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
	if (!IsOpened())
		return ERROR_MY_CASINO_NO_OPERATOR;

	MyCasinoBet* bet = NULL;
	// if bet does not exists yet create a new bet
	if (!GetBet(firstNumber, secondNumber, &bet))
		bet = new MyCasinoBet(user.m_username,m_currentBetId++, firstNumber, secondNumber, setAmount);

	BOOL resVal = FALSE;
	MyCasinoAccount* account = NULL;
	DOUBLE setAmountDifference = 0.0;
	DOUBLE previousWager = 0.0;
	ULONG currentTransactionId = 0;

	MyCasinoTransactionsInformationTypes infoType = MyCasinoTransactionsInformationTypes::Bet;
	switch (CheckBet(user, *bet))
	{
		case TRUE:
			// new bet, everything fine continue
			
			// check if operator account balance is sufficient for this bet
			resVal = CheckOperatorAccount(*bet);
			if (FAILED(resVal))
				return resVal;

			// book transaction on gamer account
			if (!GetAccount(user, &account))
				return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

			ULONG transactionId;
			resVal = (account)->CreateTransaction(-(bet->GetSetAmount()), MyCasinoTransactionsTypes::BET_WAGER, bet, &infoType, &transactionId);
			if (FAILED(resVal))
				return resVal;

			// book wager income for operator
			resVal = (m_pOperatorAccount)->CreateTransaction(bet->GetSetAmount(), MyCasinoTransactionsTypes::BET_WAGER, bet, &infoType, &transactionId);
			if (FAILED(resVal))
				return resVal;


			// add to current bets
			m_currentBets.insert(std::pair<MyCasinoUser, MyCasinoBet*>(user, bet));
			
			return TRUE;

		case INFORMATION_MY_CASINO_USER_HAS_NUMBERS:
			// user has already bet on this numbers, apply new values on bet and account
			

			if (!GetAccount(user, &account))
				return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;


			// first check if new amount is 0, in this case close the bet
			if (setAmount < 0.001)
			{
				// remove entry from map
				if (!DeleteBet(bet->GetFirstNumber(), bet->GetSecondNumber()))
					return ERROR_MY_CASINO_BET_CANNOT_DELETE;

				// cancel transactions
				resVal = CloseBet(user, *bet);
				if (FAILED(resVal))
					return resVal;

				return TRUE;
			}

			previousWager = bet->GetSetAmount();
			bet->SetWager(setAmount);

			// check if operator account balance is sufficient for this bet,
			// therefore apply new wager temporarily
			resVal = CheckOperatorAccount(*bet);
			if (FAILED(resVal))
			{
				bet->SetWager(previousWager);
				return resVal;
			}

			// undo wager change until transaction was changed sucessfully
			bet->SetWager(previousWager);
				
			// check if gamer account balance is sufficient
			setAmountDifference = setAmount - previousWager;
			if ((account)->GetCurrentBalance() - setAmountDifference < 0)
				return ERROR_MY_CASINO_USER_ACCOUNT_BALANCE_NOT_SUFFICIENT;
			
			// adjust transaction object
			if (!(account)->GetTransaction(bet, &currentTransactionId))
				return ERROR_MY_CASINO_UNKNOWN_TRANSACTION_ID;

			resVal = (account)->ChangeTransaction(currentTransactionId, -setAmount, MyCasinoTransactionsTypes::BET_WAGER, bet, &infoType);
			if (FAILED(resVal))
				return resVal;

			// adjust operators transaction object
			if (!(m_pOperatorAccount)->GetTransaction(bet, &currentTransactionId))
				return ERROR_MY_CASINO_UNKNOWN_TRANSACTION_ID;

			resVal = (m_pOperatorAccount)->ChangeTransaction(currentTransactionId, setAmount, MyCasinoTransactionsTypes::BET_WAGER, bet, &infoType);
			if (FAILED(resVal))
				return resVal;

			// finally overwrite amount in bet object
			bet->SetWager(setAmount);

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
	if (!IsOpened())
		return ERROR_MY_CASINO_NO_OPERATOR;

	BOOL deleteSuccess = FALSE;
	for (std::multimap<MyCasinoUser, MyCasinoBet*>::iterator it = m_currentBets.begin(); it != m_currentBets.end();)
	{
		if (firstNumber == (*it).second->GetFirstNumber()
			&& secondNumber == (*it).second->GetSecondNumber()
			)
		{
			m_formerBets.push_back((*it).second);
			it = m_currentBets.erase(it);
			deleteSuccess = TRUE;
			break;
		}
	}

	return deleteSuccess;
}

BOOL MyCasino::CloseBet(const MyCasinoUser& user, MyCasinoBet& bet)
{
	MyCasinoAccount* account = NULL;

	if (!GetAccount(user, &account))
		return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

	ULONG currentTransactionId = -1;
	// cancel gamer transaction 
	if (!(account)->GetTransaction(&bet, &currentTransactionId))
		return ERROR_MY_CASINO_UNKNOWN_TRANSACTION_ID;

	BOOL resVal = (account)->CancelTransaction(currentTransactionId);
	if (FAILED(resVal))
		return resVal;

	// cancel operator transaction 
	if (!(m_pOperatorAccount)->GetTransaction(&bet, &currentTransactionId))
		return ERROR_MY_CASINO_UNKNOWN_TRANSACTION_ID;

	resVal = (m_pOperatorAccount)->CancelTransaction(currentTransactionId);
	if (FAILED(resVal))
		return resVal;

	return TRUE;
}

BOOL MyCasino::GetBet(SHORT firstNumber, SHORT secondNumber, MyCasinoBet** bet)
{
	if (!IsOpened())
		return ERROR_MY_CASINO_NO_OPERATOR;

	*bet = NULL;
	for (std::multimap<MyCasinoUser, MyCasinoBet*>::iterator it = m_currentBets.begin(); it != m_currentBets.end(); it++)
	{
		if (firstNumber == (*it).second->GetFirstNumber()
			&& secondNumber == (*it).second->GetSecondNumber()
			)
		{
			*bet = ((*it).second);
			break;
		}
	}

	return (NULL != *bet);
}

BOOL MyCasino::GetAccount(const MyCasinoUser& user, MyCasinoAccount** account)
{
	*account = NULL;

	// operator account is not stored in user list
	if (NULL != m_pOperator && user == *m_pOperator)
	{
		if (NULL == m_pOperatorAccount)
			return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

		*account = m_pOperatorAccount;
		return TRUE;
	}

	for (std::map<MyCasinoUser, MyCasinoAccount*>::iterator it = m_loadedUserAccounts.begin(); it != m_loadedUserAccounts.end(); it++)
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

		m_loadedUserAccounts.insert(std::pair<MyCasinoUser, MyCasinoAccount*>(user, *account));
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
	if (!IsOpened())
		return ERROR_MY_CASINO_NO_OPERATOR;

	MyCasinoAccount* account = NULL;

	if (!GetAccount(user, &account))
		return ERROR_MY_CASINO_CANNOT_LOAD_ACCOUNT;

	ULONG transactionId;
	BOOL retVal = (account)->CreateTransaction(amount, MyCasinoTransactionsTypes::DEPOSIT, NULL, NULL, &transactionId);
	if (FAILED(retVal))
		return retVal;

	if (!SaveAccounts())
		return ERROR_MY_CASINO_CANNOT_SAVE_ACCOUNTS;

	return TRUE;
}

BOOL MyCasino::Withdraw(MyCasinoUser& user, DOUBLE amount)
{
	if (!IsOpened())
		return ERROR_MY_CASINO_NO_OPERATOR;

	return E_NOTIMPL;
}

BOOL MyCasino::Draw(SHORT** firstNumber, SHORT** secondNumber)
{
	if (!IsOpened())
		return ERROR_MY_CASINO_NO_OPERATOR;

	if (NULL == *firstNumber)
		*firstNumber = new short(GenerateDrawNumber(NULL));
	if (NULL == *secondNumber)
		*secondNumber = new short(GenerateDrawNumber(*firstNumber));

	BOOL resVal = TRUE;
	DOUBLE rewardForOne = 0.0;
	DOUBLE rewardForTwo = 0.0;
	DOUBLE totalReward = 0.0;
	MyCasinoAccount* account;
	ULONG currentTransactionId = 0;
	ULONG currentOperatorTransactionId = 0;
	for (std::multimap<MyCasinoUser, MyCasinoBet*>::iterator it = m_currentBets.begin(); it != m_currentBets.end();)
	{
		totalReward = 0.0;
		rewardForOne = 0.0;
		rewardForTwo = 0.0;

		resVal = CalculateProfit(*(*it).second, &rewardForOne, &rewardForTwo);
		if (FAILED(resVal)) 
			return resVal; // should never happen

		// calculate total rewards
		// both numbers are drawn
		if (**firstNumber == (*it).second->GetFirstNumber() && **secondNumber == (*it).second->GetSecondNumber())
			totalReward = rewardForTwo;
		// one number is drawn
		else if (**firstNumber == (*it).second->GetFirstNumber() || **secondNumber == (*it).second->GetFirstNumber())		
			totalReward = rewardForOne;


		// book transaction on gamer account
		GetAccount((*it).first, &account);

		if (!(account)->GetTransaction((*it).second, &currentTransactionId))
			return ERROR_MY_CASINO_UNKNOWN_TRANSACTION_ID;

		if (!(m_pOperatorAccount)->GetTransaction((*it).second, &currentOperatorTransactionId))
			return ERROR_MY_CASINO_UNKNOWN_TRANSACTION_ID;

		MyCasinoTransactionsInformationTypes infoType = MyCasinoTransactionsInformationTypes::Bet;
		if (totalReward > 0.001)
		{
			resVal = (account)->ChangeTransaction(currentTransactionId,
				totalReward, 
				MyCasinoTransactionsTypes::BET_WIN, 
				(*it).second, &infoType);

			if (FAILED(resVal))
				return resVal;

			resVal = (m_pOperatorAccount)->ChangeTransaction(currentOperatorTransactionId,
				-totalReward, 
				MyCasinoTransactionsTypes::BET_LOSS, 
				(*it).second, &infoType);

			if (FAILED(resVal))
				return resVal;
		}
		else
		{
			resVal = (account)->ChangeTransaction(currentTransactionId,
				-(*it).second->GetSetAmount(),
				MyCasinoTransactionsTypes::BET_LOSS,
				(*it).second,
				&infoType);

			if (FAILED(resVal))
				return resVal;

			resVal = (m_pOperatorAccount)->ChangeTransaction(currentOperatorTransactionId,
				(*it).second->GetSetAmount(),
				MyCasinoTransactionsTypes::BET_WIN,
				(*it).second,
				&infoType);

			if (FAILED(resVal))
				return resVal;
		}

		// save result
		(*it).second->SetBetResult(**firstNumber, **secondNumber, totalReward);

		// save bets in order to delete them later
		m_formerBets.push_back((*it).second);

		// delete bet entry
		it = m_currentBets.erase(it);
	}

	// save user accounts
	if (!SaveAccounts())
		return ERROR_MY_CASINO_CANNOT_SAVE_ACCOUNTS;

	return TRUE;
}


SHORT MyCasino::GenerateDrawNumber(SHORT* firstNumber)
{
	// ToDo: check whether it is these number have to fullfill 
	// the same restrictions as bet sets (first number < second number)
	//return (NULL==firstNumber)? (rand() % 4 + 1) : (rand() % (5 - *firstNumber) + *firstNumber + 1);
	return (rand() % 5 + 1);
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

	if (!SaveAccounts())
		return ERROR_MY_CASINO_CANNOT_SAVE_ACCOUNTS;

	// close all current bets
	ULONG currentTransactionId = -1;
	BOOL resVal = TRUE;
	for (std::multimap<MyCasinoUser, MyCasinoBet*>::iterator it = m_currentBets.begin(); it != m_currentBets.end();)
	{
		// close transactions
		resVal = CloseBet((*it).first, *(*it).second);
		if (!resVal)
			return resVal;

		// save bets in order to delete them later
		m_formerBets.push_back((*it).second);

		// delete bet entry
		it = m_currentBets.erase(it);
	}

	// operator will be deleted by AuthService
	m_pOperator = NULL;

	// account will be deleted in destructor
	m_pOperatorAccount = NULL;

	return TRUE;
}
