using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using MyBayWSPhoneCln.Resources;
using PortForwardLib;
using MyBayWSPhoneCln.MyBayWSSrvASMXSoapService;

namespace MyBayWSPhoneCln
{
    /// <summary>
    /// Mainpage of the Windows Phone 8 Application
    /// </summary>
    public partial class MainPage : PhoneApplicationPage
    {
        /// <summary>
        /// Constructor of the MainPage
        /// </summary>
        public MainPage()
        {
            InitializeComponent();

            // Need to check if it already exists, so not everytime the mainpage is created, the object is created new
            if (App.MyDataObject.RemoteSrvMyBay == null)
            {
                App.MyDataObject.RemoteSrvMyBay = new MyBayWSSrvASMXSoapClient();
            }

            // If user is logged in, change button content to Logout and register other method for click event
            if (App.MyDataObject.SessionID > 0)
            {
                this.btn_login.Content = "Logout";

                this.btn_login.Click -= btn_login_Click;
                this.btn_login.Click += btn_logout_Click;
            }
        }

        /// <summary>
        /// Method handles the click event of the Login button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_login_Click(object sender, RoutedEventArgs e)
        {
            if (App.MyDataObject.SessionID != 0)
            {
                MessageBox.Show("Sie sind bereits eingeloggt", "Fehler", MessageBoxButton.OK);
                return;
            }
            this.ShowNewDialog<LoginPage>();
        }

        /// <summary>
        /// Method handles the click event of the Logout button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_logout_Click(object sender, RoutedEventArgs e)
        {
            App.MyDataObject.RemoteSrvMyBay.logoutCompleted += myBaySvc_logout_completed;
            App.MyDataObject.RemoteSrvMyBay.logoutAsync(App.MyDataObject.SessionID);
        }


        /// <summary>
        /// Method which handles the logoutCompleted event
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void myBaySvc_logout_completed(object sender, logoutCompletedEventArgs args)
        {
            try
            {
                // Handle connection error
                if (args != null) App.handleConnectionError(args);

                String errText = args.Result;

                if (!errText.Contains("OK"))
                {
                    MessageBox.Show(errText, "Warnung", MessageBoxButton.OK);
                    return;
                }

                else
                {
                    App.MyDataObject.SessionID = 0;
                    this.btn_login.Content = "Login";

                    this.btn_login.Click -= btn_logout_Click;
                    this.btn_login.Click += btn_login_Click;
                    MessageBox.Show("User wurde erfolgreich abgemeldet", "Hinweis", MessageBoxButton.OK);
                }
            }
            catch (Exception)
            {
                MessageBox.Show("Unerwarteter Fehler bei der Durchführung des Logout", "Warnung", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                App.MyDataObject.RemoteSrvMyBay.logoutCompleted -= myBaySvc_logout_completed;
            }
        }

        /// <summary>
        /// Method handles the click event of the NewAuction Button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_NewAuction_Click(object sender, RoutedEventArgs e)
        {
            // Check, if user is logged in
            if (App.MyDataObject.SessionID == 0)
            {
                MessageBox.Show("Sie sind nicht angemeldet, melden Sie sich bitte an", "Fehler", MessageBoxButton.OK);
                return;
            }
            this.ShowNewDialog<newAuction>();
        }

        /// <summary>
        /// Method handles the click event of the showAuctions Button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_showAuctions_Click(object sender, RoutedEventArgs e)
        {
            // Check, if user is logged in
            if (App.MyDataObject.SessionID == 0)
            {
                MessageBox.Show("Sie sind nicht angemeldet, melden Sie sich bitte an", "Fehler", MessageBoxButton.OK);
                return;
            }
            this.ShowNewDialog<showAuctions>();
        }
    }
}