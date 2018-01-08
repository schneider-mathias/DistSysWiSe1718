using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyCasinoLib
{
    public class Account
    {
        //List<Transaction> transactionList = new List<Transaction>();
        //List<Bet> betResultList = new List<Bet>();
        List<Bet> betList = new List<Bet>();

        private double moneyAmountLeft;

        public double MoneyAmountLeft
        {
            get { return moneyAmountLeft; }
            set { moneyAmountLeft = value; }
        }

                public Account()
        {

        }

        public void getBetList(out List<Bet> bets)
        {
            bets = new List<Bet>();
            bets.AddRange(betList);
        }

        //public bool ReadUserTransaction(string username, List<Transaction>transactionList)
        public bool ReadUserTransaction(string username, Dictionary<Transaction, Draw> dictTransDraw)
        {
            try
            {
                //Read UserTransactionfile.csv line by line
                using (FileStream fs = File.OpenRead(@"d:\StdArbVS\trunk\src\MyCASINO\WCF\MyCasinoData\UserBalance.txt"))
                using (StreamReader sr = new StreamReader(fs))
                {
                 
                        string line;
                        while ((line = sr.ReadLine()) != null)
                        {
                            int money = 0;
                            string[] substring = line.Split();
                            Int32.TryParse(substring[1], out money);
                        if (substring[0] == username)
                        {
                            //read moneyamount from file
                            Transaction readFileTransaction = new Transaction();
                            readFileTransaction.M_id = 1;
                            readFileTransaction.CurrentAmount = money;
                            MoneyAmountLeft = money;
                            readFileTransaction.ChangeAmount = 0;
                            readFileTransaction.Name=username;
                            readFileTransaction.TransType = MyCasinoTransactionTypes.DEPOSIT;
                            dictTransDraw.Add(readFileTransaction,null);
                        }
                        }
                 }
                
            }
            catch (Exception ex)
            {
                return false;
            }
            return true;
        }

        public bool Deposit(double amountMoney, Dictionary<Transaction, Draw> dictTransDraw, string name, MyCasinoTransactionTypes typeTmp)
        {
            try
            {
                Transaction trans = new Transaction(dictTransDraw.Last().Key.M_id + 1, dictTransDraw.Last().Key.CurrentAmount + amountMoney, amountMoney,name,typeTmp);
                dictTransDraw.Add(trans,null);
                moneyAmountLeft += amountMoney;
                return true;
            }
            catch
            {
                return false;
            }
        }

        public bool Bet(string nameTmp ,double amountMoneyTmp, int firstNumberTmp, int secondNumberTmp, out  MyCasinoTransactionTypes typetmp, out bool overridden)
        {
                overridden = false;
            try
            {
                Bet tmpbet = new Bet(nameTmp, firstNumberTmp, secondNumberTmp, amountMoneyTmp);
                //delete bet
                Bet delbet= betList.Find(item => item.M_firstNumber == firstNumberTmp && item.M_secondNumber == secondNumberTmp && 0==amountMoneyTmp);
                if (delbet!=null)
                {
                    typetmp = MyCasinoTransactionTypes.CANCELED;
                    betList.Remove(delbet);
                    return true;
                }

                Bet bet = betList.Find(item => item.M_firstNumber == firstNumberTmp && item.M_secondNumber == secondNumberTmp);
                //delete bet to override it
                if (bet!=null)
                {
                    overridden = true;
                    betList.Remove(bet);
                }
                //add bet
                typetmp = MyCasinoTransactionTypes.BET_WAGER;
                betList.Add(tmpbet);
                return true;
            }
            catch
            {
                typetmp = MyCasinoTransactionTypes.CANCELED;
                return false;
            }
        }

        public bool Showbets(out List<Bet> bets)
        {
            //Initialize
            bets = null;
            bets = new List<Bet>();
            //try to copy a single userinformation into betlist for showbets
            try
            {
                for (int i = 0; i < betList.Count; i++)
                {
                    bets.Add(betList.ElementAt(i));
                }
                
                return true;
            }
            catch
            {
                
                return false;
            }
        }

        public bool CalculateProfit(double amountMoney, out double profitForOneMatch, out double profitForTwoMatches)
        {
            //Init
            profitForOneMatch = 0;
            profitForTwoMatches = 0;

            //Calculate profit for one and two matches of numbers
            try
            {
                profitForOneMatch = amountMoney * 2;
                profitForTwoMatches = profitForOneMatch; //TODO: Profit ???
                for (int i = 0; i < betList.Count; i++)
                {
                    profitForTwoMatches += betList.ElementAt(i).M_setAmount;
                }
                return true;
            }
            catch
            {
                return false;
            }

        }

        public bool Profit(int firstNumber, int secondNumber, out List<double> profit)
        {
            //init
            double oneMatch = 0, twoMatches = 0;
            profit = new List<double>();
            try
            {
                foreach (Bet bet in betList)
                {
                    //One match profit save
                    if (bet.M_firstNumber == firstNumber || bet.M_secondNumber == secondNumber || bet.M_firstNumber == secondNumber
                        || bet.M_secondNumber == firstNumber)
                    {
                        CalculateProfit(bet.M_setAmount, out oneMatch, out twoMatches);
                        profit.Add(oneMatch);
                    }
                    //Two matches profit save
                    else if (bet.M_firstNumber == firstNumber && bet.M_secondNumber == secondNumber)
                    {
                        CalculateProfit(bet.M_setAmount, out oneMatch, out twoMatches);
                        profit.Add(twoMatches);
                    }
                    else profit.Add(0);
                }
                return true;
            }
            catch
            {
                return false;
            }
        }


        public bool Draw(out int firstNumber, out int secondNumber)
        {
            //init
            firstNumber = 0;
            secondNumber = 0;
            try
            {
                do
                {
                    Random rnd = new Random();
                    firstNumber = rnd.Next(1, 5);
                    secondNumber = rnd.Next(1, 5);
                } while (!(firstNumber < secondNumber));
                return true;
            }
            catch
            {
                return false;
            }
        }

        public bool DelBets()
        {
            try
            {
                betList.Clear();
                return true;
            }
            catch
            {
                return false;
            }
        }

        //evtl löschen
        public bool SaveBetsDrawings(out List<Bet> bets)
        {
            //Initialize
            bets = null;
            bets = new List<Bet>();
            try
            {
                bets.AddRange(betList);
                return true;
            }
            catch
            {
                bets = null;
                return false;
            }
        }
    }
}
