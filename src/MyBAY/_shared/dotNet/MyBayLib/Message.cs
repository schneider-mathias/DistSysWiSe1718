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
        EndOfAuction
    };

    public class Message
    {
        #region Properties
        public MessageType Type { get; private set; }

        // Depending on MessageType, Client will interpret
        public String MessageText { get; private set; }

        public String MessageText2 { get; private set; }

        public Double MessageDoubleValue { get; private set; }

        public UInt32 MessageIntValue { get; private set; }

        public UInt32 MessageIntValue2 { get; private set; }
        #endregion

        #region C'Tors
        public Message(MessageType type, String text, String text2, Double doublevalue, UInt32 intvalue, UInt32 intvalue2)
        {
            this.Type = type;
            this.MessageText = text;
            this.MessageText2 = text2;
            this.MessageDoubleValue = doublevalue;
            this.MessageIntValue = intvalue;
            this.MessageIntValue2 = intvalue2;
        }
        #endregion
    }

    public class MessageTransfer
    {
        // Depending on MessageType, Client will interprete
        public String MessageText { get; set; }

        public String MessageText2 { get; set; }

        public Double MessageDoubleValue { get; set; }

        public UInt32 MessageIntValue { get; set; }

        public UInt32 MessageIntValue2 { get; set; }
    }
}
