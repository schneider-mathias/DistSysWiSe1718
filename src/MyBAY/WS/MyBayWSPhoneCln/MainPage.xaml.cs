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

namespace MyBayWSPhoneCln
{
    public partial class MainPage : PhoneApplicationPage
    {
        // Konstruktor
        public MainPage()
        {
            InitializeComponent();

            // Need to check if it already exists, so not everytime the mainpage is created, the object is created new
            if (App.MyDataObject.RemoteSrvMyBay == null)
            {
                App.MyDataObject.RemoteSrvMyBay = new MyBayWSSrv.MyBayWSSrvASMXSoapClient();
            }

            // If User is logged in, change Button to Logout Button and register other event for logout function

            if (App.MyDataObject.SessionID > 0)
            {
                this.btn_login.Content = "Logout";

                this.btn_login.Click -= btn_login_Click;
                this.btn_login.Click += btn_logout_Click;
            }
        }

        private void btn_login_Click(object sender, RoutedEventArgs e)
        {
            //this.ShowNewDialog<LoginPage>(
            //    cp => { cp.MyDataObject = _myDataObjectMain; },
            //    cp => { MyDataObjectMain = cp.MyDataObject; });
            this.ShowNewDialog<LoginPage>();
        }

        private void btn_logout_Click(object sender, RoutedEventArgs e)
        {
            App.MyDataObject.RemoteSrvMyBay.logoutCompleted += myBaySvc_logout_completed;

            App.MyDataObject.RemoteSrvMyBay.logoutAsync(App.MyDataObject.SessionID);
        }

        private void myBaySvc_logout_completed(object sender, MyBayWSSrv.logoutCompletedEventArgs args)
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
                    App.MyDataObject.SessionID = 0;
                    this.btn_login.Content = "Login";

                    this.btn_login.Click -= btn_logout_Click;
                    this.btn_login.Click += btn_login_Click;
                    MessageBox.Show("User wurde erfolgreich abgemeldet", "Hinweis", MessageBoxButton.OK);
                }
            }
            catch (Exception except)
            {
                MessageBox.Show("Fehler beim Verbinden zum Server, haben Sie die richtige Adresse eingegeben? " + except.Message, "Warnung", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                App.MyDataObject.RemoteSrvMyBay.logoutCompleted -= myBaySvc_logout_completed;
            }
        }

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
    }
}