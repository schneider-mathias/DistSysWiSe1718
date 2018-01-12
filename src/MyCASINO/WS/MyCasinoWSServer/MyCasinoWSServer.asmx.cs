using MyCasinoLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Services;

namespace MyCasinoWSServer
{
    /// <summary>
    /// Zusammenfassungsbeschreibung für MyCasinoWSServer
    /// </summary>
    //[WebService(Namespace = "http://tempuri.org/")]
    [WebService(Namespace = "http://MyCasinoServer.org/")]
    [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
    [System.ComponentModel.ToolboxItem(false)]
    // Wenn der Aufruf dieses Webdiensts aus einem Skript zulässig sein soll, heben Sie mithilfe von ASP.NET AJAX die Kommentarmarkierung für die folgende Zeile auf. 
    // [System.Web.Script.Services.ScriptService]
    public class MyCasinoWSServer : System.Web.Services.WebService
    {

        /// <summary>
        /// lock object
        /// </summary>
        private Object thisLockDictTransDraw = new Object();
        private Object thisLockUserListLoggedOn = new Object();
        /// <summary>
        /// list for all registered users
        /// </summary>
        public static AuthService m_authService = new AuthService();
        /// <summary>
        /// list for all active users
        /// </summary>
        public static List<User> userListLoggedOn = new List<User>();
        /// <summary>
        /// dictonary for all bets
        /// </summary>
        Dictionary<Transaction, Draw> dictTransDraw = new Dictionary<Transaction, Draw>();

        [WebMethod]
        public bool login(string username, string password, out int sessionId, out int userType, out string errMsg)
        {
            User currUser;
            lock (thisLockDictTransDraw)
            {
                errMsg = m_authService.Login(username, password, out sessionId, out userType, dictTransDraw, out currUser);
            }
            //Login successful
            if (errMsg == "S_OK")
            {
                lock (thisLockUserListLoggedOn)
                {
                    if (currUser != null) userListLoggedOn.Add(currUser);
                }
                Console.WriteLine(username + ": logged in");
                return true;
            }
            //Login unsuccessful 
            Console.WriteLine("ERROR: " + errMsg);
            return false;
        }

        [WebMethod]
        public bool logout(int sessionId, out string errMsg)
        {
            //init
            string name = "";
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "INVALID_SESSION_ID";
                Console.WriteLine(errMsg);
                return false;
            }
            //delete user in loggedlist and empty all bets of specific user
            lock (thisLockUserListLoggedOn)
            {
                User userOperatorCheck = userListLoggedOn.Find(item => item.UserType == MyCasinoUserTypes.Operator);
                if (userOperatorCheck != null)
                {
                    if (sessionId == userOperatorCheck.SessionId)
                    {
                        lock (thisLockDictTransDraw)
                        {
                            for (int i = 0; i < dictTransDraw.Count; i++)
                            {
                                if (dictTransDraw.ElementAt(i).Key.TransType == MyCasinoTransactionTypes.BET_WAGER)
                                {
                                    dictTransDraw.ElementAt(i).Key.TransType = MyCasinoTransactionTypes.CANCELED;
                                    //set money of unfinished bets back and delete bets
                                    foreach (User user in userListLoggedOn)
                                    {
                                        user.account.DelBets();
                                        user.account.MoneyAmountLeft += dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                                    }
                                    userOperatorCheck.account.MoneyAmountLeft -= dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                                }
                            }
                        }
                        //user from logged list
                        name = userOperatorCheck.Username;
                        userListLoggedOn.Remove(userOperatorCheck);
                    }
                }

                else
                {
                    foreach (User user in userListLoggedOn)
                    {
                        if (user.SessionId == sessionId)
                        {
                            lock (thisLockDictTransDraw)
                            {
                                for (int i = 0; i < dictTransDraw.Count; i++)
                                {
                                    //set money of unfinished bets back
                                    if (dictTransDraw.ElementAt(i).Key.Name == user.username &&
                                        dictTransDraw.ElementAt(i).Key.TransType == MyCasinoTransactionTypes.BET_WAGER)
                                    {
                                        dictTransDraw.ElementAt(i).Key.TransType = MyCasinoTransactionTypes.CANCELED;
                                        user.account.MoneyAmountLeft += dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                                        userOperatorCheck.account.MoneyAmountLeft -= dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                                    }
                                }
                            }
                            //delete bets and user from logged list
                            user.account.DelBets();
                            name = user.Username;
                            userListLoggedOn.Remove(user);
                            break;
                        }
                    }
                }
            }
            //set sessionid in userlist to 0
            errMsg = m_authService.Logout(sessionId);
            //Logout successful
            if (errMsg == "S_OK")
            {
                Console.WriteLine(name + ": logged out!");
                return true;
            }
            //Logout unsuccessful
            Console.WriteLine("ERROR: " + errMsg);
            return false;
        }

        [WebMethod]
        public bool deposit(int sessionId, string name, double amountMoney, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "INVALID_SESSION_ID";
                Console.WriteLine(errMsg);
                return false;
            }
            if (amountMoney > 10000)
            {
                errMsg = "PAYIN_TOO_HIGH";
                Console.WriteLine(errMsg);
                return false;
            }
            //select user
            lock (thisLockUserListLoggedOn)
            {
                User user = userListLoggedOn.Find(item => item.username == name);
                if (user == null)
                {
                    errMsg = "USER_NOT_LOGGED_IN";
                    Console.WriteLine(errMsg);
                    return false;
                }
                //check if maximum deposit amount is reached
                if (user.account.MoneyAmountLeft + amountMoney > int.MaxValue)
                {
                    errMsg = "MAXIMUM_BALANCE_REACHED";
                    Console.WriteLine(errMsg);
                    return false;
                }
                lock (thisLockDictTransDraw)
                {
                    user.account.Deposit(amountMoney, dictTransDraw, user.Username, MyCasinoTransactionTypes.DEPOSIT);
                }
                errMsg = null;
                Console.WriteLine(user.username + "'s deposit increased by " + amountMoney);
                return true;
            }
        }

        [WebMethod]
        public bool bet(int sessionId, double amountMoney, int firstNumber, int secondNumber, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "INVALID_SESSION_ID";
                Console.WriteLine(errMsg);
                return false;
            }
            if (amountMoney > 5000)
            {
                errMsg = "BET_AMOUNT_TOO_HIGH";
                Console.WriteLine(errMsg);
                return false;
            }

            lock (thisLockUserListLoggedOn)
            {
                User userOperatorCheck = userListLoggedOn.Find(item => item.UserType == MyCasinoUserTypes.Operator);
                if (userOperatorCheck != null)
                {
                    //init
                    MyCasinoTransactionTypes transType;
                    double profitForOneMatch = 0, profitForTwoMatches = 0;
                    double profitForOneMatchTmp = 0, profitForTwoMatchesTmp = 0;
                    double moneyAmountLeftChange = userOperatorCheck.account.MoneyAmountLeft;
                    List<Bet> betsProfit = new List<Bet>();
                    //calculate how much money the operator will have after bet
                    foreach (User userCalcProfit in userListLoggedOn)
                    {
                        double amount = 0, max = 0;
                        userCalcProfit.account.getBetList(out betsProfit);
                        for (int i = 0; i < betsProfit.Count; i++)
                        {
                            amount = betsProfit.ElementAt(i).M_setAmount;
                            if (max < amount) max = amount;
                        }
                        userCalcProfit.account.CalculateProfit(max, out profitForOneMatchTmp, out profitForTwoMatchesTmp);
                        userCalcProfit.account.CalculateProfit(amountMoney, out profitForOneMatch, out profitForTwoMatches);
                        if (profitForTwoMatches < profitForTwoMatchesTmp) moneyAmountLeftChange -= profitForTwoMatchesTmp;
                        else { moneyAmountLeftChange -= profitForTwoMatches; }
                    }
                    //check if operator has enough money to support this bet
                    if (moneyAmountLeftChange + amountMoney > 0)
                    {
                        //check if money of user is enough
                        User useraccountmoney = userListLoggedOn.Find(item => item.SessionId == sessionId);
                        if (useraccountmoney.account.MoneyAmountLeft < amountMoney)
                        {
                            errMsg = "NOT_ENOUGH_MONEY";
                            Console.WriteLine(errMsg);
                            return false;
                        }

                        //check if numbers are already set
                        foreach (User userLogged in userListLoggedOn)
                        {
                            List<Bet> bets = new List<Bet>();
                            userLogged.account.getBetList(out bets);
                            for (int i = 0; i < bets.Count; i++)
                            {
                                if (!(sessionId == userLogged.SessionId) && bets.ElementAt(i).M_firstNumber == firstNumber && bets.ElementAt(i).M_secondNumber == secondNumber)
                                {
                                    errMsg = "BET_ALREADY_SET";
                                    Console.WriteLine(errMsg);
                                    return false;
                                }
                            }
                        }
                        //add the additional bet funds to casino
                        userOperatorCheck.account.MoneyAmountLeft += amountMoney;
                        useraccountmoney.account.MoneyAmountLeft -= amountMoney;
                        //setup for bet
                        bool overridden;
                        bool delOverriddenBet;
                        bool betdeleted;
                        double saveMoney = 0;
                        User user = userListLoggedOn.Find(item => item.SessionId == sessionId);
                        List<Bet> betsMoneyLeft = new List<Bet>();

                        //save money from last bet, if it is a bet with same numbers
                        user.account.getBetList(out betsMoneyLeft);
                        for (int i = 0; i < betsMoneyLeft.Count; i++)
                        {
                            if (betsMoneyLeft.ElementAt(i).M_firstNumber == firstNumber && betsMoneyLeft.ElementAt(i).M_secondNumber == secondNumber)
                            {
                                saveMoney = betsMoneyLeft.ElementAt(i).M_setAmount;
                            }
                        }

                        user.account.Bet(user.Username, amountMoney, firstNumber, secondNumber, out transType, out overridden, out delOverriddenBet, out betdeleted);
                        //set money for operator back
                        if (transType == MyCasinoTransactionTypes.CANCELED)
                        {
                            userOperatorCheck.account.MoneyAmountLeft -= saveMoney;
                        }
                        //check if bet was overriden
                        if (betdeleted == false)
                        {
                            lock (thisLockDictTransDraw)
                            {
                                dictTransDraw.Add(new Transaction(dictTransDraw.Last().Key.M_id + 1,
                                                                 dictTransDraw.Last(item => item.Key.Name == user.Username).Key.CurrentAmount,
                                                                 dictTransDraw.Last(item => item.Key.Name == user.Username).Key.ChangeAmount,
                                                                 user.Username, transType)
                                                                 , new Draw(new Bet(user.Username, firstNumber, secondNumber, amountMoney), 0, 0, 0));
                            }
                        }
                        //set money for new bet if bet was overridden
                        if (overridden == true)
                        {
                            lock (thisLockDictTransDraw)
                            {
                                for (int i = 0; i < dictTransDraw.Count; i++)
                                {
                                    if (dictTransDraw.ElementAt(i).Value != null)
                                    {
                                        if (dictTransDraw.ElementAt(i).Value.DrawBet.M_firstNumber == firstNumber &&
                                            dictTransDraw.ElementAt(i).Value.DrawBet.M_secondNumber == secondNumber &&
                                            dictTransDraw.ElementAt(i).Key.TransType == MyCasinoTransactionTypes.BET_WAGER)
                                        {
                                            dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount = amountMoney;
                                        }
                                    }
                                }
                            }
                        }
                        //set bet that was overridden to canceled
                        if (delOverriddenBet == true)
                        {
                            lock (thisLockDictTransDraw)
                            {
                                for (int i = 0; i < dictTransDraw.Count; i++)
                                {
                                    if (dictTransDraw.ElementAt(i).Value != null)
                                    {
                                        if (dictTransDraw.ElementAt(i).Key.TransType == MyCasinoTransactionTypes.BET_WAGER &&
                                            dictTransDraw.ElementAt(i).Value.DrawBet.M_firstNumber == firstNumber &&
                                            dictTransDraw.ElementAt(i).Value.DrawBet.M_secondNumber == secondNumber)
                                        {
                                            userOperatorCheck.account.MoneyAmountLeft -= saveMoney;
                                            dictTransDraw.ElementAt(i).Key.TransType = MyCasinoTransactionTypes.CANCELED;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        errMsg = null;
                        return true;
                    }
                    errMsg = "NOT_ENOUGH_MONEY_CASINO";
                    Console.WriteLine(errMsg);
                    return false;
                }
                errMsg = "NO_OPERATOR_LOGGED_IN";
                Console.WriteLine(errMsg);
                return false;
            }
        }

        [WebMethod]
        public bool calculateProfit(int sessionId, double amountMoney, int firstNumber, int secondNumber, out double profitForOneMatch, out double profitForTwoMatches, out string errMsg)
        {
            //Initializiation
            profitForTwoMatches = 0;
            profitForOneMatch = 0;
            errMsg = null;

            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "INVALID_SESSION_ID";
                Console.WriteLine(errMsg);
                return false;
            }
            lock (thisLockUserListLoggedOn)
            {
                foreach (User user in userListLoggedOn)
                {
                    List<Bet> bets = new List<Bet>();
                    user.account.getBetList(out bets);
                    for (int i = 0; i < bets.Count; i++)
                    {
                        if (bets.ElementAt(i).M_firstNumber == firstNumber && bets.ElementAt(i).M_secondNumber == secondNumber)
                        {
                            user.account.CalculateProfit(amountMoney, out profitForOneMatch, out profitForTwoMatches);
                        }
                    }
                }
                return true;
            }
        }

        [WebMethod]
        public bool showbets(int sessionId, out List<string> names, out List<int> firstNumber, out List<int> secondNumber, out List<double> amount, out int count, out string errMsg)
        {
            //Initialization of out params
            List<Bet> betsUser = new List<Bet>();
            names = null;
            firstNumber = null;
            secondNumber = null;
            amount = null;
            count = 0;

            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "INVALID_SESSION_ID";
                Console.WriteLine(errMsg);
                return false;
            }

            //fill userlist for showbets
            //init
            names = new List<string>();
            firstNumber = new List<int>();
            secondNumber = new List<int>();
            amount = new List<double>();
            lock (thisLockUserListLoggedOn)
            {
                foreach (User user in userListLoggedOn)
                {
                    user.account.Showbets(out betsUser);
                    for (int i = 0; i < betsUser.Count; i++)
                    {
                        //set values for the data that has to be sent to the client
                        names.Add(betsUser.ElementAt(i).M_name.ToString());
                        firstNumber.Add(betsUser.ElementAt(i).M_firstNumber);
                        secondNumber.Add(betsUser.ElementAt(i).M_secondNumber);
                        amount.Add(betsUser.ElementAt(i).M_setAmount);
                        count++;
                    }
                }

                errMsg = null;
                return true;
            }
        }

        [WebMethod]
        public bool drawtest(int sessionId, int firstNumberTest, int secondNumberTest, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "INVALID_SESSION_ID";
                Console.WriteLine(errMsg);
                return false;
            }
            if (firstNumberTest == 0 || secondNumberTest == 0)
            {
                errMsg = "ERROR_MY_CASINO_BET_INVALID_NUMBER";
                Console.WriteLine(errMsg);
                return false;
            }
            //init
            List<double> profit = null;
            int count = 0;
            //save all bets with drawing numbers
            lock (thisLockUserListLoggedOn)
            {
                foreach (User userdrawingbets in userListLoggedOn)
                {
                    count = 0;
                    if (MyCasinoUserTypes.Gamer == userdrawingbets.UserType)
                    {
                        //calculate money won
                        userdrawingbets.account.Profit(firstNumberTest, secondNumberTest, out profit);
                        //save all bets with drawing numbers and amount won or lost
                        lock (thisLockDictTransDraw)
                        {
                            for (int i = 0; i < dictTransDraw.Count; i++)
                            {
                                if (!(dictTransDraw.ElementAt(i).Value == null) &&
                                    profit.Count > 0 &&
                                    dictTransDraw.ElementAt(i).Key.Name == userdrawingbets.username)
                                {
                                    if (dictTransDraw.ElementAt(i).Key.TransType == MyCasinoTransactionTypes.BET_WAGER)
                                    {
                                        dictTransDraw.ElementAt(i).Value.M_drawnFirstNumber = firstNumberTest;
                                        dictTransDraw.ElementAt(i).Value.M_drawnSecondNumber = secondNumberTest;
                                        if (profit.ElementAt(count) > 0)
                                        {
                                            dictTransDraw.ElementAt(i).Key.TransType = MyCasinoTransactionTypes.BET_WIN;
                                            dictTransDraw.ElementAt(i).Value.MoneyWon = profit.ElementAt(count);
                                            dictTransDraw.ElementAt(i).Key.ChangeAmount = profit.ElementAt(count);
                                            //find the right element in the dictionary, (i-1) is always > than 0
                                            int index = 1;
                                            for (int k = 1; k < dictTransDraw.Count + 1; k++)
                                            {
                                                if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.DEPOSIT &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                else if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.BET_WIN &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                else if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.BET_LOSS &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                index++;

                                            }
                                            dictTransDraw.ElementAt(i).Key.CurrentAmount = dictTransDraw.ElementAt(i - index).Key.CurrentAmount + profit.ElementAt(count);
                                            dictTransDraw.ElementAt(i).Key.CurrentAmount -= dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                                            //set casino money
                                            User userOperatorCheck = userListLoggedOn.Find(item => item.UserType == MyCasinoUserTypes.Operator);
                                            userOperatorCheck.account.MoneyAmountLeft -= profit.ElementAt(count);
                                            //set user money
                                            User userMoney = userListLoggedOn.Find(item => item.Username == dictTransDraw.ElementAt(i).Key.Name);
                                            userMoney.account.MoneyAmountLeft += profit.ElementAt(count);
                                        }
                                        //check if bet was lost and set money values
                                        if (profit.ElementAt(count) == 0)
                                        {
                                            dictTransDraw.ElementAt(i).Key.TransType = MyCasinoTransactionTypes.BET_LOSS;
                                            dictTransDraw.ElementAt(i).Value.MoneyWon = profit.ElementAt(count);
                                            dictTransDraw.ElementAt(i).Key.ChangeAmount = profit.ElementAt(count);
                                            //find the right element in the dictionary, (i-1) is always > than 0
                                            int index = 1;
                                            for (int k = 1; k < dictTransDraw.Count + 1; k++)
                                            {
                                                if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.DEPOSIT &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                else if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.BET_WIN &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                else if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.BET_LOSS &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                index++;
                                            }
                                            //set user money to current situation
                                            dictTransDraw.ElementAt(i).Key.CurrentAmount = dictTransDraw.ElementAt(i - index).Key.CurrentAmount;
                                            dictTransDraw.ElementAt(i).Key.CurrentAmount -= dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                                        }

                                        count++;
                                    }
                                }

                            }
                        }
                    }
                }
            }
            //delete all current bets
            lock (thisLockUserListLoggedOn)
            {
                foreach (User userdelbets in userListLoggedOn)
                {
                    if (MyCasinoUserTypes.Gamer == userdelbets.UserType)
                    {
                        userdelbets.account.DelBets();
                    }
                }
            }
            errMsg = null;
            return true;
        }

        [WebMethod]
        public bool draw(int sessionId, out int firstNumber, out int secondNumber, out string errMsg)
        {
            //init
            secondNumber = 0;
            firstNumber = 0;
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "INVALID_SESSION_ID";
                Console.WriteLine(errMsg);
                return false;
            }
            lock (thisLockUserListLoggedOn)
            {
                User user = userListLoggedOn.Find(item => item.SessionId == sessionId);
                user.account.Draw(out firstNumber, out secondNumber);

                //init
                List<double> profit = null;
                int count = 0;
                //save all bets with drawing numbers
                foreach (User userdrawingbets in userListLoggedOn)
                {
                    count = 0;
                    if (MyCasinoUserTypes.Gamer == userdrawingbets.UserType)
                    {
                        //calculate money won
                        userdrawingbets.account.Profit(firstNumber, secondNumber, out profit);
                        //save all bets with drawing numbers and amount won or lost
                        lock (thisLockDictTransDraw)
                        {
                            for (int i = 0; i < dictTransDraw.Count; i++)
                            {
                                if (!(dictTransDraw.ElementAt(i).Value == null) &&
                                    profit.Count > 0 &&
                                    dictTransDraw.ElementAt(i).Key.Name == userdrawingbets.username)
                                {
                                    if (dictTransDraw.ElementAt(i).Key.TransType == MyCasinoTransactionTypes.BET_WAGER)
                                    {
                                        dictTransDraw.ElementAt(i).Value.M_drawnFirstNumber = firstNumber;
                                        dictTransDraw.ElementAt(i).Value.M_drawnSecondNumber = secondNumber;
                                        if (profit.ElementAt(count) > 0)
                                        {
                                            dictTransDraw.ElementAt(i).Key.TransType = MyCasinoTransactionTypes.BET_WIN;
                                            dictTransDraw.ElementAt(i).Value.MoneyWon = profit.ElementAt(count);
                                            dictTransDraw.ElementAt(i).Key.ChangeAmount = profit.ElementAt(count);
                                            //find the right element in the dictionary, (i-1) is always > than 0
                                            int index = 1;
                                            for (int k = 1; k < dictTransDraw.Count + 1; k++)
                                            {
                                                if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.DEPOSIT &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                else if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.BET_WIN &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                else if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.BET_LOSS &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                index++;

                                            }
                                            dictTransDraw.ElementAt(i).Key.CurrentAmount = dictTransDraw.ElementAt(i - index).Key.CurrentAmount + profit.ElementAt(count);
                                            dictTransDraw.ElementAt(i).Key.CurrentAmount -= dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                                            //set casino money
                                            User userOperatorCheck = userListLoggedOn.Find(item => item.UserType == MyCasinoUserTypes.Operator);
                                            userOperatorCheck.account.MoneyAmountLeft -= profit.ElementAt(count);
                                            //set user money
                                            User userMoney = userListLoggedOn.Find(item => item.Username == dictTransDraw.ElementAt(i).Key.Name);
                                            userMoney.account.MoneyAmountLeft += profit.ElementAt(count);
                                        }
                                        //check if bet was lost and set money values
                                        if (profit.ElementAt(count) == 0)
                                        {
                                            dictTransDraw.ElementAt(i).Key.TransType = MyCasinoTransactionTypes.BET_LOSS;
                                            dictTransDraw.ElementAt(i).Value.MoneyWon = profit.ElementAt(count);
                                            dictTransDraw.ElementAt(i).Key.ChangeAmount = profit.ElementAt(count);
                                            //find the right element in the dictionary, (i-1) is always > than 0
                                            int index = 1;
                                            for (int k = 1; k < dictTransDraw.Count + 1; k++)
                                            {
                                                if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.DEPOSIT &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                else if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.BET_WIN &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                else if (dictTransDraw.ElementAt(i - k).Key.TransType == MyCasinoTransactionTypes.BET_LOSS &&
                                                    dictTransDraw.ElementAt(i - k).Key.Name == userdrawingbets.username) break;
                                                index++;
                                            }
                                            //set user money to current situation
                                            dictTransDraw.ElementAt(i).Key.CurrentAmount = dictTransDraw.ElementAt(i - index).Key.CurrentAmount;
                                            dictTransDraw.ElementAt(i).Key.CurrentAmount -= dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                                        }
                                        count++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //delete all current bets
            lock (thisLockUserListLoggedOn)
            {
                foreach (User userdelbets in userListLoggedOn)
                {
                    if (MyCasinoUserTypes.Gamer == userdelbets.UserType)
                    {
                        userdelbets.account.DelBets();
                    }
                }
            }
            errMsg = null;
            return true;
        }

        [WebMethod]
        public bool getTransactions(int sessionId, out bool isFinished, out List<string> transaction, out int transactionType, out string errMsg)
        {
            //init
            transaction = new List<string>(); ;
            transactionType = 0;
            isFinished = false;
            errMsg = null;
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "INVALID_SESSION_ID";
                Console.WriteLine(errMsg);
                return false;
            }
            lock (thisLockUserListLoggedOn)
            {
                User user = userListLoggedOn.Find(item => item.SessionId == sessionId);
                //prepare string to be transmitted to the client, if the client is a gamer
                if (user.UserType == MyCasinoUserTypes.Gamer)
                {
                    lock (thisLockDictTransDraw)
                    {
                        for (int i = 0; i < dictTransDraw.Count; i++)
                        {
                            if (dictTransDraw.ElementAt(i).Key.Name == user.username &&
                                dictTransDraw.ElementAt(i).Key.IsFinished == false)
                            {
                                //save strings
                                transaction.Add(dictTransDraw.ElementAt(i).Key.M_id.ToString());
                                transaction.Add(dictTransDraw.ElementAt(i).Key.CurrentAmount.ToString());
                                transaction.Add(dictTransDraw.ElementAt(i).Key.ChangeAmount.ToString());
                                transactionType = (int)dictTransDraw.ElementAt(i).Key.TransType;
                                dictTransDraw.ElementAt(i).Key.IsFinished = true;
                                return true;
                            }
                        }
                    }
                }
                //prepare string to be transmitted to the client, if the client is a operator
                else if (user.UserType == MyCasinoUserTypes.Operator)
                {
                    lock (thisLockDictTransDraw)
                    {
                        for (int i = 0; i < dictTransDraw.Count; i++)
                        {
                            //save information to display only display operator deposit
                            if (dictTransDraw.ElementAt(i).Key.Name == user.username &&
                                dictTransDraw.ElementAt(i).Key.TransType == MyCasinoTransactionTypes.DEPOSIT &&
                                dictTransDraw.ElementAt(i).Key.IsFinished == false)
                            {
                                transaction.Add(dictTransDraw.ElementAt(i).Key.M_id.ToString());
                                transaction.Add(dictTransDraw.ElementAt(i).Key.CurrentAmount.ToString());
                                transaction.Add(dictTransDraw.ElementAt(i).Key.ChangeAmount.ToString());
                                transactionType = (int)dictTransDraw.ElementAt(i).Key.TransType;
                                dictTransDraw.ElementAt(i).Key.IsFinished = true;
                                return true;
                            }

                            //save information to display all userinformation for bets
                            if (!(dictTransDraw.ElementAt(i).Key.TransType == MyCasinoTransactionTypes.DEPOSIT) &&
                                dictTransDraw.ElementAt(i).Key.IsFinished == false)
                            {
                                transaction.Add(dictTransDraw.ElementAt(i).Key.M_id.ToString());
                                transaction.Add(dictTransDraw.ElementAt(i).Key.CurrentAmount.ToString());
                                transaction.Add(dictTransDraw.ElementAt(i).Key.ChangeAmount.ToString());
                                transactionType = (int)dictTransDraw.ElementAt(i).Key.TransType;
                                dictTransDraw.ElementAt(i).Key.IsFinished = true;
                                return true;
                            }
                        }
                    }
                }
                isFinished = true;
                //reset isFinished in all elements
                lock (thisLockDictTransDraw)
                {
                    for (int i = 0; i < dictTransDraw.Count; i++)
                    {
                        dictTransDraw.ElementAt(i).Key.IsFinished = false;
                    }
                }
            }
            return true;
        }
        [WebMethod]
        public bool getTransactionInformation(int sessionId, int transactionId, out List<string> information, out int informationType, out string errMsg)
        {
            information = new List<string>();
            informationType = 0;
            errMsg = null;
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "INVALID_SESSION_ID";
                Console.WriteLine(errMsg);
                return false;
            }

            //save all information, that has to be transmitted to the client if the request came from a gamer
            lock (thisLockUserListLoggedOn)
            {
                User user = userListLoggedOn.Find(item => item.SessionId == sessionId);
                if (user.UserType == MyCasinoUserTypes.Gamer)
                {
                    lock (thisLockDictTransDraw)
                    {
                        for (int i = 0; i < dictTransDraw.Count; i++)
                        {
                            if (dictTransDraw.ElementAt(i).Key.M_id == transactionId)
                            {
                                information.Add(dictTransDraw.ElementAt(i).Key.Name.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.DrawBet.M_firstNumber.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.DrawBet.M_secondNumber.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.M_drawnFirstNumber.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.M_drawnSecondNumber.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.MoneyWon.ToString());
                            }
                        }
                    }

                    return true;
                }
                //save all information, that has to be transmitted to the client if the request came form the operator
                if (user.UserType == MyCasinoUserTypes.Operator)
                {
                    lock (thisLockDictTransDraw)
                    {
                        for (int i = 0; i < dictTransDraw.Count; i++)
                        {
                            if (dictTransDraw.ElementAt(i).Key.M_id == transactionId)
                            {
                                information.Add(dictTransDraw.ElementAt(i).Key.Name.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.DrawBet.M_firstNumber.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.DrawBet.M_secondNumber.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.M_drawnFirstNumber.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.M_drawnSecondNumber.ToString());
                                information.Add(dictTransDraw.ElementAt(i).Value.MoneyWon.ToString());
                            }
                        }
                    }
                }
            }
            return true;
        }

    }
}
