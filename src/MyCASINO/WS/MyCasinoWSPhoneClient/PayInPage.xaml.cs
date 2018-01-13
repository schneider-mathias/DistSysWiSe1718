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
using System.Windows.Input;

namespace MyCasinoWSPhoneClient
{
    public partial class PayInPage : PhoneApplicationPage
    {
        private TransportData myCasinoSvcPayIn = new TransportData();
        public TransportData MyCasinoSvcPayIn
        {
            get { return myCasinoSvcPayIn; }
            set { myCasinoSvcPayIn = value; }
        }
        public PayInPage()
        {
            InitializeComponent();
            cbxPayInUsername.Items.Add("Martin");
            cbxPayInUsername.Items.Add("Manuel");
            cbxPayInUsername.Items.Add("Johannes");
            cbxPayInUsername.Items.Add("Mathias");
        }

        #region Page buttons
        private void BtnPlay_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<GamingPage>(
                    cp => { cp.MyCasinoSvcGaming = myCasinoSvcPayIn; },
                    //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                    cp => { });
        }
        private void BtnHistory_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<HistoryPage>(
                     cp => { cp.MyCasinoSvcHistory = myCasinoSvcPayIn; },
                     //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                     cp => { });
        }
        private void BtnPayIn_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<PayInPage>(
                    cp => { cp.MyCasinoSvcPayIn = myCasinoSvcPayIn; },
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
                //    cp => { cp.MyCasinoSvcLogin = myCasinoSvcPayIn; },
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

        #region Methods

        private void btnPayIn_Click(object sender, RoutedEventArgs e)
        {
            string errMsg = null;
            double amount;
            double.TryParse(tbxPayInAmount.Text, out amount);

            //add event method
            myCasinoSvcPayIn.MyCasinoSvc.depositCompleted += MyCasinoSvc_depositCompleted;
            //call servicefunction
            myCasinoSvcPayIn.MyCasinoSvc.depositAsync(MyCasinoSvcPayIn.SessionId, cbxPayInUsername.SelectedItem.ToString(), amount);

            //_RemSrvMyCasino.deposit(SessionId, cbxPayInUsername.Text, amount, out errMsg);
          
        }

        private void MyCasinoSvc_depositCompleted(object sender, MyCasinoWSServer.depositCompletedEventArgs e)
        {
            //error handling
            string errMsg = null;
            errMsg = e.errMsg;
            if (errMsg == "USER_NOT_LOGGED_IN")
            {
                MessageBox.Show("User nicht eingeloggt");
            }
            else if (errMsg == "PAYIN_TOO_HIGH")
            {
                MessageBox.Show("Soviel kann nicht auf einmal Einbezahlt werden");
            }
            else if (errMsg == "MAXIMUM_BALANCE_REACHED")
            {
                MessageBox.Show("Account Geld maximum erreicht!");
            }
            else if (errMsg == "INVALID_SESSION_ID")
            {
                MessageBox.Show("Ungültige ID!");
            }

            //set specific userinterface options
            tblPayInSuccessful.Visibility = Visibility.Visible;
            btnPayIn.IsEnabled = false;
            //release event method
            myCasinoSvcPayIn.MyCasinoSvc.depositCompleted -= MyCasinoSvc_depositCompleted;
        }

        private void tbxPayInAmount_KeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = !(e.Key >= Key.D0 && e.Key <= Key.D9 ||
               e.Key >= Key.NumPad0 && e.Key <= Key.NumPad9 ||
               e.Key == Key.Back || e.Key == Key.Decimal
               );

            tblPayInSuccessful.Visibility = Visibility.Collapsed;
            btnPayIn.IsEnabled = true;
            //Only one period is allowed
            if (e.Key == Key.Decimal)
            {
                if (((TextBox)sender).Text.ToString().Contains('.'))
                {
                    e.Handled = true;
                    return;
                }
            }

            //Set money format
            if (((TextBox)sender).Text.ToString().Contains('.'))
            {
                string[] tmp = ((TextBox)sender).Text.ToString().Split('.');
                if (tmp[1].Length >= 2)
                {
                    e.Handled = true;
                    return;
                }

            }
        }

        private void cbxPayInUsername_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            tblPayInSuccessful.Visibility = Visibility.Collapsed;
            btnPayIn.IsEnabled = true;
        }
        #endregion

    }
}