/*************************************************************************/
/*                                                                       */
/*    Inhalt:   Implementation of the main functionality of the          */
/*              MyBay WCF-Server                                         */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     09.01.2018                                              */
/*                                                                       */
/*************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using MyBayWCFSrv;
using MyBayLib;
using System.Collections.Concurrent;

namespace MyBayWCFSrv
{
    /// <summary>
    /// Class MyBayWCFSrv is implementing the IMyBay interface
    /// </summary>
    class MyBayWCFSrv : MyBayWCFLibrary.IMyBay
    {
        /// <summary>
        /// This is a private list of auctions which is created during the start of the WCF-server in it's constructor
        /// The list is either created new or the list is coming from the persistent data on the hard disk drive, if the server 
        /// was already executed before on the same machine
        /// </summary>
        private List<Auction> listAuctions;

        #region C'Tors
        /// <summary>
        /// Constructor of the MyBayWCFSrv class
        /// </summary>
        public MyBayWCFSrv()
        {
            // Initialization of the authentification service
            AuthenticationService.AuthService.initializeAuthService();

            this.listAuctions = Auction.GetPersistentAuctions();
            if (this.listAuctions != null)
            {
                Auction.setCountAuctions((UInt32)this.listAuctions.Count);
            }
            else this.listAuctions = new List<Auction>();
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
        public String offer(UInt32 sessionID, String artName, Double startBid, out UInt32 auctionNumber)
        {
            auctionNumber = 0;
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            // Translating current sessionID of client to the index of the user which is logged in
            UInt32 auctioneerIndex = AuthenticationService.AuthService.getIndexBySessionID(sessionID);

            Auction newAuction = new Auction(artName, startBid, auctioneerIndex);
            auctionNumber = newAuction.AuctionNumber;

            lock(this.listAuctions)
            {
                this.listAuctions.Add(newAuction);
            }
            Auction.SaveAuctionsPersistent(this.listAuctions);

            return "OK";
        }

        /// <summary>
        /// Method to add a user on the list of interested users of an auction so that
        /// the user will be informed about updates on this auction
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="auctionNumber"></param>
        /// <returns></returns>
        public String interested(UInt32 sessionID, UInt32 auctionNumber)
        {
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            lock (this.listAuctions)
            {
                Auction auctionTemp;
                try
                {
                    auctionTemp = this.listAuctions.Find(x => x.AuctionNumber == auctionNumber);
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
            Auction.SaveAuctionsPersistent(this.listAuctions);
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
        public String getAuctions(UInt32 sessionID, UInt32 flags, String artName, out UInt32 countAuctions, out List<AuctionTransfer> auctions)
        {
            countAuctions = 0;
            auctions = new List<AuctionTransfer>();
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";
            UInt32 userIndex = AuthenticationService.AuthService.getIndexBySessionID(sessionID);

            List<Auction> personalizedAuctionList;
            
            // flags can be 0: normal getAuctions 1: get all Auctions 2: get all Auctions including the one which are ended
            switch(flags)
            {
                case 0: // Get all auctions where User is Interested in and which are not closed
                    lock (this.listAuctions)
                    {
                        // ConvertAll Method is being used, because Method creates a deepcopy of the Object with creating new Objects -> that listAuctions is not blocked too long
                        personalizedAuctionList = this.listAuctions.FindAll(w => (w.BidderInterested.Contains(userIndex) && w.AuctionState != 2)).ConvertAll(item => new Auction(item));
                    }
                    break;
                case 1: // Get all auctions which are open
                    lock (this.listAuctions)
                    {
                        personalizedAuctionList = this.listAuctions.FindAll(w => w.AuctionState != 2).ConvertAll(item => new Auction(item));
                    }
                    break;
                case 2: // Get all auctions
                    lock(this.listAuctions)
                    {
                        personalizedAuctionList = this.listAuctions.ConvertAll(item => new Auction(item));
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
            countAuctions = (UInt32) auctions.Count;
            
            return "OK";
        }

        /// <summary>
        /// Method for bidding on an article
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="auctionNumber"></param>
        /// <param name="bidVal"></param>
        /// <returns></returns>
        public String bid(UInt32 sessionID, UInt32 auctionNumber, Double bidVal)
        {
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            // Find the auction with the passed auctionNumber
            Auction auctionTemp;
            lock (this.listAuctions)
            {
                try
                {
                    auctionTemp = this.listAuctions.Find(x => x.AuctionNumber == auctionNumber);
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
                Auction.SaveAuctionsPersistent(this.listAuctions);
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
        public String details(UInt32 sessionID, UInt32 auctionNumber, out UInt32 countBids, out List<BidTransfer> allBids)
        {
            countBids = 0;
            allBids = new List<BidTransfer>();
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            List<Bid> copyBids;
            lock (this.listAuctions)
            {
                try
                {
                    Auction tempAuction = this.listAuctions.Find(x => x.AuctionNumber == auctionNumber);
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
            countBids = (UInt32) copyBids.Count;

            return "OK";           
        }

        /// <summary>
        /// Method for ending an auction
        /// An auction can just be ended if the calling client (user) is the auctioneer of the auction with the passed auctionNumber
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="auctionNumber"></param>
        /// <returns></returns>
        public String endauction(UInt32 sessionID, UInt32 auctionNumber)
        {
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            lock (this.listAuctions)
            {
                Auction tempAuction;
                try
                {
                    tempAuction = this.listAuctions.FirstOrDefault(x => x.AuctionNumber == auctionNumber);
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

                if(retValue == "OK") Auction.SaveAuctionsPersistent(this.listAuctions);

                return retValue;
            }
        }

        /// <summary>
        /// Method checks, if there are any messages for the calling client (user) in the 
        /// static list of the Auction class.
        /// If there is a message, it will be copied to a MessageTransfer object and passed back to the client
        /// Is there another message for the calling client, the out parameter messageAvailable is set to true so the client
        /// knows it can poll again directly after to get the next message
        /// </summary>
        /// <param name="sessionID"></param>
        /// <param name="messageAvailable"></param>
        /// <param name="messageType"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        public String getMessage(UInt32 sessionID, out Boolean messageAvailable, out UInt32 messageType, out MessageTransfer message)
        {
            messageType = 0;
            messageAvailable = false;
            message = new MessageTransfer();

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

                    // Out parameter is null, if there is no element inside the ConcurrentBag
                    tempBag.TryTake(out tempMessage);
                    if (tempMessage != null)
                    {
                        // Properties of MessageTransfer are interpreted on client side depending on the Type
                        // of the message
                        message.MessageDoubleValue = tempMessage.MessageDoubleValue;
                        message.MessageIntValue = tempMessage.MessageIntValue;
                        message.MessageIntValue2 = tempMessage.MessageIntValue2;
                        message.MessageText = tempMessage.MessageText;
                        message.MessageText2 = tempMessage.MessageText2;

                        switch (tempMessage.Type)
                        {
                            case MessageType.NewBid:
                                messageType = 0;
                                break;
                            case MessageType.AuctionEndStart:
                                messageType = 1;
                                break;
                            case MessageType.EndOfAuction:
                                messageType = 2;
                                break;
                        }
                    }
                    else
                    {
                        return "NoMessage";
                    }
                    // Set messageAvailable to true if there is another message for the same client (user)
                    if(tempBag.Count > 0) messageAvailable = true;                   
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
