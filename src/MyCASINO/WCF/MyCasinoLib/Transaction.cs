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
        WITHDRAWL,
        CANCELED,
        BET_WAGER,
        BET_WIN,
        BET_LOSS
    }
    public class Transaction
    {
        private MyCasinoTransactionTypes transType;
        public MyCasinoTransactionTypes TransType
        {
            get { return transType; }
            set { transType = value; }
        }
        private string name;
        public string Name
        {
            get { return name; }
            set { name = value; }
        }
        private ulong m_id;
        public ulong M_id
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
        private bool isFinished;
        public bool IsFinished
        {
            get { return isFinished; }
            set { isFinished = value; }
        }
        public Transaction()
        {
        }
        public Transaction(ulong m_idTmp, double currentAmountTmp, double changeAmountTmp, string nameTmp, MyCasinoTransactionTypes typeTmp)
        {
            M_id = m_idTmp;
            CurrentAmount = currentAmountTmp;
            ChangeAmount = changeAmountTmp;
            Name = nameTmp;
            TransType = typeTmp;
            IsFinished = false;
        }
    }
}
