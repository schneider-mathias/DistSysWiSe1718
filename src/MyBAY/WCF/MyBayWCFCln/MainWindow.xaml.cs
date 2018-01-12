//#define COM

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
using MyBayLib;
using MyBayWCFLibrary;
using System.ServiceModel;
using System.Net;
using System.Text.RegularExpressions;
using System.Timers;

namespace MyBayWCFCln
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
#if COM
        /// <summary>
        /// MyBay COM Server
        /// </summary>
        //private .COMMyINVENT _myBayCOMSrv = new COMMyINVENTSvrLib.COMMyINVENT();
#else
        private ChannelFactory<IMyBay> _MyBayFactory = new ChannelFactory<IMyBay>(new BasicHttpBinding());
        private IMyBay _remoteSrvMyBay;
#endif
                    
        private UInt32 sessionID;
        private string srvAddress;

        private Timer getMessageTimer;

        public MainWindow()
        {
            this.DataContext = this;

            this.getMessageTimer = new Timer();
            this.getMessageTimer.Interval = 1500;
            this.getMessageTimer.Elapsed += OnTimedEvent;

            InitializeComponent();
        }

        private void OnTimedEvent(Object source, System.Timers.ElapsedEventArgs e)
        {

            if (sessionID == 0) return;

            this.getMessageTimer.Interval = 1500;
            Boolean messageAvailable;
            UInt32 messageType;
            MessageTransfer message;

            try
            {
                String returnStr = _remoteSrvMyBay.getMessage(sessionID,out messageAvailable,out messageType, out message);
                if (returnStr.Contains("OK"))
                {
                    switch (messageType)
                    {
                        case 0: // New Bid
                            if (!String.IsNullOrEmpty(message.MessageText2))
                            {
                                Dispatcher.BeginInvoke(new Action(delegate ()
                                {
                                    listBox_messages.Items.Add("Neues Gebot für Artikel: "
                                                            + message.MessageText2
                                                            + " Gebot: "
                                                            + message.MessageDoubleValue.ToString("C")
                                                            + " Auktionsstatus: "
                                                            + message.MessageIntValue.ToString()
                                                            + " Höchstbietender: "
                                                            + message.MessageText);
                                }));                                
                            }
                            else
                            {
                                Dispatcher.BeginInvoke(new Action(delegate ()
                                {
                                    listBox_messages.Items.Add("Neues Gebot für Artikel: "
                                                                + message.MessageText
                                                                + " Gebot: "
                                                                + message.MessageDoubleValue.ToString("C")
                                                                + " Auktionsstatus: "
                                                                + message.MessageIntValue.ToString());
                                }));
                            }
                            break;
                        case 1:
                            Dispatcher.BeginInvoke(new Action(delegate ()
                            {
                                listBox_messages.Items.Add("Auktion: "
                                                            + message.MessageText
                                                            + " mit der Auktionsnummer: "
                                                            + message.MessageIntValue2.ToString()
                                                            + "\t endet bald, dies ist die "
                                                            + message.MessageIntValue.ToString()
                                                            + ". Warnung");
                            
                            }));
                            break;
                        case 2:
                            Dispatcher.BeginInvoke(new Action(delegate ()
                            {
                                listBox_messages.Items.Add("Auktion beendet. Käufer: "
                                + message.MessageText
                                + " Preis: "
                                + message.MessageDoubleValue.ToString("C")
                                + " Auktionsstatus: "
                                + message.MessageIntValue.ToString());
                            }));
                            break;
                        default:
                            break;
                    }
                    if (messageAvailable)
                    {
                        this.getMessageTimer.Interval = 10;
                    }
                }
                else
                {
                    return;
                }
            }
            catch (Exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
            
        }

        private void btn_login_Click(object sender, RoutedEventArgs e)
        {
            if (String.IsNullOrEmpty(this.txtBox_serverIP.Text))
            {
                MessageBox.Show("Bitte geben Sie eine Serveradresse ein", "Fehler", MessageBoxButton.OK);
                return;
            }

            this.srvAddress = this.txtBox_serverIP.Text + "MyBayWCF";
            if (this.btn_login.Content.ToString().Contains("Login"))
            {
                try
                {
#if COM
                    Type comType = Type.GetTypeFromCLSID(new Guid("UID"), this.srvAddress, false);
                    _comSrv = (mybay.mybaycom)Activator.CreateInstance(comType);
                    _comServer.login(this.usernameTxtBox.Text, this.passwordBox.Password, out sessionID);
#else
                    _remoteSrvMyBay = _MyBayFactory.CreateChannel(new EndpointAddress(srvAddress));
                    String returnStr = _remoteSrvMyBay.login(this.txtBox_username.Text, this.txtBox_password.Password, out sessionID);
                    if (!returnStr.Contains("OK"))
                    {
                        MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                        return;
                    }
                    
                    this.btn_login.Content = "Logout";
                    this.txtBox_username.IsEnabled = false;
                    this.txtBox_password.IsEnabled = false;
                    this.txtBox_serverIP.IsEnabled = false;

                    // Enable all other functionality
                    this.btn_NewAuction.IsEnabled = true;
                    this.btn_getAuctions.IsEnabled = true;
                    this.getMessageTimer.Start();
#endif
                }
                catch (Exception except)
                {
#if COM
                    // TODO com exception translation
#else
                    MessageBox.Show("Fehler beim Verbinden zum Server, haben Sie die richtige Adresse eingegeben? " + except.Message, "Warnung", MessageBoxButton.OK);
#endif
                }
            }
            else
            {
                try
                {
#if COM
                    _comServer.logout(this.sessionID);
                    this.getMessageTimer.Stop();

#else
                    String returnStr = _remoteSrvMyBay.logout(this.sessionID);
                    if (!returnStr.Contains("OK"))
                    {
                        MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                        return;
                    }
                    this.getMessageTimer.Stop();

                    // Close Client Connection
                    if (_remoteSrvMyBay != null)
                    {
                        ((IClientChannel)_remoteSrvMyBay).Close();
                        ((IDisposable)_remoteSrvMyBay).Dispose();
                        _remoteSrvMyBay = null;
                    }
#endif
                    MessageBox.Show("User wurde erfolgreich abgemeldet", "Hinweis", MessageBoxButton.OK);
                    this.sessionID = 0;
                    this.txtBox_username.IsEnabled = true;
                    this.txtBox_password.IsEnabled = true;
                    this.txtBox_serverIP.IsEnabled = true;
                    this.btn_login.Content = "Login";

                    // Disable all other functionality
                    this.btn_NewAuction.IsEnabled = false;
                    this.btn_getAuctions.IsEnabled = false;
                    this.btn_interested.IsEnabled = false;
                    this.btn_bid.IsEnabled = false;
                    this.btn_getDetails.IsEnabled = false;
                    this.btn_endAuction.IsEnabled = false;
                }
                catch (Exception except)
                {
#if COM
                    // TODO com exception translation
#else
                    MessageBox.Show("Fehler beim Verbinden zum Server, haben Sie die richtige Adresse eingegeben? " + except.Message, "Warnung", MessageBoxButton.OK);
#endif
                }
            }
        }

        private void btn_NewAuction_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                UInt32 auctionNumber;

                Double tempStartBid;
                if (!Double.TryParse(this.txtBox_startBid.Text, out tempStartBid))
                {
                   MessageBox.Show("Bitte geben Sie einen gültigen Wert für das Startgebot an", "Warnung", MessageBoxButton.OK);
                }

#if COM

#else
                String returnStr = _remoteSrvMyBay.offer(sessionID, this.txtBox_articleName.Text, tempStartBid, out auctionNumber);

                if (!returnStr.Contains("OK"))
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                    return;
                }
#endif
                MessageBox.Show("Auktion erfolgreich mit der Auktionsnummer: " + auctionNumber.ToString() + " erstellt", "Hinweis", MessageBoxButton.OK);
            }
            catch (Exception exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }

        private void btn_bid_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (this.listBox_auctions.SelectedItem == null)
                {
                    MessageBox.Show("Sie haben keine Auktion ausgewählt", "Hinweis", MessageBoxButton.OK);
                    return;
                }

                var selectedAuction = this.listBox_auctions.SelectedItem;
                UInt32 auctionNumber = (selectedAuction as AuctionListBoxItem).auctionNumber;

                Double bidValue;
                if (!Double.TryParse(this.txtBox_bid.Text, out bidValue))
                {
                    MessageBox.Show("Bitte geben Sie einen gültigen Wert für das Gebot an", "Warnung", MessageBoxButton.OK);
                }

#if COM

#else
                String returnStr = _remoteSrvMyBay.bid(sessionID, auctionNumber, bidValue);
                if (!returnStr.Contains("OK"))
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                    return;
                }
#endif
                MessageBox.Show("Gebot Abgegeben", "Hinweis", MessageBoxButton.OK);
            }
            catch (Exception exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }

        private void btn_interested_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (this.listBox_auctions.SelectedItem == null)
                {
                    MessageBox.Show("Sie haben keine Auktion ausgewählt", "Hinweis", MessageBoxButton.OK);
                    return;
                }

                var selectedAuction = this.listBox_auctions.SelectedItem;
                UInt32 auctionNumber = (selectedAuction as AuctionListBoxItem).auctionNumber;

#if COM

#else
                String returnStr = _remoteSrvMyBay.interested(sessionID, auctionNumber);
                if (!returnStr.Contains("OK"))
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                    return;
                }
#endif
                MessageBox.Show("Sie folgen dieser Auktion jetzt", "Hinweis", MessageBoxButton.OK);
            }
            catch (Exception exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }

        private void btn_endAuction_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (this.listBox_auctions.SelectedItem == null)
                {
                    MessageBox.Show("Sie haben keine Auktion ausgewählt", "Hinweis", MessageBoxButton.OK);
                    return;
                }

                var selectedAuction = this.listBox_auctions.SelectedItem;
                UInt32 auctionNumber = (selectedAuction as AuctionListBoxItem).auctionNumber;

#if COM

#else
                String returnStr = _remoteSrvMyBay.endauction(sessionID, auctionNumber);

                if (!returnStr.Contains("OK"))
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                    return;
                }
#endif
                MessageBox.Show("Auktion wird beendet", "Hinweis", MessageBoxButton.OK);
            }
            catch (Exception exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }

        private void btn_getAuctions_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                List<AuctionTransfer> newListAuctions;

                UInt32 flags = 0;
                UInt32 countAuctions;

                if ((bool)radioBtn_interested.IsChecked)
                {
                    flags = 0;
                }
                else if ((bool)radioBtn_allOpen.IsChecked)
                {
                    flags = 1;
                }
                else if ((bool)radioBtn_all.IsChecked)
                {
                    flags = 2;
                }

                this.listBox_auctions.Items.Clear();

#if COM

#else
                String returnStr;
                if (String.IsNullOrEmpty(this.txtBox_search.Text))
                {
                    returnStr = _remoteSrvMyBay.getAuctions(sessionID, flags, "", out countAuctions, out newListAuctions);
                }
                else returnStr = _remoteSrvMyBay.getAuctions(sessionID, flags, this.txtBox_search.Text, out countAuctions, out newListAuctions);

                if (!returnStr.Contains("OK"))
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                    return;
                }
#endif
                foreach (AuctionTransfer item in newListAuctions)
                {
                    this.listBox_auctions.Items.Add(new AuctionListBoxItem(item.ArtName, item.AuctNumber, item.HighestBid, item.CountBids, item.AuctionState));
                }
                if (this.listBox_auctions.Items.Count > 0)
                {
                    this.btn_getDetails.IsEnabled = true;
                    this.btn_bid.IsEnabled = true;
                    this.btn_endAuction.IsEnabled = true;
                    this.btn_interested.IsEnabled = true;
                }
                else
                {
                    this.btn_getDetails.IsEnabled = false;
                    this.btn_bid.IsEnabled = false;
                    this.btn_endAuction.IsEnabled = false;
                    this.btn_interested.IsEnabled = false;
                }
            }
            catch (Exception exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }

        private void btn_getDetails_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                List<BidTransfer> newListBids;

                UInt32 countBids;

                if (this.listBox_auctions.SelectedItem == null)
                {
                    MessageBox.Show("Sie haben keine Auktion ausgewählt", "Hinweis", MessageBoxButton.OK);
                    return;
                }

                var selectedAuction = this.listBox_auctions.SelectedItem;
                UInt32 auctionNumber = (selectedAuction as AuctionListBoxItem).auctionNumber;

#if COM

#else
                String returnStr = _remoteSrvMyBay.details(sessionID, auctionNumber, out countBids, out newListBids);
                if (!returnStr.Contains("OK"))
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                    return;
                }

#endif
                this.listBox_auctions.Items.Clear();

                if (countBids == 0)
                {
                    this.listBox_auctions.Items.Add("Für diese Auktion wurden noch keine Gebote abgegeben");
                }

                newListBids.Sort((a, b) => (a.BidNumber.CompareTo(b.BidNumber)));

                foreach (BidTransfer item in newListBids)
                {
                    this.listBox_auctions.Items.Add("GebotNr: "
                                                + item.BidNumber.ToString()
                                                + " - Höhe Gebot: "
                                                + item.BidValue.ToString("C")
                                                + " - Bieter: "
                                                + item.Bidder.ToString());
                }
                this.btn_interested.IsEnabled = false;
                this.btn_bid.IsEnabled = false;
                this.btn_endAuction.IsEnabled = false;
                this.btn_getDetails.IsEnabled = false;
            }
            catch (Exception exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }
    }
}
