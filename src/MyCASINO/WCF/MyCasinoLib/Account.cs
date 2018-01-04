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
        List<Bet> betList = new List<Bet>();
        int m_currentMoney;

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
                            if(substring[0]==username)m_currentMoney = money;

                        }
                 }
                
            }
            catch (Exception ex)
            {
                return false;
            }
            return true;
        }

    }
}
