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

    class Transaction
    {
        private int m_id;

        public int M_id
        {
            get { return m_id; }
            set { m_id = value; }
        }

        private int m_type;

        public int M_type
        {
            get { return m_type; }
            set { m_type = value; }
        }


    }
}
