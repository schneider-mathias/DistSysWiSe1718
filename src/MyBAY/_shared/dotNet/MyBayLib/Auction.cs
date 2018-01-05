/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Autkionen des MyBay WCF-Servers                         */
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
using System.Timers;
using AuthenticationService;

namespace MyBayLib
{
    public class Auction
    {
        public static List<Message> messageBucket = new List<Message>();

        // Will increase for each new auction
        private static UInt32 auctionNumberCount = 0;
        private List<Bid> bidList;       
        private UInt16 auctionEndCounter = 1;

        private System.Timers.Timer auctionEndTimer;

        #region Properties
        private UInt32 _auctionNumber;

        public UInt32 AuctionNumber
        {
            get {
                return this._auctionNumber;
            }
        }

        private List<UInt32> _bidderInterested;

        public List<UInt32> BidderInterested
        {
            get
            {
                return this._bidderInterested;
            }
        }

        private String _artName;

        public String ArtName
        {
            get
            {
                return this._artName;
            }
        }

        private Bid _highestBid;

        public Bid HighestBid
        {
            get {
                return this._highestBid;
            }
        }

        private UInt32 _auctionState;

        public UInt32 AuctionState
        {
            get
            {
                return this._auctionState;
            }
        }

        private UInt32 _auctioneer;

        public UInt32 Auctioneer
        {
            get {
                return this._auctioneer;
            }
        }
        #endregion

        #region C'tors
        public Auction(String artName, Double startBid, UInt32 auctioneer)
        {
            this._highestBid = new Bid(0,auctioneer,startBid);
            this._artName = artName;

            this._auctionNumber = auctionNumberCount;
            auctionNumberCount++;

            this._auctioneer = auctioneer;
            this._auctionState = 0; // AuctionState 0 -> Auction is running and open

            this.bidList = new List<Bid>();
            this._bidderInterested = new List<uint>();

            // Auctioneer automatically interested in auction
            this._bidderInterested.Add(auctioneer);

            auctionEndTimer = new System.Timers.Timer();
            auctionEndTimer.Elapsed += new ElapsedEventHandler(OnTimedEvent);
            auctionEndTimer.Interval = 3000;
        }
        #endregion

        #region Methods
        public String addBid(UInt32 bidderIndex, Double bidValue)
        {
            if (this._auctionState == 2)
            {
                return "Die Auktion wurde beendet, es sind keine weiteren Gebote möglich";
            }
            lock (this)
            {
                Bid newBid = new Bid((UInt32)bidList.Count + 1, bidderIndex, bidValue);

                if (newBid.BidValue > this.HighestBid.BidValue)
                {
                    this.bidList.Add(newBid);
                    this._highestBid = newBid;
                }
                else
                {
                    return "Das Angegebene Gebot ist zu niedrig, Gebot wird nicht akzeptiert";
                }
                
            }
            informBidders(bidValue, bidderIndex);
            return "OK";
        }

        public bool addToInterested(UInt32 bidderIndex)
        {
            lock (this._bidderInterested)
            {
                this._bidderInterested.Add(bidderIndex);
            }
            return true;
        }

        public List<Bid> getBids()
        {
            lock (this.bidList) return new List<Bid>(this.bidList);
        }

        private bool informBidders(Double bidValue, UInt32 bidderIndex)
        {
            List<UInt32> biddersInterested;
            
            lock (this._bidderInterested)
            {
                biddersInterested = new List<uint>(this._bidderInterested);
            }

            lock (Auction.messageBucket)
            {
                foreach (UInt32 bidder in biddersInterested)
                {
                    // Different message for auctioneer
                    if (bidder == this._auctioneer) continue;

                    Message newMessage = new Message(MessageType.NewBid, bidder, this.ArtName, bidValue, 0);
                    Auction.messageBucket.Add(newMessage);
                }

                // For the auctioneer with Biddername
                String biddername = AuthenticationService.AuthService.getNameByIndex(bidderIndex);
                Auction.messageBucket.Add(new Message(MessageType.NewBid, this.Auctioneer, this.ArtName + " " + biddername, bidValue, 0));
            }
            return true;
        }

        public bool endAuction()
        {
            this._auctionState = 1; // AuctionState 1 -> Auction is preparing to be finished

            this.auctionEndTimer.Enabled = true;

            return true;
        }

        private void OnTimedEvent(object source, ElapsedEventArgs e)
        {
            List<UInt32> biddersInterested;

            lock (this._bidderInterested)
            {
                biddersInterested = new List<uint>(this._bidderInterested);
            }

            if (auctionEndCounter == 4)
            {
                auctionEndTimer.Enabled = false;

                // Auction finished, no more bids accepted
                this._auctionState = 2;
                lock (Auction.messageBucket)
                {
                    String buyer = AuthenticationService.AuthService.getNameByIndex(HighestBid.BidderIndex);

                    foreach (UInt32 bidder in biddersInterested)
                    {
                        Message newMessage = new Message(MessageType.EndOfAuction, bidder, buyer, HighestBid.BidValue, 2);
                        Auction.messageBucket.Add(newMessage);
                    }

                    // For the auctioneer
                    Auction.messageBucket.Add(new Message(MessageType.EndOfAuction, this.Auctioneer, buyer, HighestBid.BidValue, 2));
                }
            }

            lock (Auction.messageBucket)
            {
                foreach (UInt32 bidder in biddersInterested)
                {
                    // Different message for auctioneer
                    if (bidder == this._auctioneer) continue;

                    Message newMessage = new Message(MessageType.AuctionEndStart, bidder, "", 0.0, auctionEndCounter);
                    Auction.messageBucket.Add(newMessage);
                }

                // For the auctioneer
                Auction.messageBucket.Add(new Message(MessageType.AuctionEndStart, this.Auctioneer, "", 0.0, auctionEndCounter));
            }            
            auctionEndCounter++;
        }
        #endregion
    }

    public class AuctionTransfer
    {
        public UInt32 ArtNumber { get; set; }

        public String ArtName { get; set; }

        public Double HighestBid { get; set; }

        public UInt32 AuctionState { get; set; }
    }
}
