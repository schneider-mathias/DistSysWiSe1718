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
        private async void btnLogout_Click(object sender, RoutedEventArgs e)
        {
            var result = await myCasinoSvcPayIn.MyCasinoSvc.LogoutAsyncTask(myCasinoSvcPayIn.SessionId);

            if (result.errMsg == "S_OK")
            {
                this.ShowNewDialog<LoginPage>(
                  cp => { cp.MyCasinoSvcLogin = myCasinoSvcPayIn; },
                  //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                  cp => { });
            }
            else if (result.errMsg == "INVALID_SESSION_ID")
            {
                MessageBox.Show("Ungültige ID!");
            }
            else if (result.errMsg != null)
            {
                MessageBox.Show("Fehler beim Logout");
            }

        }
        #endregion

        #region Methods

        private void btnPayIn_Click(object sender, RoutedEventArgs e)
        {
            //check if valid bet format
            int dotcount=0;
            for (int i = 0; i < tbxPayInAmount.Text.Length; i++)
            {
                char test;
                test=tbxPayInAmount.Text.ElementAt(i);
                if ((test == '1') || (test == '2') || (test == '3') || (test == '4') ||
                    (test == '5') || (test == '6') || (test == '7') || (test == '8') ||
                    (test == '9') || (test == '0') || (test == '.'))
                {
                    //only numbers and one dot dot allowed in string
                    if (test == ',') dotcount++;
                    if(dotcount>1)
                    {
                        MessageBox.Show("Gültiges Geldformat eingeben!");
                        return;
                    }
                }
                else
                {
                    MessageBox.Show("Gültiges Geldformat eingeben!");
                    return;
                }
            }

            double amount;
            double.TryParse(tbxPayInAmount.Text, out amount);

            //add event method
            myCasinoSvcPayIn.MyCasinoSvc.depositCompleted += MyCasinoSvc_depositCompleted;
            //call servicefunction
            myCasinoSvcPayIn.MyCasinoSvc.depositAsync(MyCasinoSvcPayIn.SessionId, cbxPayInUsername.SelectedItem.ToString(), amount);
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
            
            tblPayInSuccessful.Visibility = Visibility.Collapsed;
            btnPayIn.IsEnabled = true;

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