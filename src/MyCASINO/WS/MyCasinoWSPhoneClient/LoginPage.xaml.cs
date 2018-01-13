using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using System.ServiceModel;
using System.Windows.Media;
using DialogExtensions;

namespace MyCasinoWSPhoneClient
{
    public partial class LoginPage : PhoneApplicationPage
    {

        /// <summary>
        /// Field to save data between page change
        /// </summary>
        private TransportData myCasinoSvcLogin = new TransportData();
        public TransportData MyCasinoSvcLogin
        {
            get { return myCasinoSvcLogin; }
            set { myCasinoSvcLogin = value; }
        }
                   
        public LoginPage()
        {
            //create new webservice
            myCasinoSvcLogin.MyCasinoSvc = new MyCasinoWSServer.MyCasinoWSServerSoapClient();
            InitializeComponent();
        }

        #region Username and Password standard values

        /// <summary>
        /// Set textbox to empty string when clicking
        /// Change Fontcolors accordingly
        /// </summary>
        /// <param name="sender">Username textbox</param>
        /// <param name="e"></param>
        private void tbxUsername_GotFocus(object sender, RoutedEventArgs e)
        {
            if (((TextBox)sender).Text == "Username")
            {
                tbxUsername.Text = "";
            }

           ((TextBox)sender).Foreground = new SolidColorBrush(Colors.Black);
        }

        /// <summary>
        /// Set textbox to Username when focus is lost and nothing is written in the Username textbox
        /// Change Fontcolors accordingly
        /// </summary>
        /// <param name="sender">Username textbox</param>
        /// <param name="e"></param>
        private void tbxUsername_LostFocus(object sender, RoutedEventArgs e)
        {
            if (((TextBox)sender).Text == "")
            {
                ((TextBox)sender).Foreground = new SolidColorBrush(Colors.LightGray);
                ((TextBox)sender).Text = "Username";
            }
        }

        /// <summary>
        /// Set Passwordbox to the standardvalue when focus is lost and nothing has been added in the password field
        /// Change Fontcolors accordingly
        /// </summary>
        /// <param name="sender">Password box</param>
        /// <param name="e"></param>
        private void pwbPassword_LostFocus(object sender, RoutedEventArgs e)
        {
            if (((PasswordBox)sender).Password == "")
            {
                ((PasswordBox)sender).Foreground = new SolidColorBrush(Colors.LightGray);
                pwbPassword.Password = "****";
            }
        }
        /// <summary>
        /// Passwordbox got focus with tab
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void pwbPassword_GotFocus(object sender, RoutedEventArgs e)
        {
            if (((PasswordBox)sender).Password == "****")
            {
                pwbPassword.Password = "";
            }
            ((PasswordBox)sender).Foreground = new SolidColorBrush(Colors.Black);
        }

        /// <summary>
        /// Ip address to server field got focus, remove standardvalue
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbxIpAddress_GotFocus(object sender, RoutedEventArgs e)
        {
            if (((TextBox)sender).Text == "Localhost")
            {
                tbxIpAddress.Text = "";
            }
            ((TextBox)sender).Foreground = new SolidColorBrush(Colors.Black);
        }
        /// <summary>
        /// Ip address to server field lost focus, set standardvalues for box if nessecery
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbxIpAddress_LostFocus(object sender, RoutedEventArgs e)
        {
            if (((TextBox)sender).Text == "")
            {
                ((TextBox)sender).Foreground = new SolidColorBrush(Colors.LightGray);
                tbxIpAddress.Text = "Localhost";
            }
        }

        #endregion
        /// <summary>
        /// Checks if login is ok
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            //check if ip address is valid
            string ipAddress = tbxIpAddress.Text;
            IPAddress address;

            if (ipAddress == "Localhost")
            {
                //ip is automatically set for localhost
            }
            else if (IPAddress.TryParse(ipAddress, out address))
            {
                switch (address.AddressFamily)
                {
                    case System.Net.Sockets.AddressFamily.InterNetwork:

                        break;

                    default:
                        tblAuthentificationFailed.Text = "Falsches IP-Addressen Format";
                        return;
                }
            }
            else
            {
                tblAuthentificationFailed.Text = "Falsches IP-Addressen Format";
                return;
            }
            try
            {
                //add endpointaddress to webservice
                MyCasinoSvcLogin.MyCasinoSvc.Endpoint.Address = new EndpointAddress("http://" + ipAddress + ":7193/MyCasinoWSServer.asmx");
            }
            catch (Exception ex)
            {
                MessageBox.Show("Fehler beim anlegen der EndpunktAddresse!" + ex);
            }
            //add eventhandler
            //MyCasinoSvcLogin.MyCasinoSvc.loginCompleted += myCasinoSvc_loginCompleted;
            //call login function
            //MyCasinoSvcLogin.MyCasinoSvc.loginAsync(tbxUsername.Text, pwbPassword.Password);

            var result = await myCasinoSvcLogin.MyCasinoSvc.GetLoginAsyncTask(tbxUsername.Text, pwbPassword.Password);
            try
            {
                String errMsg = result.errMsg;

                if (errMsg == "S_OK")
                {
                    myCasinoSvcLogin.SessionId = result.sessionId;
                    myCasinoSvcLogin.UserName = tbxUsername.Text;
                    myCasinoSvcLogin.UserType = result.userType;
                    this.ShowNewDialog<GamingPage>(
                    cp => { cp.MyCasinoSvcGaming = myCasinoSvcLogin; },
                    //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                    cp => { });

                    //MyCasinoSvcLogin.MyCasinoSvc.loginCompleted -= myCasinoSvc_loginCompleted;
                }
                if (errMsg == "WRONG_USERNAME_OR_PASSWORD")
                {
                    tblAuthentificationFailed.Text = "Falscher Benutzername oder Passwort";
                }
                else if (errMsg == "OPERATOR_ALREADY_LOGGED_IN")
                {
                    tblAuthentificationFailed.Text = "Es ist bereits ein Betreiber eingeloggt";
                }
                else if (errMsg == "ALREADY_LOGGED_IN")
                {
                    tblAuthentificationFailed.Text = "User ist bereits angemeldet";
                }
            }
            catch
            {
                MessageBox.Show("Fehler beim Login: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            }



        }
        //private void myCasinoSvc_loginCompleted(object sender, MyCasinoWSServer.loginCompletedEventArgs e)
        //{
            //try
            //{
            //    String errMsg = e.errMsg;

            //    if (errMsg == "S_OK")
            //    {
            //        myCasinoSvcLogin.SessionId = e.sessionId;
            //        myCasinoSvcLogin.UserName = tbxUsername.Text;
            //        myCasinoSvcLogin.UserType = e.userType;
            //        this.ShowNewDialog<GamingPage>(
            //        cp => { cp.MyCasinoSvcGaming = myCasinoSvcLogin; },
            //        //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
            //        cp => { });

            //        MyCasinoSvcLogin.MyCasinoSvc.loginCompleted -= myCasinoSvc_loginCompleted;
            //    }
            //    if (errMsg == "WRONG_USERNAME_OR_PASSWORD")
            //    {
            //        tblAuthentificationFailed.Text = "Falscher Benutzername oder Passwort";
            //    }
            //    else if (errMsg == "OPERATOR_ALREADY_LOGGED_IN")
            //    {
            //        tblAuthentificationFailed.Text = "Es ist bereits ein Betreiber eingeloggt";
            //    }
            //    else if (errMsg == "ALREADY_LOGGED_IN")
            //    {
            //        tblAuthentificationFailed.Text = "User ist bereits angemeldet";
            //    }
            //}
            //catch
            //{
            //    MessageBox.Show("Fehler beim Login: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            //}
        //}
    }
}