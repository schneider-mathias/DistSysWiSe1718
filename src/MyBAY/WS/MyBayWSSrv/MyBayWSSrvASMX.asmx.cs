﻿/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Implementierung des Webservices                         */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     12.01.2018                                              */
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
    /// Zusammenfassungsbeschreibung für MyBayWSSrvASMX
    /// </summary>
    [WebService(Namespace = "http://MyBayServer.org/")]
    [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
    [System.ComponentModel.ToolboxItem(false)]
    public class MyBayWSSrvASMX : System.Web.Services.WebService
    {
        private List<Auction> listAuctions;

        #region C'Tors
        public MyBayWSSrvASMX()
        {
            AuthService.initializeAuthService();
            this.listAuctions = new List<Auction>();
        }
        #endregion

        #region Methods
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

            sessionID = 0;
            return ReturnMessage;
        }

        [WebMethod]
        public String logout(UInt32 sessionID)
        {
            return AuthenticationService.AuthService.Logout(sessionID);
        }

        [WebMethod]
        public String offer(UInt32 sessionID, String artName, Double startBid, out UInt32 auctionNumber)
        {
            auctionNumber = 0;
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            UInt32 auctioneerIndex = AuthenticationService.AuthService.getIndexBySessionID(sessionID);

            Auction newAuction = new Auction(artName, startBid, auctioneerIndex);
            auctionNumber = newAuction.AuctionNumber;

            this.listAuctions.Add(newAuction);

            return "OK";
        }

        [WebMethod]
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
                auctionTemp.addToInterested(AuthenticationService.AuthService.getIndexBySessionID(sessionID));
            }
            return "OK";
        }

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
                    lock (this.listAuctions)
                    {
                        personalizedAuctionList = this.listAuctions.ConvertAll(item => new Auction(item));
                    }
                    break;
                default:
                    personalizedAuctionList = new List<Auction>();
                    break;
            }

            if (!String.IsNullOrEmpty(artName))
            {
                personalizedAuctionList = personalizedAuctionList.FindAll(item => item.ArtName.StartsWith(artName));

                if (personalizedAuctionList.Count < 1)
                {
                    return "Keine Auktionen mit den angegebenen Artikelnamen gefunden";
                }
            }

            if (personalizedAuctionList.Count < 1)
            {
                return "Keine Auktionen mit den angegebenen Parametern gefunden";
            }

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

        [WebMethod]
        public String bid(UInt32 sessionID, UInt32 auctionNumber, Double bidVal)
        {
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

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

            return auctionTemp.addBid(AuthenticationService.AuthService.getIndexBySessionID(sessionID), bidVal);
        }

        [WebMethod]
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

        [WebMethod]
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

                return tempAuction.endAuction();
            }
        }

        [WebMethod]
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

                    tempBag.TryTake(out tempMessage);
                    if (tempMessage != null)
                    {
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
                    if (tempBag.Count > 0) messageAvailable = true;
                }
                else return "NoMessage";

            }
            catch (Exception e)
            {

                return "Fehler bei der Verarbeitung der Messages im Server";
            }
            return "OK";
        }
        #endregion
    }
}

