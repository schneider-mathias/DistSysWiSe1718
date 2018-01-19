/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Messages of the MyBay .net-Servers                      */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     03.01.2018                                              */
/*                                                                       */
/*************************************************************************/

using System;

namespace MyBayLib
{
    /// <summary>
    /// Each message can either be a message informing the client about a new bid,
    /// a message informing the client about the approaching closing of the auction
    /// and a message about the auction being closed
    /// </summary>
    public enum MessageType
    {
        NewBid = 0,
        AuctionEndStart,
        EndOfAuction
    };

    /// <summary>
    /// Definition of class Message which is used internal to store messages on the server.
    /// The Properties of a message are variable and contains 2 strings, one double and 2 unsigned integer variables
    /// which are filled and used depending on what kind of message
    /// </summary>
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
        /// <summary>
        /// Constructor of class Message
        /// </summary>
        /// <param name="type"></param>
        /// <param name="text"></param>
        /// <param name="text2"></param>
        /// <param name="doublevalue"></param>
        /// <param name="intvalue"></param>
        /// <param name="intvalue2"></param>
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

    /// <summary>
    /// Class for transferring Messages from the server to the client. 
    /// The difference to the class Message is, 
    /// that the enum MessageType is replaced by an unsigned integer 
    /// (interface defined also because of .net - COM - communication
    /// </summary>
    public class MessageTransfer
    {
        // Depending on MessageType, Client will interprete
        public String MessageText { get; set; }

        public String MessageText2 { get; set; }

        public Double MessageDoubleValue { get; set; }

        public UInt32 MessageIntValue { get; set; }

        public UInt32 MessageIntValue2 { get; set; }

        // This variable is just needed for the WebServer, the WCF Server doesnt use it
        public UInt32 MessageType { get; set; }
    }
}
