/************************************************************/
/* Content:   Bet class                                     */
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
    public class Bet
    {
        private string m_name;
        private int m_firstNumber;
        private int m_secondNumber;
        private double m_setAmount;

        public string M_name
        {
            get { return m_name; }
            set { m_name = value; }
        }

        public int M_firstNumber
        {
            get { return m_firstNumber; }
            set { m_firstNumber = value; }
        }

        public int M_secondNumber
        {
            get { return m_secondNumber; }
            set { m_secondNumber = value; }
        }

        public double M_setAmount
        {
            get { return m_setAmount; }
            set { m_setAmount = value; }
        }

        public Bet(string name, int firstNumber, int secondNumber, double setAmount)
        {
            M_name = name;
            M_firstNumber = firstNumber;
            M_secondNumber = secondNumber;
            M_setAmount = setAmount;
        }


    }
}
