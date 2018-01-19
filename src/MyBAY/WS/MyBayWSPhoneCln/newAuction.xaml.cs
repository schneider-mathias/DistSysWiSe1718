/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Background logic of the newAuction page                 */
/*               of the WP8 Client                                       */
/*                                                                       */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     19.01.2018                                              */
/*                                                                       */
/*************************************************************************/

using System;
using System.Windows;
using Microsoft.Phone.Controls;
using System.Globalization;
using MyBayWSPhoneCln.MyBayWSSrvASMXSoapService;

namespace MyBayWSPhoneCln
{
    public partial class newAuction : PhoneApplicationPage
    {
        /// <summary>
        /// Constructor of the class newAuction
        /// </summary>
        public newAuction()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Method handles Click event of btn_NewAuction button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_NewAuction_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Double tempStartBid;
                // check if input in txtBox_startBid is a valid double
                if (!Double.TryParse(this.txtBox_startBid.Text, NumberStyles.Any, new CultureInfo("en-US"), out tempStartBid))
                {
                    MessageBox.Show("Bitte geben Sie einen gültigen Wert für das Startgebot an", "Warnung", MessageBoxButton.OK);
                }

                // register event handler for offerCompleted event
                App.MyDataObject.RemoteSrvMyBay.offerCompleted += myBaySvc_offer_completed;

                App.MyDataObject.RemoteSrvMyBay.offerAsync(App.MyDataObject.SessionID,this.txtBox_articleName.Text,tempStartBid);
            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
        }

        /// <summary>
        /// Method handles the offerCompleted event (callback function of offerAsync method)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void myBaySvc_offer_completed(object sender, offerCompletedEventArgs args)
        {
            try
            {
                // handle connection error
                if (args != null) App.handleConnectionError(args);

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
                // remove method in the eventhandler
                App.MyDataObject.RemoteSrvMyBay.offerCompleted -= myBaySvc_offer_completed;
            }
        }
    }   
}