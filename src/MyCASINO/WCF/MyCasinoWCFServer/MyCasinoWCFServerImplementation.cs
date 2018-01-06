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
        public bool login(string username, string password, out int sessionId, out MyCasinoUserTypes userType, out string errMsg)
        {
            User currUser;
            //TODO: wettenbutton aktivieren falls es operator war
            errMsg = m_authService.Login(username, password, out sessionId, out userType, out currUser);

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
            user.account.Deposit(amountMoney);
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

            User user = userListLoggedOn.Find(item => item.SessionId == sessionId);
            user.account.Bet(user.Username, amountMoney, firstNumber, secondNumber);
            

            errMsg = null;
            return true;
        }

        public bool calculateProfit(int sessionId, double amountMoney, out double profitForOneMatch, out double profitForTwoMatches, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                profitForTwoMatches = 0;
                profitForOneMatch = 0;
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }

            profitForTwoMatches = 0;
            profitForOneMatch = 0;
            errMsg = null;
            return true;
        }

        //public bool showbets(int sessionId, out List<Bet> bets, out int count, out string errMsg)
        //{
        //    //Check for valid sessionId
        //    if (!m_authService.SessionIdCheck(sessionId))
        //    {
        //        bets = null;
        //        count = 0;
        //        errMsg = "Ungueltige SessionId";
        //        Console.WriteLine(errMsg);
        //        return false;
        //    }
        //    //TODO:
        //    //Initialization of out params
        //    bets = null;
        //    count = 0;
        //    List<Bet> betsUser = new List<Bet>();
        //    //fill userlist for showbets
        //    bets = new List<Bet>();
        //    foreach (User user in userListLoggedOn)
        //    {
        //        user.account.Showbets(out betsUser);
        //        for (int i = 0; i < betsUser.Count; i++)
        //        {
        //            bets.Add(betsUser.ElementAt(i));
        //            count++;
        //        }
        //    }

        //    errMsg = null;
        //    return true;
        //}

        public bool showbets(int sessionId, out List<string> names, out List<int> firstNumber, out List<int> secondNumber, out List<double> amount, out int count , out string errMsg)
        {
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
            //Initialization of out params
            
            List<Bet> betsUser = new List<Bet>();
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
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                secondNumber = 0;
                firstNumber = 0;
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }

            secondNumber = 0;
            firstNumber = 0;
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
