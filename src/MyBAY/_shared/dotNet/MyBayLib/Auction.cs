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
using System.Collections.Concurrent;
using System.Globalization;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization; 

namespace MyBayLib
{
    [Serializable()]
    public class Auction
    {
        public static ConcurrentDictionary<UInt32, ConcurrentBag<Message>> messageBucket = new ConcurrentDictionary<UInt32, ConcurrentBag<Message>>();

        // Will increase for each new auction
        private static UInt32 auctionNumberCount = 0;
        private List<Bid> bidList = new List<Bid>();    
        private UInt16 auctionEndCounter = 1;

        [NonSerialized]
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

        private UInt32 _countBids;
        public UInt32 CountBids
        {
            get
            {
                return this._countBids;
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
            this._bidderInterested = new List<UInt32>();

            this._countBids = 0;

            // Auctioneer automatically interested in auction
            this._bidderInterested.Add(auctioneer);

            auctionEndTimer = new System.Timers.Timer();
            auctionEndTimer.Elapsed += new ElapsedEventHandler(OnTimedEvent);
            auctionEndTimer.Interval = 3000;
        }

        // CopyConstructor
        public Auction(Auction oldAuction)
        {
            _highestBid = oldAuction._highestBid;
            _artName = oldAuction._artName;
            _auctionNumber = oldAuction._auctionNumber;
            _auctionState = oldAuction._auctionState;
            _bidderInterested = new List<UInt32>(oldAuction._bidderInterested);
            _auctioneer = oldAuction._auctioneer;
            _countBids = oldAuction._countBids;
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
                Bid newBid = new Bid((UInt32)(bidList.Count + 1), bidderIndex, bidValue);

                if (newBid.BidValue > this.HighestBid.BidValue)
                {
                    this.bidList.Add(newBid);
                    this._highestBid = newBid;
                    this._countBids = newBid.BidNumber;

                    lock (this._bidderInterested)
                    {
                        if (!this._bidderInterested.Contains(bidderIndex)) this._bidderInterested.Add(bidderIndex);
                    }
                }
                else
                {
                    return "Gebot ist zu niedrig, bieten Sie mehr als: " + String.Format(new CultureInfo("en-US"), "{0:C}", this._highestBid.BidValue);
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


            foreach (UInt32 bidder in biddersInterested)
            {
                // Different message for auctioneer
                if (bidder == this._auctioneer) continue;

                Message newMessage = new Message(MessageType.NewBid, this.ArtName, String.Empty, bidValue, 0, 0);
                AddMessageToBucket(bidder, newMessage);
            }

            // For the auctioneer with Biddername
            String biddername = AuthenticationService.AuthService.getNameByIndex(bidderIndex);
            AddMessageToBucket(this.Auctioneer, new Message(MessageType.NewBid, biddername, this.ArtName, bidValue, 0, 0));
            return true;
        }

        public string endAuction()
        {
            if (this._auctionState != 0)
            {
                return "Auction wird oder ist bereits beendet";
            }

            this._auctionState = 1; // AuctionState 1 -> Auction is preparing to be finished


            // enable timer for auction end messages
            this.auctionEndTimer = new System.Timers.Timer();
            this.auctionEndTimer.Elapsed += new ElapsedEventHandler(OnTimedEvent);
            this.auctionEndTimer.Interval = 4000;
            this.auctionEndTimer.Enabled = true;

            return "OK";
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

                String buyer = AuthenticationService.AuthService.getNameByIndex(HighestBid.BidderIndex);

                foreach (UInt32 bidder in biddersInterested)
                {
                    // Different message for auctioneer
                    if (bidder == this._auctioneer) continue;

                    Message newMessage = new Message(MessageType.EndOfAuction, buyer, this.ArtName, HighestBid.BidValue, 2, 0);
                    AddMessageToBucket(bidder, newMessage);
                }

                // For the auctioneer
                AddMessageToBucket(this.Auctioneer, new Message(MessageType.EndOfAuction, buyer, this.ArtName, HighestBid.BidValue, 2, 0));
            }
            else
            {
                foreach (UInt32 bidder in biddersInterested)
                {
                    // Different message for auctioneer
                    if (bidder == this._auctioneer) continue;

                    Message newMessage = new Message(MessageType.AuctionEndStart, this.ArtName, String.Empty, 0.0, auctionEndCounter, this.AuctionNumber);
                    AddMessageToBucket(bidder, newMessage);

                }

                // For the auctioneer
                AddMessageToBucket(this.Auctioneer, new Message(MessageType.AuctionEndStart, this.ArtName, String.Empty, 0.0, auctionEndCounter, this.AuctionNumber));

                auctionEndCounter++;
            }
        }

        private bool AddMessageToBucket(UInt32 receipientIndex, Message message)
        {
            if (!messageBucket.ContainsKey(receipientIndex))
            {
                messageBucket.TryAdd(receipientIndex, new ConcurrentBag<Message>());
            }

            try
            {
                ConcurrentBag<Message> tempBag;
                messageBucket.TryGetValue(receipientIndex, out tempBag);
                tempBag.Add(message);
            }
            catch (Exception)
            {
                return false;
            }

            return true;
        }

        public static bool SaveAuctionsPersistent(List<Auction> auctionList)
        {
            try
            {
                string tempPath = Path.GetPathRoot(Environment.SystemDirectory);

                tempPath += "_MyBayData\\persistentServerData.txt";
                FileStream stream;
                stream = new FileStream(tempPath, FileMode.Create);
                BinaryFormatter formatter = new BinaryFormatter();
              

                lock (auctionList)
                {
                    formatter.Serialize(stream, auctionList);
                }
                stream.Close(); 
            }
            catch (Exception ex)
            {
                string excepttext = ex.ToString();
                return false;
            }
            return true;
        }

        public static List<Auction> GetPersistentAuctions()
        {
            List<Auction> listAuctions = new List<Auction>();
            try
            {

                string tempPath = Path.GetPathRoot(Environment.SystemDirectory);

                tempPath += "_MyBayData\\persistentServerData.txt";
                FileStream stream;
                stream = new FileStream(tempPath, FileMode.Open);
                BinaryFormatter formatter = new BinaryFormatter();
                listAuctions = (List<Auction>)formatter.Deserialize(stream);
                stream.Close();

                foreach (Auction auct in listAuctions)
                {
                    if (auct._auctionState == 1) auct._auctionState = 2;
                }

            }
            catch (Exception ex)
            {
                string excepttext = ex.ToString();
                return null;
            }
            return listAuctions;
        }

        public static bool setCountAuctions(UInt32 count)
        {
            Auction.auctionNumberCount = count;
            return true;
        }
        #endregion
    }

    public class AuctionTransfer
    {
        public UInt32 AuctNumber { get; set; }

        public String ArtName { get; set; }

        public Double HighestBid { get; set; }

        public UInt32 AuctionState { get; set; }

        public UInt32 CountBids { get; set; }
    }
}
