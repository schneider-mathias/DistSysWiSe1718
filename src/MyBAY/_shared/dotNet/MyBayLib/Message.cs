/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Messages des MyBay WCF-Servers                          */
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
        #region Properties
        public MessageType Type { get; private set; }

        public UInt32 recipientIndex { get; private set; }

        // Depending on MessageType, Client will interpret
        public String MessageText { get; private set; }

        public Double MessageDoubleValue { get; private set; }

        public UInt32 MessageIntValue { get; private set; }
        #endregion

        #region C'Tors
        public Message(MessageType type, UInt32 recipientIndex, String text, Double doublevalue, UInt32 intvalue)
        {
            this.Type = type;
            this.recipientIndex = recipientIndex;
            this.MessageText = text;
            this.MessageDoubleValue = doublevalue;
            this.MessageIntValue = intvalue;
        }
        #endregion
    }

    public class MessageTransfer
    {
        public UInt32 Type { get; set; }

        // Depending on MessageType, Client will interprete
        public String MessageText { get; set; }

        public Double MessageDoubleValue { get; set; }

        public UInt32 MessageIntValue { get; set; }
    }
}
