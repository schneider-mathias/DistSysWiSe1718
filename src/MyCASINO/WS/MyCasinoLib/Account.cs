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
        /// <summary>
        /// lock object for betlist
        /// </summary>
        private Object thisLockUserList = new Object();
        /// <summary>
        /// Betlist for active but unfinished bets
        /// </summary>
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
            lock (thisLockUserList)
            {
                bets.AddRange(betList);
            }
        }
        public bool ReadUserTransaction(string username, Dictionary<Transaction, Draw> dictTransDraw)
        {
            try
            {
                //Read UserBalance.txt line by line
#if _IIS_DEPLOY_ // is set for Release configuration
                using (FileStream fs = File.OpenRead("C:\\inetpub\\wwwroot\\UserBalance.txt"))
#else
                using (FileStream fs = File.OpenRead(Environment.GetEnvironmentVariable("SystemDrive") + "\\_MyCasinoData\\UserBalance.txt"))
#endif
                using (StreamReader sr = new StreamReader(fs))
                {
                    string line;
                    while ((line = sr.ReadLine()) != null)
                    {
                        double money = 0;
                        string[] substring = line.Split();
                        Double.TryParse(substring[1], out money);
                        if (substring[0] == username)
                        {
                            //read moneyamount from file
                            //Dictonary is already locked
                            Transaction readFileTransaction = new Transaction();
                            if (dictTransDraw.Count == 0)
                            {
                                readFileTransaction.M_id = 1;
                            }
                            else
                            {
                                readFileTransaction.M_id = dictTransDraw.Last().Key.M_id + 1;
                            }
                            readFileTransaction.CurrentAmount = money;
                            MoneyAmountLeft = money;
                            readFileTransaction.ChangeAmount = 0;
                            readFileTransaction.Name = username;
                            readFileTransaction.TransType = MyCasinoTransactionTypes.DEPOSIT;
                            dictTransDraw.Add(readFileTransaction, null);
                        }
                    }
                }
            }
            catch (IOException ex)
            {
                Console.Write("FILE_READING_ERROR: " + ex);
                return false;
            }
            return true;
        }

        public bool Deposit(double amountMoney, Dictionary<Transaction, Draw> dictTransDraw, string name, MyCasinoTransactionTypes typeTmp)
        {
            try
            {
                int index = dictTransDraw.Count - 1;
                for (int k = dictTransDraw.Count-1; k > 1; k--)
                {
                    index = k;
                    if (dictTransDraw.ElementAt(k).Key.TransType == MyCasinoTransactionTypes.DEPOSIT &&
                        dictTransDraw.ElementAt(k).Key.Name == name) break;
                    else if (dictTransDraw.ElementAt(k).Key.TransType == MyCasinoTransactionTypes.BET_WIN &&
                        dictTransDraw.ElementAt(k).Key.Name == name) break;
                    else if (dictTransDraw.ElementAt(k).Key.TransType == MyCasinoTransactionTypes.BET_LOSS &&
                        dictTransDraw.ElementAt(k).Key.Name == name) break;
                }
                //already locked
                Transaction trans = new Transaction(dictTransDraw.Last().Key.M_id + 1, dictTransDraw.ElementAt(index).Key.CurrentAmount + amountMoney, amountMoney,name,typeTmp);
                dictTransDraw.Add(trans,null);
                moneyAmountLeft += amountMoney;
                return true;
            }
            catch
            {
                return false;
            }
        }

        public bool Bet(string nameTmp ,double amountMoneyTmp, int firstNumberTmp, int secondNumberTmp, out  MyCasinoTransactionTypes typetmp, out bool overridden, out bool delOverriddenBet, out bool _betDel)
        {
                overridden = true;
                delOverriddenBet = false;
                _betDel = false;
            try
            {
                Bet tmpbet = new Bet(nameTmp, firstNumberTmp, secondNumberTmp, amountMoneyTmp);
                //delete bet
                lock (thisLockUserList)
                {
                    Bet delbet = betList.Find(item => item.M_firstNumber == firstNumberTmp && item.M_secondNumber == secondNumberTmp && 0 == amountMoneyTmp);
                    if (delbet != null)
                    {
                        MoneyAmountLeft += delbet.M_setAmount;
                        typetmp = MyCasinoTransactionTypes.CANCELED;
                        betList.Remove(delbet);
                        _betDel = true;
                        delOverriddenBet = true;
                        return true;
                    }
                }
                if (amountMoneyTmp == 0)
                {
                    typetmp = MyCasinoTransactionTypes.CANCELED;
                    return true;
                }
                lock (thisLockUserList)
                {
                    Bet bet = betList.Find(item => item.M_firstNumber == firstNumberTmp && item.M_secondNumber == secondNumberTmp);
                    //delete bet to override it
                    if (bet != null)
                    {
                        MoneyAmountLeft += bet.M_setAmount;
                        delOverriddenBet = true;
                        betList.Remove(bet);
                    }
                    //add bet
                    overridden = false;
                    typetmp = MyCasinoTransactionTypes.BET_WAGER;
                    betList.Add(tmpbet);
                }
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
                lock (thisLockUserList)
                {
                    for (int i = 0; i < betList.Count; i++)
                    {
                        bets.Add(betList.ElementAt(i));
                    }
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
                profitForTwoMatches = profitForOneMatch;
                lock (thisLockUserList)
                {
                    for (int i = 0; i < betList.Count; i++)
                    {
                        profitForTwoMatches += betList.ElementAt(i).M_setAmount;
                    }
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
                lock (thisLockUserList)
                {
                    foreach (Bet bet in betList)
                    {
                        //Two matches profit save
                        if (bet.M_firstNumber == firstNumber && bet.M_secondNumber == secondNumber)
                        {
                            CalculateProfit(bet.M_setAmount, out oneMatch, out twoMatches);
                            profit.Add(twoMatches);
                        }
                        //One match profit save
                        else if (bet.M_firstNumber == firstNumber || bet.M_secondNumber == secondNumber || bet.M_firstNumber == secondNumber
                            || bet.M_secondNumber == firstNumber)
                        {
                            CalculateProfit(bet.M_setAmount, out oneMatch, out twoMatches);
                            profit.Add(oneMatch);
                        }
                        else profit.Add(0);
                    }
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
                lock (thisLockUserList)
                {
                    betList.Clear();
                }
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
                lock (thisLockUserList)
                {
                    bets.AddRange(betList);
                }
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
