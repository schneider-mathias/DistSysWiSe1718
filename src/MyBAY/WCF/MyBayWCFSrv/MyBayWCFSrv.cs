﻿/*************************************************************************/
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
using System.Collections.Concurrent;

namespace MyBayWCFSrv
{
    class MyBayWCFSrv : MyBayWCFLibrary.IMyBay
    {
        private List<Auction> listAuctions;
        //private ConcurrentBag<Auction> bagtest;

        public MyBayWCFSrv()
        {
            AuthenticationService.AuthService.initializeAuthService();
            this.listAuctions = new List<Auction>();
        }

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

        public String logout(UInt32 sessionID)
        {
            return AuthenticationService.AuthService.Logout(sessionID);
        }

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

        public String getAuctions(UInt32 sessionID, UInt32 flags, String artName, out UInt32 countAuctions, out List<AuctionTransfer> auctions)
        {
            countAuctions = 0;
            auctions = new List<AuctionTransfer>();
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";
            UInt32 userIndex = AuthenticationService.AuthService.getIndexBySessionID(sessionID);

            List<Auction> personalizedAuctionList;
            personalizedAuctionList = this.listAuctions.Where(m => m.BidderInterested.Contains(userIndex)).ToList<Auction>();           


            return "OK";
        }

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

            if (auctionTemp.HighestBid.BidValue < bidVal)
            {
                return auctionTemp.addBid(AuthenticationService.AuthService.getIndexBySessionID(sessionID), bidVal);
            }
            else
            {
                return "Das Angegebene Gebot ist zu niedrig, bieten Sie mehr als: " + auctionTemp.HighestBid.ToString();
            }
        }

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
            return "OK";           
        }

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
                if(tempAuction.endAuction()) return "OK";
            }
            return "OK";
        }

        public String getMessage(UInt32 sessionID, out Boolean messageAvailable, out UInt32 messageType, out MessageTransfer message)
        {
            messageType = 0;
            messageAvailable = false;
            message = default(MessageTransfer);
            if (!AuthenticationService.AuthService.isLoggedIn(sessionID)) return "Die angegebene SessionID ist nicht registriert, loggen Sie sich erneut ein";

            Message tempMessage;

            try
            {
                // Translate SessionID to UserIndex
                UInt32 userIndex = AuthenticationService.AuthService.getIndexBySessionID(sessionID);

                lock (Auction.messageBucket)
                {
                    tempMessage = Auction.messageBucket.FirstOrDefault(m => m.MessageIntValue == userIndex);
                    if (!tempMessage.Equals(default(Message)))
                    {
                        Auction.messageBucket.Remove(tempMessage);
                    }
                    else
                    {
                        return "NoMessage";
                    }

                    Message tempMessage2 = Auction.messageBucket.FirstOrDefault(m => m.MessageIntValue == userIndex);
                    if (!tempMessage2.Equals(default(Message)))
                    {
                        messageAvailable = true;
                    }
                }
            }        
            catch (Exception)
            {
                return "Fehler bei der Verarbeitung der Messages im Server";
            }

            message.MessageDoubleValue = tempMessage.MessageDoubleValue;
            message.MessageIntValue = tempMessage.MessageIntValue;
            message.MessageText = tempMessage.MessageText;

            switch (tempMessage.Type)
            {
                case MessageType.NewBid:
                    message.Type = 0;
                    break;
                case MessageType.AuctionEndStart:
                    message.Type = 1;
                    break;
                case MessageType.EndOfAuction:
                    message.Type = 2;
                    break;
                case MessageType.InfoMessage:
                    message.Type = 3;
                    break;
            }         
            return "OK";            
        }
    }
}
