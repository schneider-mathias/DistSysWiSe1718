using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyBayLib
{
    public enum MessageType
    {
        NewBid = 0,
        AuctionEndStart,
        EndOfAuction,
        InfoMessage
    };

    public class Message
    {
        public MessageType Type { get; set; }

        // Depending on MessageType, Client will interprete
        public String MessageText { get; set; }

        public Double MessageDoubleValue { get; set; }

        public UInt32 MessageIntValue { get; set; }
    }
}
