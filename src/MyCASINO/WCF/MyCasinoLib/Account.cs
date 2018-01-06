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
        List<Transaction> transactionList = new List<Transaction>();
        List<Bet> betResultList = new List<Bet>();
        List<Bet> betList = new List<Bet>();

        public Account()
        {

        }

        public bool ReadUserTransaction(string username)
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
                            readFileTransaction.ChangeAmount = 0;
                            transactionList.Add(readFileTransaction);
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

        public bool Deposit(double amountMoney)
        {
            try
            {
                Transaction trans = new Transaction(transactionList.Last().M_id + 1, transactionList.Last().CurrentAmount + amountMoney, amountMoney);
                transactionList.Add(trans);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public bool Bet(string nameTmp ,double amountMoneyTmp, int firstNumberTmp, int secondNumberTmp)
        {
            try
            {
                Bet tmpbet = new Bet(nameTmp, firstNumberTmp, secondNumberTmp, amountMoneyTmp);
                //delete bet
                Bet delbet= betList.Find(item => item.M_firstNumber == firstNumberTmp && item.M_secondNumber == secondNumberTmp && item.M_setAmount==amountMoneyTmp);
                if (delbet!=null)
                {
                    betList.Remove(delbet);
                    return true;
                }

                Bet bet = betList.Find(item => item.M_firstNumber == firstNumberTmp && item.M_secondNumber == secondNumberTmp);
                //delete bet to override it
                if (bet!=null)
                {
                    betList.Remove(bet);
                }
                //add bet
                betList.Add(tmpbet);
                return true;
            }
            catch
            {
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
    }
}
