/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Background logic LoginPage                              */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     19.01.2018                                              */
/*                                                                       */
/*************************************************************************/

//#define PORTFWDLIBACTIVE

using System;
using System.Net;
using System.Windows;
using Microsoft.Phone.Controls;
using System.ServiceModel;
using MyBayWSPhoneCln.MyBayWSSrvASMXSoapService;

namespace MyBayWSPhoneCln
{
    public partial class LoginPage : PhoneApplicationPage
    {
        /// <summary>
        /// Constructor of class LoginPage
        /// </summary>
        public LoginPage()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Method handles Click event of btn_login button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_login_Click(object sender, RoutedEventArgs e)
        {
            IPAddress srvAddressIP;
            string srvAddress = "http://localhost:3333/MyBayWSSrvASMX.asmx";

            if (String.IsNullOrEmpty(this.txtBox_serverip.Text))
            {
                MessageBox.Show("Bitte geben Sie eine Serveradresse ein", "Fehler", MessageBoxButton.OK);
                return;
            }

            if (!txtBox_serverip.Text.Contains("localhost"))
            {
#if PORTFWDLIBACTIVE
                MessageBox.Show("IP-Adresse kann bei Verwendung der PortforwardLib nicht geändert werden", "Warnung", MessageBoxButton.OK);
                txtBox_serverip.Text = "localhost";
                return;
#else
                if (IPAddress.TryParse(txtBox_serverip.Text, out srvAddressIP))
                {
                    srvAddress = srvAddress.Replace("localhost", srvAddressIP.ToString());
                }
                else
                {
                    MessageBox.Show("Bitte geben Sie eine gültige IP Adresse an", "Warnung", MessageBoxButton.OK);
                    return;
                }
                App.MyDataObject.RemoteSrvMyBay.Endpoint.Address = new EndpointAddress(srvAddress);
#endif
            }

            // register eventhandler for event loginCompleted
            App.MyDataObject.RemoteSrvMyBay.loginCompleted += myBaySvc_login_completed;

            // Call server method login asynchron
            App.MyDataObject.RemoteSrvMyBay.loginAsync(txtBox_username.Text, passBox_password.Password);
        }

        /// <summary>
        /// Callback function of asynchron server request loginAsync
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void myBaySvc_login_completed(object sender, loginCompletedEventArgs args)
        {
            try
            {
                // Handle connection error
                if (args != null) App.handleConnectionError(args);

                String errText = args.Result;

                // Handle error case if something went wrong in the Login method on the server
                if (!errText.Contains("OK"))
                {
                    MessageBox.Show(errText, "Warnung", MessageBoxButton.OK);
                    return;
                }
                else
                {
                    App.MyDataObject.SessionID = args.sessionID;
                    MessageBox.Show("Sie wurden erfolgreich mit der Session ID: " + App.MyDataObject.SessionID + " angemeldet", "Hinweis", MessageBoxButton.OK);
                    // Go back to MainPage
                    this.ShowNewDialog<MainPage>();
                }
            }
            catch (Exception)
            {
                MessageBox.Show("Fehler beim Verbinden zum Server, haben Sie die richtige Adresse eingegeben?", "Warnung", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                App.MyDataObject.RemoteSrvMyBay.loginCompleted -= myBaySvc_login_completed;
            }
        }

    }
}