using MyCasinoLib;
using MyCasinoWCFLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyCasinoWCFServer
{
    class MyCasinoWCFServerImplementation : INETMyCasino
    {
        public static AuthService m_authService = new AuthService();
        public static List<User> userListLoggedOn = new List<User>();
        public static List<Draw> drawList = new List<Draw>();
        //List<Transaction> transactionList = new List<Transaction>();
        Dictionary<Transaction, Draw> dictTransDraw = new Dictionary<Transaction, Draw>();
        public bool login(string username, string password, out int sessionId, out MyCasinoUserTypes userType, out string errMsg)
        {
            User currUser;
            //TODO: wettenbutton aktivieren falls es operator war
            //errMsg = m_authService.Login(username, password, out sessionId, out userType, transactionList, out currUser);
            errMsg = m_authService.Login(username, password, out sessionId, out userType, dictTransDraw, out currUser);

            //Login successful
            if (errMsg == "S_OK")
            {
                if(currUser != null) userListLoggedOn.Add(currUser);
                Console.WriteLine(username + ": angemeldet");
                return true;
            }
            //Login unsuccessful 
            Console.WriteLine("ERROR: " + errMsg);
            return false;
        }

        public bool logout(int sessionId, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }

            errMsg = m_authService.Logout(sessionId);
            foreach (User user in userListLoggedOn)
            {
                if (user.SessionId == sessionId) userListLoggedOn.Remove(user);
            }
            //Logout successful
            if (errMsg == "S_OK")
            {
                Console.WriteLine("Ausloggen erfolgreich!");
                return true;
            }
            //Logout unsuccessful
            Console.WriteLine("ERROR: " + errMsg);
            return false;
        }


        public bool deposit(int sessionId, string name, double amountMoney, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }
            //select user
            User user = userListLoggedOn.Find(item => item.username == name);
            if (user == null)
            {
                errMsg = "User nicht eingeloggt";
                Console.WriteLine(errMsg);
                return false;
            }
            user.account.Deposit(amountMoney, dictTransDraw, user.Username, MyCasinoTransactionTypes.DEPOSIT);
            errMsg = null;
            Console.WriteLine(user.username + "'s Kontostand um " + amountMoney + " erhöht");
            return true;
        }

        public bool bet(int sessionId, double amountMoney, int firstNumber, int secondNumber, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }
            //TODO:Operator has to be online && has to have enough money

            User userOperatorCheck = userListLoggedOn.Find(item => item.UserType == MyCasinoUserTypes.Operator);
            if (userOperatorCheck != null)
            {
                //init
                double profitForOneMatch = 0, profitForTwoMatches=0;
                double profitForOneMatchTmp = 0, profitForTwoMatchesTmp=0;
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
                if (moneyAmountLeftChange > 0)
                {
                    
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
                    //set bet
                    User user = userListLoggedOn.Find(item => item.SessionId == sessionId);
                    user.account.Bet(user.Username, amountMoney, firstNumber, secondNumber);

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

        public bool calculateProfit(int sessionId, double amountMoney, int firstNumber, int secondNumber, out double profitForOneMatch, out double profitForTwoMatches, out string errMsg)
        {
            //Initializiation
            profitForTwoMatches = 0;
            profitForOneMatch = 0;
            errMsg = null;

            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }
            //TODO: new para name to get all showbetprofits correct
            foreach (User user in userListLoggedOn)
            {
                List<Bet> bets = new List<Bet>();
                user.account.getBetList(out bets);
                for (int i = 0; i < bets.Count; i++)
                {
                    if (bets.ElementAt(i).M_firstNumber==firstNumber && bets.ElementAt(i).M_secondNumber==secondNumber)
                    {
                        user.account.CalculateProfit(amountMoney, out profitForOneMatch, out profitForTwoMatches);
                    }
                 }
            }
            return true;
        }

        public bool showbets(int sessionId, out List<string> names, out List<int> firstNumber, out List<int> secondNumber, out List<double> amount, out int count , out string errMsg)
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
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }
            
            //fill userlist for showbets
            names = new List<string>();
            firstNumber = new List<int>();
            secondNumber = new List<int>();
            amount = new List<double>();
            foreach (User user in userListLoggedOn)
            {
                user.account.Showbets(out betsUser);
                for (int i = 0; i < betsUser.Count; i++)
                {
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

        public bool drawtest(int sessionId, int firstNumberTest, int secondNumberTest, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }

            errMsg = null;
            return true;
        }

        public bool draw(int sessionId, out int firstNumber, out int secondNumber, out string errMsg)
        {
            //init
            secondNumber = 0;
            firstNumber = 0;
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }
            //TODO:lock
            User user = userListLoggedOn.Find(item => item.SessionId == sessionId);
            user.account.Draw(out firstNumber, out secondNumber);


            //save all bets with drawing numbers
            List<Bet> bets;
            foreach (User userdrawingbets in userListLoggedOn)
            {
                if (MyCasinoUserTypes.Gamer == userdrawingbets.UserType)
                {
                    //init
                    List<double> profit = null;
                    //calculate money won
                    userdrawingbets.account.Profit(firstNumber, secondNumber, out profit);

                    //save all bets with drawing numbers and amount won or lost
                    userdrawingbets.account.SaveBetsDrawings(out bets);
                    Draw item = new Draw(bets, firstNumber, secondNumber, profit);
                    drawList.Add(item);
                }
            }

            //delete all current bets
            foreach (User userdelbets in userListLoggedOn)
            {
                if (MyCasinoUserTypes.Gamer == userdelbets.UserType)
                {
                    userdelbets.account.DelBets();
                }
            }

            errMsg = null;
            return true;
        }

        public bool getTransactions(int sessionId, out bool isFinished, out List<string> transaction, out int transactionType, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                transaction = null;
                transactionType = 0;
                isFinished = true;
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }

            transaction = null;
            transactionType = 0;
            isFinished = true;
            errMsg = null;
            return true;
        }
        public bool getTransactionInformation(int sessionId, int transactionId, out List<string> information, out int informationType, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                information = null;
                informationType = 0;
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }

            information = null;
            informationType = 0;
            errMsg = null;
            return true;
        }

    }
}
