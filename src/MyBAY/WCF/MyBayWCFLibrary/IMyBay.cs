/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Interface of the MyBay WCF-Server                       */
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
using System.ServiceModel;
using MyBayLib;

namespace MyBayWCFLibrary
{
    /// <summary>
    /// Implementing the Interface definition
    /// </summary>
    [ServiceContract]
    public interface IMyBay
    {
        #region Methods
        [OperationContract]
        String login(String userName, String password, out UInt32 sessionID);

        [OperationContract]
        String offer(UInt32 sessionID, String artName, Double startBid, out UInt32 auctionNumber);

        [OperationContract]
        String interested(UInt32 sessionID, UInt32 auctionNumber);

        // TODO define AUCTION, implement in lib for MyBay
        [OperationContract]
        String getAuctions(UInt32 sessionID, UInt32 flags, String artName, out UInt32 countAuctions, out List<Auction> auctions);

        [OperationContract]
        String bid(UInt32 sessionID, UInt32 auctionNumber, Double bidVal);

        // TODO define BID, implement in lib for MyBay
        [OperationContract]
        String details(UInt32 sessionID, UInt32 auctionNumber, out UInt32 countBids, out List<Bid> allBids);

        [OperationContract]
        String endauction(UInt32 sessionID, UInt32 auctionNumber);

        [OperationContract]
        String logout(UInt32 sessionID);

        // TODO define Message, 0 Neues Gebot, 1 Auktionsende start, 2 Ende Auktion, 3 Infomessage
        [OperationContract]
        String getMessage(UInt32 sessionID, out Boolean messageAvailable, out UInt32 messageType, out Message message);

        #endregion
    }
}
