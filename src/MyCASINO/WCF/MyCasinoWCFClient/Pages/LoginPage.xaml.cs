﻿#define SINGLETON

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using MyCasinoLib;
using MyCasinoWCFLib;
using System.ServiceModel;
using System.Net;

namespace MyCasinoWCFClient.Pages
{
    /// <summary>
    /// Interaction logic for LoginPage.xaml
    /// </summary>
    public partial class LoginPage : Page
    {
#if SINGLETON
       // private ChannelFactory<INETMyCasino> _remSrvMyCasinoFactory = new ChannelFactory<INETMyCasino>(new BasicHttpBinding());
#else

#endif
        
        //public INETMyCasino _remSrvMyCasino;

        private INETMyCasino _remSrvMyCasinoLogin;

        public INETMyCasino _RemSrvMyCasinoLogin
        {
            get { return _remSrvMyCasinoLogin; }
            set { _remSrvMyCasinoLogin = value; }
        }


        public int _sessionId;
        private MyCasinoUserTypes _userType;
        private string _errMsg;


        public LoginPage(INETMyCasino _RemSrvMyCasino)
        {
            _RemSrvMyCasinoLogin = _RemSrvMyCasino;
            InitializeComponent();
        }

#region Username and Password standard values

        /// <summary>
        /// Set textbox to empty string when clicking
        /// Change Fontcolors accordingly
        /// </summary>
        /// <param name="sender">Username textbox</param>
        /// <param name="e"></param>
        private void TextBox_PreviewMouseDown(object sender, MouseButtonEventArgs e)
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
        /// Set Passwordbox to an empty string when clicking into the password field
        /// Change Fontcolors accordingly
        /// </summary>
        /// <param name="sender">Password box</param>
        /// <param name="e"></param>
        private void PasswordBox_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (((PasswordBox)sender).Password == "****")
            {
                pwbPassword.Password = "";
            }
            ((PasswordBox)sender).Foreground = new SolidColorBrush(Colors.Black);
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

#endregion
        /// <summary>
        /// Checks if login is ok
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            // TODO: Passwortüberprüfung noch einbauen
            IPAddress ip;
           // string srvAddress = "http://localhost:1200/MyCasinoWCF";

            try
            {
                //_RemSrvMyCasino = _remSrvMyCasinoFactory.CreateChannel(new EndpointAddress(srvAddress));
                
                if (_RemSrvMyCasinoLogin.login(tbxUsername.Text, pwbPassword.Password, out _sessionId, out _userType, out _errMsg))
                {
                    this.NavigationService.Navigate(new GamingPage(_RemSrvMyCasinoLogin, tbxUsername.Text, _sessionId, _userType));
                    return;
                } 
                if (_errMsg == "WRONG_USERNAME_OR_PASSWORD")
                {
                    tblAuthentificationFailed.Text = "Falscher Benutzername oder Passwort";
                }
                else if (_errMsg == "OPERATOR_ALREADY_LOGGED_IN")
                {
                    tblAuthentificationFailed.Text = "Es ist bereits ein Betreiber eingeloggt";
                }
                else if (_errMsg == "ALREADY_LOGGED_IN")
                {
                    tblAuthentificationFailed.Text = "User ist bereits angemeldet";
                }
            }
            catch (EndpointNotFoundException ex)
            {
                MessageBox.Show("Fehler beim Login" + ex);     
            }
            
            //If authentifications is ok, login to the next page
        }
    }
}