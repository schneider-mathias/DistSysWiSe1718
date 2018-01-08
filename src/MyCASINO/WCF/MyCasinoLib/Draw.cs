using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyCasinoLib
{
    public class Draw
    {
        //private static List<Bet> drawBetList = new List<Bet>();
        //private static List<double> moneyWonList = new List<double>();

        private Bet drawBet;

        public Bet DrawBet
        {
            get { return drawBet; }
            set { drawBet = value; }
        }

        private double moneyWon;

        public double MoneyWon
        {
            get { return moneyWon; }
            set { moneyWon = value; }
        }


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

        //public Draw(List<Bet> bet, int firstNumberDrawn, int secondNumberDrawn, List<double> amountWon)
        //{
        //    for (int i = 0; i<bet.Count; i++)
        //    {
        //        drawBetList.Add(bet.ElementAt(i));
        //        moneyWonList.Add(amountWon.ElementAt(i));
        //    }

        //    M_drawnFirstNumber = firstNumberDrawn;
        //    M_drawnSecondNumber = secondNumberDrawn;
        //}
        public Draw(Bet bet, int firstNumberDrawn, int secondNumberDrawn, double amountWon)
        {
            DrawBet = bet;
            MoneyWon = amountWon;
            M_drawnFirstNumber = firstNumberDrawn;
            M_drawnSecondNumber = secondNumberDrawn;
        }

       
    }
}
