/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Dataobject for exchanging data between the different    */
/*               phone pages                                             */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     19.01.2018                                              */
/*                                                                       */
/*************************************************************************/

using MyBayWSPhoneCln.MyBayWSSrvASMXSoapService;
using System;

namespace MyBayWSPhoneCln
{
    /// <summary>
    /// This struct is needed for the communication between the different Phone pages
    /// </summary>
    public class DataObject
    {
        /// <summary>
        /// The current session ID of the client
        /// </summary>
        private UInt32 _sessionID;
        public UInt32 SessionID
        {
            get { return _sessionID; }
            set { _sessionID = value; }
        }

        /// <summary>
        /// This object represents the instance of the MyBay WebService
        /// </summary>
        private MyBayWSSrvASMXSoapClient _remoteSrvMyBay;
        public MyBayWSSrvASMXSoapClient RemoteSrvMyBay
        {
            get { return _remoteSrvMyBay; }
            set { _remoteSrvMyBay = value; }
        }

        /// <summary>
        /// Conctructor of the DataObject
        /// </summary>
        public DataObject()
        {
            this._sessionID = 0;
        }
    }
}
