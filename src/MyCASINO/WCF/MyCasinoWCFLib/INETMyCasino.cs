using MyCasinoLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace MyCasinoWCFLib
{
    [ServiceContract]
    public interface INETMyCasino
    {
        /// <summary>
        /// Login method to log onto the MyCasino server
        /// </summary>
        /// <param name="username">Username</param>
        /// <param name="password">Password</param>
        /// <param name="sessionId">Session ID</param>
        /// <param name="userType">Usertype, casinoowner or normal user</param>
        /// <param name="errMsg">Error Message</param>
        /// <returns></returns>
        [OperationContract]
        bool login(string username, string password, out int sessionId, out MyCasinoUserTypes userType, out string errMsg);

        /// <summary>
        /// Logout method to log out of the server
        /// </summary>
        /// <param name="sessionId">Current session ID</param>
        /// <param name="errMsg">Error message</param>
        /// <returns></returns>
        [OperationContract]
        bool logout(int sessionId, out string errMsg);

        /// <summary>
        /// A method to pay into the account of a user/casino
        /// </summary>
        /// <param name="sessionId">Current session ID</param>
        /// <param name="name">Username</param>
        /// <param name="amountMoney">The amount of money to be added to the account balance</param>
        /// <param name="errMsg">Error message</param>
        /// <returns></returns>
        [OperationContract]
        bool deposit(int sessionId, string name, double amountMoney, out string errMsg);

        /// <summary>
        /// A method to enter bets
        /// </summary>
        /// <param name="sessionID">Current session ID</param>
        /// <param name="amountMoney">The amount of money that has been bet on one draw</param>
        /// <param name="firstNumber">First number to be bet on</param>
        /// <param name="secondNumber">Second number to be bet on</param>
        /// <param name="errMsg">Error message</param>
        /// <returns></returns>
        [OperationContract]
        bool bet(int sessionId, double amountMoney, int firstNumber, int secondNumber, out string errMsg);

        /// <summary>
        /// A method to calculate the profit of bets
        /// </summary>
        /// <param name="sessionId"></param>
        /// <param name="amountMoney"></param>
        /// <param name="profitForOneMatch"></param>
        /// <param name="profitForTwoMatches"></param>
        /// <param name="errMsg"></param>
        /// <returns></returns>
        [OperationContract]
        bool calculateProfit(int sessionId, double amountMoney, out double profitForOneMatch, out double profitForTwoMatches, out string errMsg);

        //TODO:see list bets
        [OperationContract]
        bool showbets(int sessionId, out List<string> bets, out int count, out string errMsg);

        /// <summary>
        /// A method to test drawing
        /// </summary>
        /// <param name="sessionId"></param>
        /// <param name="firstNumberTest"></param>
        /// <param name="secondNumberTest"></param>
        /// <param name="errMsg"></param>
        /// <returns></returns>
        [OperationContract]
        bool drawtest(int sessionId, int firstNumberTest, int secondNumberTest, out string errMsg);

        /// <summary>
        /// A method to draw two random numbers
        /// </summary>
        /// <param name="sessionId">Current session id</param>
        /// <param name="firstNumber">First random number</param>
        /// <param name="secondNumber">Second random number</param>
        /// <param name="errMsg">Error message</param>
        /// <returns></returns>
        [OperationContract]
        bool draw(int sessionId, out int firstNumber, out int secondNumber, out string errMsg);

        //TODO:see list transaction
        [OperationContract]
        bool getTransactions(int sessionId, out bool isFinished, out List<string> transaction, out int transactionType, out string errMsg);
        //TODO:see list information
        [OperationContract]
        bool getTransactionInformation(int sessionId, int transactionId, out List<string> information, out int informationType, out string errMsg);



    }
}
