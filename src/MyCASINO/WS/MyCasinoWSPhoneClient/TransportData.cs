using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyCasinoWSPhoneClient
{
    public struct TransportData 
    {
        private MyCasinoWSServer.MyCasinoWSServerSoapClient myCasinoSvc;
        public MyCasinoWSServer.MyCasinoWSServerSoapClient MyCasinoSvc
        {
            get { return myCasinoSvc; }
            set { myCasinoSvc = value; }
        }

        private int sessionId;
        public int SessionId
        {
            get { return sessionId; }
            set { sessionId = value; }
        }

        private int userType;
        public int UserType
        {
            get { return userType; }
            set { userType = value; }
        }

        private string userName;

        public string UserName
        {
            get { return userName; }
            set {
                userName = value;
                }
        }
    }
}
