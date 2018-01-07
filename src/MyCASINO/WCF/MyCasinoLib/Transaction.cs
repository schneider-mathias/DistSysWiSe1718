using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyCasinoLib
{
    public enum MyCasinoTransactionTypes
    {
        DEPOSIT = 0,
        //WITHDRAWL,
        CANCELED,
        BET_WAGER,
        BET_WIN,
        BET_LOSS
    }

    public class Transaction
    {
        private int m_id;

        public int M_id
        {
            get { return m_id; }
            set { m_id = value; }
        }

        private double currentAmount;

        public double CurrentAmount
        {
            get { return currentAmount; }
            set { currentAmount = value; }
        }

        private double changeAmount;

        public double ChangeAmount
        {
            get { return changeAmount; }
            set { changeAmount = value; }
        }

        public Transaction()
        {

        }

        public Transaction(int m_idTmp, double currentAmountTmp, double changeAmounttmp)
        {
            M_id = m_idTmp;
            CurrentAmount = currentAmountTmp;
            ChangeAmount = changeAmounttmp;
        }

    }
}
