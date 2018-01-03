/*************************************************************************/
/*                                                                       */
/*    Inhalt:   Implementierung der Funktionalität des MyBay WCF-Servers */
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
using MyBayWCFSrv;
using MyBayLib;

namespace MyBayWCFSrv
{
    class MyBayWCFSrv : MyBayWCFLibrary.IMyBay
    {
        public String login(String userName, String password, out UInt32 sessionID)
        {
            #region DummyData
            sessionID = 0;
            return "";
            #endregion
        }

        public String offer(UInt32 sessionID, String artName, Double startBid, out UInt32 auctionNumber)
        {
            #region DummyData
            auctionNumber = 0;
            return "";
            #endregion
        }

        public String interested(UInt32 sessionID, UInt32 auctionNumber)
        {
            #region DummyData
            return "";
            #endregion
        }


        public String getAuctions(UInt32 sessionID, UInt32 flags, String artName, out UInt32 countAuctions, out List<Auction> auctions)
        {
            #region DummyData
            countAuctions = 0;
            auctions = new List<Auction>();
            return "";
            #endregion
        }

        public String bid(UInt32 sessionID, UInt32 auctionNumber, Double bidVal)
        {
            #region DummyData
            return "";
            #endregion
        }

        public String details(UInt32 sessionID, UInt32 auctionNumber, out UInt32 countBids, out List<Bid> allBids)
        {
            #region DummyData
            countBids = 0;
            allBids = new List<Bid>();
            return "";
            #endregion
        }

        public String endauction(UInt32 sessionID, UInt32 auctionNumber)
        {
            #region DummyData
            return "";
            #endregion
        }

        public String logout(UInt32 sessionID)
        {
            #region DummyData
            return "";
            #endregion
        }

        public String getMessage(UInt32 sessionID, out Boolean messageAvailable, out UInt32 messageType, out Message message)
        {
            #region DummyData
            messageType = 0;
            messageAvailable = false;
            message = new Message();
            return "";
            #endregion
        }
    }
}
