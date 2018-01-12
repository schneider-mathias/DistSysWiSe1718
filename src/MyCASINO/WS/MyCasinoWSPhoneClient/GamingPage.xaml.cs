using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace MyCasinoWSPhoneClient
{
    public partial class GamingPage : PhoneApplicationPage
    {
        private MyCasinoWSServer.MyCasinoWSServerSoapClient myCasinoSvcGamingPage;
        public MyCasinoWSServer.MyCasinoWSServerSoapClient MyCasinoSvcGamingPage
        {
            get { return myCasinoSvcGamingPage; }
            set { myCasinoSvcGamingPage = value; }
        }

        
            public int _sessionIdGaming;
            public int _type;

        private int sessionId;
        private int userType;
        private string username;

        public GamingPage()
        {
            //init
            //MyCasinoWSServer.MyCasinoWSServerSoapClient _MyCasinoSvc,string _username, int _sessionId, int _userType
            //MyCasinoSvc = _MyCasinoSvc;
            //username = _username;
            //sessionId = _sessionId;
            //userType = _userType;
            InitializeComponent();
        }

        private void BtnPlay_Click(object sender, RoutedEventArgs e)
        {
            tblLoggedIn.Text = sessionId.ToString();
        }
    }
}