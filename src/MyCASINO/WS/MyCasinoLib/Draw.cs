/************************************************************/
/* Content:   Draw class                                    */
/* Autor:     Martin Obermeier                              */
/* Time:      23. Jan 2018                                  */
/************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyCasinoLib
{
    public class Draw
    {
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
        public Draw(Bet bet, int firstNumberDrawn, int secondNumberDrawn, double amountWon)
        {
            DrawBet = bet;
            MoneyWon = amountWon;
            M_drawnFirstNumber = firstNumberDrawn;
            M_drawnSecondNumber = secondNumberDrawn;
        }
    }
}
