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
        public bool login(string username, string password, out int sessionId, out MyCasinoUserTypes userType, out string errMsg)
        {
            //TODO: wettenbutton aktivieren falls es operator war
            errMsg = m_authService.Login(username, password, out sessionId, out userType);

            //Login successful
            if (errMsg == "S_OK")
            {
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

            //TODO: wettenbutton deaktivieren falls es operator war
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


            errMsg = null;
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

        public bool showbets(int sessionId, out List<string> bets, out int count, out string errMsg)
        {
            //Check for valid sessionId
            if (!m_authService.SessionIdCheck(sessionId))
            {
                bets = null;
                count = 0;
                errMsg = "Ungueltige SessionId";
                Console.WriteLine(errMsg);
                return false;
            }

            bets = null;
            count = 0;
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
