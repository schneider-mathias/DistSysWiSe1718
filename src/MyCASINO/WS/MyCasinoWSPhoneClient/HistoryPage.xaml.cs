using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using DialogExtensions;

namespace MyCasinoWSPhoneClient
{
    public partial class HistoryPage : PhoneApplicationPage
    {
        private TransportData myCasinoSvcHistory = new TransportData();
        public TransportData MyCasinoSvcHistory
        {
            get { return myCasinoSvcHistory; }
            set { myCasinoSvcHistory = value; }
        }
        public HistoryPage()
        {
            InitializeComponent();
        }

        #region Page buttons
        private void BtnPlay_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<GamingPage>(
                    cp => { cp.MyCasinoSvcGaming = myCasinoSvcHistory; },
                    //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                    cp => { });
        }
        private void BtnHistory_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<HistoryPage>(
                     cp => { cp.MyCasinoSvcHistory = myCasinoSvcHistory; },
                     //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                     cp => { });
        }
        private void BtnPayIn_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<PayInPage>(
                    cp => { cp.MyCasinoSvcPayIn = myCasinoSvcHistory; },
                    //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                    cp => { });
        }
        private void btnLogout_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                //if (_RemSrvMyCasino.logout(SessionId, out errMsg))
                //{
                //    // Close Client Connection
                //    if (_RemSrvMyCasino != null)
                //    {
                //        ((IClientChannel)_RemSrvMyCasino).Close();
                //        ((IDisposable)_RemSrvMyCasino).Dispose();
                //        _RemSrvMyCasino = null;
                //    }
                //    //TODO: go to login page 
                //    //System.Windows.Application.Current.Shutdown();
                //this.ShowNewDialog<LoginPage>(
                //    cp => { cp.MyCasinoSvcLogin = myCasinoSvcHistory; },
                //    //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                //    cp => { });
                //}
                //if (errMsg == "INVALID_SESSION_ID")
                //{
                //    MessageBox.Show("Ungültige ID!");
                //}
            }
            catch (Exception ex)
            {
                MessageBox.Show("Fehler beim Logout: " + ex.ToString());
            }
        }
        #endregion
    }
}