/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Bids des MyBay WCF-Servers                              */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     03.01.2018                                              */
/*                                                                       */
/*************************************************************************/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyBayLib
{
    public class Bid
    {
        #region Properties
        public UInt32 BidNumber { get; private set; }

        public UInt32 BidderIndex { get; private set; }

        public Double BidValue { get; private set; }
        #endregion

        #region C'tors
        public Bid(UInt32 bidNumber, UInt32 bidderIndex, Double bidValue)
        {
            this.BidderIndex = bidderIndex;
            this.BidNumber = bidNumber;
            this.BidValue = bidValue;
        }
        #endregion
    }

    public class BidTransfer
    {
        public UInt32 BidNumber { get; set; }

        public String Bidder { get; set; }

        public Double BidValue { get; set; }
    }
}
