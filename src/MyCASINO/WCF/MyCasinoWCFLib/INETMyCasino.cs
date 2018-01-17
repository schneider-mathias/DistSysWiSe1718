/************************************************************/
/* Content:   INETMyCasino Interface                        */
/* Autor:     Martin Obermeier                              */
/* Time:      23. Jan 2018                                  */
/************************************************************/
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
        bool login(string username, string password, out ulong sessionId, out short userType, out string errMsg);

        /// <summary>
        /// Logout method to log out of the server
        /// </summary>
        /// <param name="sessionId">Current session ID</param>
        /// <param name="errMsg">Error message</param>
        /// <returns></returns>
        [OperationContract]
        bool logout(ulong sessionId, out string errMsg);

        /// <summary>
        /// A method to pay into the account of a user/casino
        /// </summary>
        /// <param name="sessionId">Current session ID</param>
        /// <param name="name">Username</param>
        /// <param name="amountMoney">The amount of money to be added to the account balance</param>
        /// <param name="errMsg">Error message</param>
        /// <returns></returns>
        [OperationContract]
        bool deposit(ulong sessionId, string name, double amountMoney, out string errMsg);

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
        bool bet(ulong sessionId, double amountMoney, short firstNumber, short secondNumber, out string errMsg);

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
        bool calculateProfit(ulong sessionId, double amountMoney, short firstNumber, short secondNumber, out double profitForOneMatch, out double profitForTwoMatches, out string errMsg);

        //TODO:see list bets
        [OperationContract]
        //bool showbets(int sessionId, out List<Bet> bets, out int count, out string errMsg);
        bool showbets(ulong sessionId, out List<string> names, out List<short> firstNumbers, out List<short> secondNumbers, out List<double> amount , out ulong count, out string errMsg);

        /// <summary>
        /// A method to test drawing
        /// </summary>
        /// <param name="sessionId"></param>
        /// <param name="firstNumberTest"></param>
        /// <param name="secondNumberTest"></param>
        /// <param name="errMsg"></param>
        /// <returns></returns>
        [OperationContract]
        bool drawtest(ulong sessionId, short firstNumberTest, short secondNumberTest, out string errMsg);

        /// <summary>
        /// A method to draw two random numbers
        /// </summary>
        /// <param name="sessionId">Current session id</param>
        /// <param name="firstNumber">First random number</param>
        /// <param name="secondNumber">Second random number</param>
        /// <param name="errMsg">Error message</param>
        /// <returns></returns>
        [OperationContract]
        bool draw(ulong sessionId, out short firstNumber, out short secondNumber, out string errMsg);

        [OperationContract]
        bool getTransactions(ulong sessionId, out bool isFinished, out List<string> transaction, out ulong transactionType, out string errMsg);
        [OperationContract]
        bool getTransactionInformation(ulong sessionId, ulong transactionId, out List<string> information, out ulong informationType, out string errMsg);



    }
}
