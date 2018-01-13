﻿using System;
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

            //_myDataObject.RemoteSrvMyBay = new MyBayWSSrv.MyBayWSSrvASMXSoapClient();
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

            if (txtBox_serverip.Text.Contains("localhost")) {
                // Do nothing
            }
            else if (IPAddress.TryParse(txtBox_serverip.Text, out srvAddressIP))
            {
                srvAddress = srvAddress.Replace("localhost", srvAddressIP.ToString());
            }
            else
            {
                MessageBox.Show("Bitte geben Sie eine gültige IP Adresse an","Warnung",MessageBoxButton.OK);       
                return;
            }

            _myDataObject.RemoteSrvMyBay.Endpoint.Address = new EndpointAddress(srvAddress);


            _myDataObject.RemoteSrvMyBay.loginCompleted += myBaySvc_login_completed;


            _myDataObject.RemoteSrvMyBay.loginAsync(txtBox_username.Text, passBox_password.Password);
        }

        private void myBaySvc_login_completed(object sender, MyBayWSSrv.loginCompletedEventArgs args)
        {
            try
            {
                String errText = args.Result;

                if (!errText.Contains("OK"))
                {
                    MessageBox.Show(errText, "Warnung", MessageBoxButton.OK);
                    return;
                }

                else
                {
                    _myDataObject.SessionID = args.sessionID;
                    MessageBox.Show("Sie wurden erfolgreich mit der Session ID: " + _myDataObject.SessionID + " angemeldet", "Hinweis", MessageBoxButton.OK);
                    this.ShowNewDialog<MainPage>(
                        cp => { cp.MyDataObjectMain = _myDataObject; },
                        cp => { MyDataObject = cp.MyDataObjectMain; });
                }
            }
            catch (Exception except)
            {
                MessageBox.Show("Fehler beim Verbinden zum Server, haben Sie die richtige Adresse eingegeben? " + except.Message, "Warnung", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                MyDataObject.RemoteSrvMyBay.loginCompleted -= myBaySvc_login_completed;
            }
        }

    }
}