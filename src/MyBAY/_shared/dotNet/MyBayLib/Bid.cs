/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Class for creating Bid-objects of each auction          */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     03.01.2018                                              */
/*                                                                       */
/*************************************************************************/

using System;

namespace MyBayLib
{
    /// <summary>
    /// Bid class for storing the bids of an auction internal. Each auction has its own list of bids
    /// </summary>
    // Attribute for making this class serializable
    [Serializable()]
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
            // If startbid is less than 0, it's automatically set to 0
            if (bidValue < 0) bidValue = 0;

            this.BidderIndex = bidderIndex;
            this.BidNumber = bidNumber;
            this.BidValue = bidValue;
        }
        #endregion
    }

    /// <summary>
    /// Class for transferring bids from the server to the client.
    /// The difference to the class bid is, that the index of the bidder
    /// is translated to the name of the bidder
    /// </summary>
    public class BidTransfer
    {
        public UInt32 BidNumber { get; set; }

        public String Bidder { get; set; }

        public Double BidValue { get; set; }
    }
}
