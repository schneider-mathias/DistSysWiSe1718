using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyBayLib
{
    public class Auction
    {
        public UInt32 ArtNumber { get; set; }

        public String ArtName { get; set; }

        public Double HighestBid { get; set; }

        public UInt32 AuctionState { get; set; }
    }
}
