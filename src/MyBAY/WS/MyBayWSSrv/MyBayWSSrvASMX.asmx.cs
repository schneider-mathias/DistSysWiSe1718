/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Implementierung des MyBay - Webservices                 */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     14.01.2018                                              */
/*                                                                       */
/*************************************************************************/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Services;
using AuthenticationService;
using MyBayLib;
using System.Collections.Concurrent;

namespace MyBayWSSrv
{
    /// <summary>
    /// Class MyBayWSSrv is implementing the Web-Service for providing an endpoint for the Windows Phone App
    /// 
    /// </summary>
    [WebService(Namespace = "http://MyBayServer.org/")]
    [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
    [System.ComponentModel.ToolboxItem(false)]
    public class MyBayWSSrvASMX : System.Web.Services.WebService
    {
        /// <summary>
        /// This is a private list of auctions which is initialized during the start of the Web-Service in it's constructor
        /// The list is either created new or the list is created from the persistent data on the hard disk drive, if the service 
        /// was already executed before on the same machine
        /// </summary>
        private static List<Auction> listAuctions;

        #region C'Tors
        /// <summary>
        /// Constructor of the class MayBayWSSrvASMX
        /// </summary>
        public MyBayWSSrvASMX()
        {
            AuthService.initializeAuthService();

            // Check if Auctionlist is already instantiated
            if (MyBayWSSrvASMX.listAuctions == null)
            {
                MyBayWSSrvASMX.listAuctions = Auction.GetPersistentAuctions();
                // check if reading from file was successful
                if (MyBayWSSrvASMX.listAuctions != null)
                {
                    Auction.setCountAuctions((UInt32)MyBayWSSrvASMX.listAuctions.Count);
                }
                // if reading from file not successfull, create new list
                else MyBayWSSrvASMX.listAuctions = new List<Auction>();
            }
        }
        #endregion

        #region Methods
        /// <summary>
        /// Login method to log in a user
        /// </summary>
        /// <param name="userName"></param>
        /// <param name="password"></param>
        /// <param name="sessionID"></param>
        /// <returns></returns>
        [WebMethod]
        public String login(String userName, String password, out UInt32 sessionID)
        {
            UInt32 loginSessionID;

            String ReturnMessage = AuthenticationService.AuthService.Login(userName, password, out loginSessionID);

            if (ReturnMessage.Contains("OK"))
            {
                sessionID = loginSessionID;
                return "OK";
            }
            
            // If login failed, set sessionID of user to 0 (not logged in)
            sessionID = 0;
            return ReturnMessage;
        }

        /// <summary>
        /// logout method to log out a user
        /// </summary>
        /// <param name="sessionID"></param>
        /// <returns></returns>
        [WebMethod]
        public String logout(UInt32 sessionID)
        {
            return AuthenticationService.AuthService.Logout(sessionID);
        }

        /// <summary>
        /// Offer method to create a new auction and add it to the list of auctions
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="artName"></param>
        /// <param name="startBid"></param>
        /// <param name="auctionNumber"></param>
        /// <returns></returns>
        [WebMethod]
        public String offer(UInt32 sessionID, String artName, Double startBid, out UInt32 auctionNumber)
        {
            auctionNumber = 0;
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            // Translating current sessionID of client to the index of the user which is logged in
            UInt32 auctioneerIndex = AuthenticationService.AuthService.getIndexBySessionID(sessionID);

            Auction newAuction = new Auction(artName, startBid, auctioneerIndex);
            auctionNumber = newAuction.AuctionNumber;

            lock (MyBayWSSrvASMX.listAuctions)
            {
                MyBayWSSrvASMX.listAuctions.Add(newAuction);
            }
            Auction.SaveAuctionsPersistent(MyBayWSSrvASMX.listAuctions);

            return "OK";
        }

        /// <summary>
        /// Method to add a user on the list of interested users of an auction so that
        /// the user will be informed about updates on this auction
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="auctionNumber"></param>
        /// <returns></returns>
        [WebMethod]
        public String interested(UInt32 sessionID, UInt32 auctionNumber)
        {
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            lock (MyBayWSSrvASMX.listAuctions)
            {
                Auction auctionTemp;
                try
                {
                    auctionTemp = MyBayWSSrvASMX.listAuctions.Find(x => x.AuctionNumber == auctionNumber);
                }
                catch (Exception)
                {
                    return "Die angegebene Auktionsnummer konnte nicht gefunden werden";
                }
                // Translating current sessionID of client to the index of the user which is logged in and
                // add it to the list of interested users of the auction with the passed auctionNumber
                auctionTemp.addToInterested(AuthenticationService.AuthService.getIndexBySessionID(sessionID));
            }

            // Write all auctions in a file
            Auction.SaveAuctionsPersistent(MyBayWSSrvASMX.listAuctions);
            return "OK";
        }

        /// <summary>
        /// Method to go through the list of auctions and filter them by the criteria
        /// the calling client was passing with the parameters
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="flags">Can be 0, 1 and 2:
        /// 0: all open auctions where the calling client (user) is interested in
        /// 1: all open auctions
        /// 2: all auctions - open and closed
        /// </param>
        /// <param name="artName">
        /// If this string is not empty, the auctions are also filtered by the name of the auction
        /// </param>
        /// <param name="countAuctions"></param>
        /// <param name="auctions"></param>
        /// <returns></returns>
        [WebMethod]
        public String getAuctions(UInt32 sessionID, UInt32 flags, String artName, out UInt32 countAuctions, out List<AuctionTransfer> auctions)
        {
            countAuctions = 0;
            auctions = new List<AuctionTransfer>();
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";
            UInt32 userIndex = AuthenticationService.AuthService.getIndexBySessionID(sessionID);

            List<Auction> personalizedAuctionList;

            // flags can be 0: normal getAuctions 1: get all Auctions 2: get all Auctions including the one which are ended
            switch (flags)
            {
                case 0: // Get all auctions where User is Interested in and which are not closed
                    lock (MyBayWSSrvASMX.listAuctions)
                    {
                        // ConvertAll Method is being used, because Method creates a deepcopy of the Object with creating new Objects -> that listAuctions is not blocked too long
                        personalizedAuctionList = MyBayWSSrvASMX.listAuctions.FindAll(w => (w.BidderInterested.Contains(userIndex) && w.AuctionState != 2)).ConvertAll(item => new Auction(item));
                    }
                    break;
                case 1: // Get all auctions which are open
                    lock (MyBayWSSrvASMX.listAuctions)
                    {
                        personalizedAuctionList = MyBayWSSrvASMX.listAuctions.FindAll(w => w.AuctionState != 2).ConvertAll(item => new Auction(item));
                    }
                    break;
                case 2: // Get all auctions
                    lock (MyBayWSSrvASMX.listAuctions)
                    {
                        personalizedAuctionList = MyBayWSSrvASMX.listAuctions.ConvertAll(item => new Auction(item));
                    }
                    break;
                default:
                    personalizedAuctionList = new List<Auction>();
                    break;
            }

            // If passed string for filtering with the articleName is not empty
            if (!String.IsNullOrEmpty(artName))
            {
                personalizedAuctionList = personalizedAuctionList.FindAll(item => item.ArtName.StartsWith(artName));
            }

            // if personalizedAuctionList doesnt contain any items
            if (personalizedAuctionList.Count < 1)
            {
                return "Keine Auktionen mit den angegebenen Parametern gefunden";
            }

            // Create new list with AuctionTransfer objects for passing the found information back to the client
            foreach (Auction auct in personalizedAuctionList)
            {
                AuctionTransfer transferItem = new AuctionTransfer();
                transferItem.ArtName = auct.ArtName;
                transferItem.AuctNumber = auct.AuctionNumber;
                transferItem.CountBids = auct.CountBids;
                transferItem.HighestBid = auct.HighestBid.BidValue;
                transferItem.AuctionState = auct.AuctionState;

                auctions.Add(transferItem);
            }
            countAuctions = (UInt32)auctions.Count;

            return "OK";
        }

        /// <summary>
        /// Method for bidding on an article
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="auctionNumber"></param>
        /// <param name="bidVal"></param>
        /// <returns></returns>
        [WebMethod]
        public String bid(UInt32 sessionID, UInt32 auctionNumber, Double bidVal)
        {
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            // Find the auction with the passed auctionNumber
            Auction auctionTemp;
            lock (MyBayWSSrvASMX.listAuctions)
            {
                try
                {
                    auctionTemp = MyBayWSSrvASMX.listAuctions.Find(x => x.AuctionNumber == auctionNumber);
                }
                catch (Exception)
                {
                    return "Die angegebene Auktionsnummer konnte nicht gefunden werden";
                }
            }

            // Call method for adding the bid with the passed value (bidVal)
            String returnMsg = auctionTemp.addBid(AuthenticationService.AuthService.getIndexBySessionID(sessionID), bidVal);
            if (returnMsg == "OK")
            {
                // Write all auctions in a file
                Auction.SaveAuctionsPersistent(MyBayWSSrvASMX.listAuctions);
            }
            return returnMsg;
        }

        /// <summary>
        /// Method for getting all Bids of an auction, can just be executed by the auctioneer of an auction
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="auctionNumber"></param>
        /// <param name="countBids"></param>
        /// <param name="allBids"></param>
        /// <returns></returns>
        [WebMethod]
        public String details(UInt32 sessionID, UInt32 auctionNumber, out UInt32 countBids, out List<BidTransfer> allBids)
        {
            countBids = 0;
            allBids = new List<BidTransfer>();
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            List<Bid> copyBids;
            lock (MyBayWSSrvASMX.listAuctions)
            {
                try
                {
                    Auction tempAuction = MyBayWSSrvASMX.listAuctions.Find(x => x.AuctionNumber == auctionNumber);
                    if (tempAuction.Auctioneer != AuthenticationService.AuthService.getIndexBySessionID(sessionID))
                    {
                        return "Nur der Ersteller der Auktion kann sich die Details anzeigen lassen";
                    }
                    copyBids = tempAuction.getBids();
                }
                catch (Exception)
                {
                    return "Die angegebene Auktionsnummer konnte nicht gefunden werden";
                }
            }

            // Create new list with BidTransfer objects for passing the found bids back to the client
            foreach (Bid bid in copyBids)
            {
                BidTransfer tempTransfer = new BidTransfer();
                tempTransfer.Bidder = AuthenticationService.AuthService.getNameByIndex(bid.BidderIndex);
                tempTransfer.BidNumber = bid.BidNumber;
                tempTransfer.BidValue = bid.BidValue;
                allBids.Add(tempTransfer);
            }
            countBids = (UInt32)copyBids.Count;

            return "OK";
        }

        /// <summary>
        /// Method for ending an auction
        /// An auction can just be ended if the calling client (user) is the auctioneer of the auction with the passed auctionNumber
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="auctionNumber"></param>
        /// <returns></returns>
        [WebMethod]
        public String endauction(UInt32 sessionID, UInt32 auctionNumber)
        {
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            lock (MyBayWSSrvASMX.listAuctions)
            {
                Auction tempAuction;
                try
                {
                    tempAuction = MyBayWSSrvASMX.listAuctions.FirstOrDefault(x => x.AuctionNumber == auctionNumber);
                    if (tempAuction == default(Auction))
                    {
                        return "Auktion mit der Auktionsnummer: " + auctionNumber + " wurde nicht gefunden";
                    }
                    if (tempAuction.Auctioneer != AuthenticationService.AuthService.getIndexBySessionID(sessionID))
                    {
                        return "Nur der Ersteller der Auktion kann die Auktion beenden";
                    }
                }
                catch (Exception)
                {
                    return "Die angegebene Auktionsnummer konnte nicht gefunden werden";
                }

                string retValue = tempAuction.endAuction();

                if (retValue == "OK") Auction.SaveAuctionsPersistent(MyBayWSSrvASMX.listAuctions);

                return retValue;
            }
        }

        /// <summary>
        /// Method checks, if there are any messages for the calling client (user) in the 
        /// static list of the Auction class.
        /// If there is are messages, they will be copied to a list of MessageTransfer objects and passed back to the client
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="messageAvailable"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        [WebMethod]
        public String getMessage(UInt32 sessionID, out Boolean messageAvailable, out List<MessageTransfer> message)
        {
            messageAvailable = false;
            message = new List<MessageTransfer>();
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            Message tempMessage;

            try
            {
                // Translate SessionID to UserIndex
                UInt32 userIndex = AuthenticationService.AuthService.getIndexBySessionID(sessionID);

                if (Auction.messageBucket.ContainsKey(userIndex))
                {
                    ConcurrentBag<Message> tempBag;
                    Auction.messageBucket.TryGetValue(userIndex, out tempBag);

                    while (tempBag.TryTake(out tempMessage))
                    {
                        if (tempMessage != null)
                        {
                            MessageTransfer tempTransferMessage = new MessageTransfer();
                            // Properties of MessageTransfer are interpreted on client side depending on the Type
                            // of the message
                            tempTransferMessage.MessageDoubleValue = tempMessage.MessageDoubleValue;
                            tempTransferMessage.MessageIntValue = tempMessage.MessageIntValue;
                            tempTransferMessage.MessageIntValue2 = tempMessage.MessageIntValue2;
                            tempTransferMessage.MessageText = tempMessage.MessageText;
                            tempTransferMessage.MessageText2 = tempMessage.MessageText2;

                            switch (tempMessage.Type)
                            {
                                case MessageType.NewBid:
                                    tempTransferMessage.MessageType = 0;
                                    break;
                                case MessageType.AuctionEndStart:
                                    tempTransferMessage.MessageType = 1;
                                    break;
                                case MessageType.EndOfAuction:
                                    tempTransferMessage.MessageType = 2;
                                    break;
                            }

                            message.Add(tempTransferMessage);
                        }

                        else
                        {
                            return "NoMessage";
                        }
                    }                    
                }
                else return "NoMessage";
            }
            catch (Exception)
            {
                return "Fehler bei der Verarbeitung der Messages im Server";
            }
            return "OK";
        }
        #endregion
    }
}

