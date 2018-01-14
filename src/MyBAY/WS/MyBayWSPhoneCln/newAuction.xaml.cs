using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using System.Globalization;

namespace MyBayWSPhoneCln
{
    public partial class newAuction : PhoneApplicationPage
    {
        public newAuction()
        {
            InitializeComponent();
        }

        private void btn_NewAuction_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Double tempStartBid;
                if (!Double.TryParse(this.txtBox_startBid.Text, NumberStyles.Any, new CultureInfo("en-US"), out tempStartBid))
                {
                    MessageBox.Show("Bitte geben Sie einen gültigen Wert für das Startgebot an", "Warnung", MessageBoxButton.OK);
                }

                App.MyDataObject.RemoteSrvMyBay.offerCompleted += myBaySvc_offer_completed;

                App.MyDataObject.RemoteSrvMyBay.offerAsync(App.MyDataObject.SessionID,this.txtBox_articleName.Text,tempStartBid);
            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
        }
        private void myBaySvc_offer_completed(object sender, MyBayWSSrv.offerCompletedEventArgs args)
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
                    MessageBox.Show("Auktion erfolgreich mit der Auktionsnummer: " + args.auctionNumber.ToString() + " erstellt", "Hinweis", MessageBoxButton.OK);
                    this.ShowNewDialog<MainPage>();
                }
            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                App.MyDataObject.RemoteSrvMyBay.offerCompleted -= myBaySvc_offer_completed;
            }
        }
    }   
}