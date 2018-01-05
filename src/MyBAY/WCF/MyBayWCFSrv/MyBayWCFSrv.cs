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
        private List<Auction> listAuctions;

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

            lock (this.listAuctions)
            {
                this.listAuctions.Add(newAuction);
            }

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

            return "OK";
        }

        public String bid(UInt32 sessionID, UInt32 auctionNumber, Double bidVal)
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

                if (auctionTemp.HighestBid.BidValue < bidVal)
                {
                    return auctionTemp.addBid(AuthenticationService.AuthService.getIndexBySessionID(sessionID), bidVal);
                }
                else
                {
                    return "Das Angegebene Gebot ist zu niedrig, bieten Sie mehr als: " + auctionTemp.HighestBid.ToString();
                }
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
                    tempAuction = this.listAuctions.Find(x => x.AuctionNumber == auctionNumber);
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
            #region DummyData
            messageType = 0;
            messageAvailable = false;
            message = new MessageTransfer();
            return "";
            #endregion
        }
    }
}
