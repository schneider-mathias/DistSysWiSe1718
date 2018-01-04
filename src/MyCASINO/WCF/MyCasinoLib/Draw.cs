using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyCasinoLib
{
    class Draw
    {
        private static List<Bet> drawBetList = new List<Bet>();

        private int m_drawnFirstNumber;

        public int M_drawnFirstNumber
        {
            get { return m_drawnFirstNumber; }
            set { m_drawnFirstNumber = value; }
        }

        private int m_drawnSecondNumber;

        public int M_drawnSecondNumber
        {
            get { return m_drawnSecondNumber; }
            set { m_drawnSecondNumber = value; }
        }

        private double m_resultAmount;

        public double M_resultAmount
        {
            get { return m_resultAmount; }
            set { m_resultAmount = value; }
        }

        private bool m_isDrawn;

        public bool M_isDrawn
        {
            get { return m_isDrawn; }
            set { m_isDrawn = value; }
        }

        public Draw()
        {

        }

        public bool SetBetResult()
        {
            return true;
        }

    }
}
