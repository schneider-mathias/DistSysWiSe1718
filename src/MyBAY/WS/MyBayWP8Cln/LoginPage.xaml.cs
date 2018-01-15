using System;
using System.Net;
using System.Windows;
using Microsoft.Phone.Controls;
using System.ServiceModel;
using MyBayWP8Cln.MyBayWSSrvASMXSoapService;

namespace MyBayWP8Cln
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
            string srvAddress = "http://localhost:7777/MyBayWSSrvASMX.asmx";

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

            App.MyDataObject.RemoteSrvMyBay.Endpoint.Address = new EndpointAddress(srvAddress);


            App.MyDataObject.RemoteSrvMyBay.loginCompleted += myBaySvc_login_completed;


            App.MyDataObject.RemoteSrvMyBay.loginAsync(txtBox_username.Text, passBox_password.Password);
        }

        private void myBaySvc_login_completed(object sender, loginCompletedEventArgs args)
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
                    App.MyDataObject.SessionID = args.sessionID;
                    MessageBox.Show("Sie wurden erfolgreich mit der Session ID: " + App.MyDataObject.SessionID + " angemeldet", "Hinweis", MessageBoxButton.OK);
                    //this.ShowNewDialog<MainPage>(
                    //    cp => { cp.MyDataObjectMain = _myDataObject; },
                    //    cp => { MyDataObject = cp.MyDataObjectMain; });
                    this.ShowNewDialog<MainPage>();
                }
            }
            catch (Exception except)
            {
                MessageBox.Show("Fehler beim Verbinden zum Server, haben Sie die richtige Adresse eingegeben? " + except.Message, "Warnung", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                App.MyDataObject.RemoteSrvMyBay.loginCompleted -= myBaySvc_login_completed;
            }
        }

    }
}