//#define PORTFWDLIBACTIVE

using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Windows.UI.Popups;
using Windows.UI.Notifications;
using System.ServiceModel;
using MyBayWSPhoneCln.MyBayWSSrvASMXSoapService;

namespace MyBayWSPhoneCln
{
    public partial class LoginPage : PhoneApplicationPage
    {
        private DataObject _myDataObject = new DataObject();

        public DataObject MyDataObject
        {
            get { return _myDataObject; }
            set { _myDataObject = value; }
        }

        public LoginPage()
        {
            InitializeComponent();
        }

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

            App.MyDataObject.RemoteSrvMyBay.loginCompleted += myBaySvc_login_completed;


            App.MyDataObject.RemoteSrvMyBay.loginAsync(txtBox_username.Text, passBox_password.Password);
        }

        private void myBaySvc_login_completed(object sender, loginCompletedEventArgs args)
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
                    App.MyDataObject.SessionID = args.sessionID;
                    MessageBox.Show("Sie wurden erfolgreich mit der Session ID: " + App.MyDataObject.SessionID + " angemeldet", "Hinweis", MessageBoxButton.OK);
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