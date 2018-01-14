using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
        private MyBayWSSrv.MyBayWSSrvASMXSoapClient _remoteSrvMyBay;
        public MyBayWSSrv.MyBayWSSrvASMXSoapClient RemoteSrvMyBay
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
