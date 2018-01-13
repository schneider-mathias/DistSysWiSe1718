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
using System.Globalization;

namespace MyBayWCFCln
{
    static class comError
    {
        static public string convertException(Exception ex)
        {
            string retString = "";
            switch ((UInt32)ex.HResult)
            {
                case 0xA0110001: // ERROR_AUCTIONNUMBER_NOT_AVAILABLE
                    retString = "Die angegebene Auktionsnummer ist nicht verfügbar";
                    break;
                case 0xA0110002: // ERROR_USER_ALREADY_INTERESTED
                    retString = "Sie verfolgen bereits den Status dieser Auktion";
                    break;
                // Bid
                case 0xA0110003: // ERROR_AUCTIONNUMBER_DOES_NOT_ESXIST
                    retString = "Die angegebene Auktionsnummer ist nicht verfügbar";
                    break;
                case 0xA0110004: // ERROR_AUCTION_CLOSED
                    retString = "Die Auktion ist beendet";
                    break;
                case 0xA0110005: // ERROR_BID_TOO_LOW
                    retString = "Das angegebene Gebot ist zu niedrig";
                    break;
                case 0xA0110013: // ERROR_BID_NEGATIVE
                    retString = "Das eingegebene Gebot ist negativ";
                    break;
                // Login
                case 0xA0110006: // ERROR_ALREADY_LOGGED_IN
                    retString = "Sie sind bereits eingeloggt";
                    break;
                case 0xA0110007: // ERROR_FILE_COULD_NOT_BE_OPENED
                    retString = "Die Datei konnte nicht geöffnet werden";
                    break;
                case 0xA0110008: // ERROR_USERNAME_OR_PASSWORD_WRONG
                    retString = "Der Benutzername oder das Passwort sind falsch";
                    break;
                case 0xA0110009: // ERROR_USER_NOT_LOGGED_IN
                    retString = "Der Benutzer ist nicht eingeloggt";
                    break;
                // Offer
                case 0xA0110010: // ERROR_ARTICLENAME_IS_EMPTY
                    retString = "Der Artikelname darf nicht leer sein";
                    break;
                case 0xA0110014: // ERROR_VALUE_NEGATIVE
                    retString = "Das Startgebot darf nicht negativ sein";
                    break;
                // getAuctions
                case 0xA0110011: // ERROR_NO_AUCTIONS_AVAILABLE
                    retString = "Es sind keine Auktionen verfügbar";
                    break;
                // Details
                case 0xA0110012: // ERROR_USER_IS_NOT_AUCTIONEER
                    retString = "Der angemeldete Benutzer ist nicht der Auktionator dieser Auktion";
                    break;
                default:
                    retString = "Unbekannte COM Exception" + ex.Message;
                    break;
            }
            return retString;
        }
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
#if COM
        /// <summary>
        /// MyBay COM Server
        /// </summary>
        private COMMyBaySrvLib.COMMyBay _comServer;
#else
        private ChannelFactory<IMyBay> _MyBayFactory = new ChannelFactory<IMyBay>(new BasicHttpBinding());
        private IMyBay _remoteSrvMyBay;
#endif
                    
        private UInt32 sessionID;
        private string srvAddress;

        private Timer getMessageTimer;
        public event EventHandler myListBoxUpdateEventHandler;

         public MainWindow()
         {
            this.DataContext = this;

            this.getMessageTimer = new Timer();
            this.getMessageTimer.Interval = 1500;
            this.getMessageTimer.Elapsed += OnTimedEvent;

            InitializeComponent();
            myListBoxUpdateEventHandler += myMessageListBoxUpdateEvent;
#if COM
            this.txtBox_serverIP.Text = "192.168.52.208";
#else
#endif
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
#if COM
                Array com_message;
                int messageAvailableTemp;
                _comServer.getMessage(this.sessionID, out messageAvailableTemp, out messageType, out com_message);
                messageAvailable = Convert.ToBoolean(messageAvailableTemp);

                message = new MessageTransfer();
                if (com_message != null)
                {
                    switch (messageType)
                    {
                        case 0: // New Bid
                            message.MessageText = Convert.ToString(com_message.GetValue(0));
                            message.MessageText2 = Convert.ToString(com_message.GetValue(1));
                            message.MessageDoubleValue = Convert.ToDouble(com_message.GetValue(2), new CultureInfo("en-US"));
                            message.MessageIntValue = Convert.ToUInt32(com_message.GetValue(3));
                            break;
                        case 1:
                            message.MessageIntValue = Convert.ToUInt32(com_message.GetValue(0));
                            message.MessageText = Convert.ToString(com_message.GetValue(1));
                            message.MessageIntValue2 = Convert.ToUInt32(com_message.GetValue(2));
                            break;
                        case 2:
                            message.MessageText = Convert.ToString(com_message.GetValue(0));
                            message.MessageText2 = Convert.ToString(com_message.GetValue(1));
                            message.MessageDoubleValue = Convert.ToDouble(com_message.GetValue(2), new CultureInfo("en-US"));
                            message.MessageIntValue = Convert.ToUInt32(com_message.GetValue(3));
                            break;
                        default:
                            break;
                    }
                }
                if (com_message != null)
                {
#else
                String returnStr = _remoteSrvMyBay.getMessage(sessionID,out messageAvailable,out messageType, out message);
                if (!returnStr.Contains("OK")) return;
#endif
                    switch (messageType)
                    {
                        case 0: // New Bid
                            if (!String.IsNullOrEmpty(message.MessageText2))
                            {
                                Dispatcher.BeginInvoke(new Action(delegate ()
                                {
                                    listBox_messages.Items.Add("- Neues Gebot - Artikel: "
                                                            + message.MessageText2
                                                            + " - Gebot: "
                                                            + message.MessageDoubleValue.ToString("C")
                                                            + " - Auktionsstatus: "
                                                            + message.MessageIntValue.ToString()
                                                            + " - Bieter: "
                                                            + message.MessageText);

                                    // Send event for scrolling to the last item in the ListBox
                                    myListBoxUpdateEventHandler(this, new EventArgs());
                                }));
                            }
                            else
                            {
                                Dispatcher.BeginInvoke(new Action(delegate ()
                                {
                                    listBox_messages.Items.Add("- Neues Gebot - Artikel: "
                                                                + message.MessageText
                                                                + " - Gebot: "
                                                                + message.MessageDoubleValue.ToString("C")
                                                                + " - Auktionsstatus: "
                                                                + message.MessageIntValue.ToString());

                                    // Send event for scrolling to the last item in the ListBox
                                    myListBoxUpdateEventHandler(this, new EventArgs());
                                }));
                            }
                            break;
                        case 1:
                            Dispatcher.BeginInvoke(new Action(delegate ()
                            {
                                listBox_messages.Items.Add("Auktion: "
                                                            + message.MessageText
                                                            + " endet bald, dies ist die "
                                                            + message.MessageIntValue.ToString()
                                                            + ". Warnung");

                                // Send event for scrolling to the last item in the ListBox
                                myListBoxUpdateEventHandler(this, new EventArgs());
                            }));
                            break;
                        case 2:
                            Dispatcher.BeginInvoke(new Action(delegate ()
                            {
                                listBox_messages.Items.Add("---------------------------------------------------------------------------------------\n Auktion beendet. Käufer: "
                                + message.MessageText
                                + " Preis: "
                                + message.MessageDoubleValue.ToString("C")
                                + " Artikel: "
                                + message.MessageText2.ToString()
                                + "\n---------------------------------------------------------------------------------------");
                                // Send event for scrolling to the last item in the ListBox
                                myListBoxUpdateEventHandler(this, new EventArgs());
                            }));
                            break;
                        default:
                            break;
                    }
                    if (messageAvailable)
                    {
                        this.getMessageTimer.Interval = 10;
                    }                
#if COM
            }
#else
#endif

        }
            catch (Exception except)
            {
#if COM
                MessageBoxResult result = MessageBox.Show(comError.convertException(except), "Fehler", MessageBoxButton.OK);
#else
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
            }
            
        }

        private void btn_login_Click(object sender, RoutedEventArgs e)
        {
            if (String.IsNullOrEmpty(this.txtBox_serverIP.Text))
            {
                MessageBox.Show("Bitte geben Sie eine Serveradresse ein", "Fehler", MessageBoxButton.OK);
                return;
            }

            if (this.btn_login.Content.ToString().Contains("Login"))
            {
                try
                {
#if COM
                    this.srvAddress = this.txtBox_serverIP.Text;
                    Type comType = Type.GetTypeFromCLSID(new Guid("E591CA8E-1693-4BD0-9A65-919D333389AA"), this.srvAddress, false);
                    _comServer = (COMMyBaySrvLib.COMMyBay)Activator.CreateInstance(comType);
                    _comServer.login(this.txtBox_username.Text, this.txtBox_password.Password, out sessionID);
#else
                    this.srvAddress = this.txtBox_serverIP.Text + "MyBayWCF";
                    _remoteSrvMyBay = _MyBayFactory.CreateChannel(new EndpointAddress(srvAddress));
                    String returnStr = _remoteSrvMyBay.login(this.txtBox_username.Text, this.txtBox_password.Password, out sessionID);
                    if (!returnStr.Contains("OK"))
                    {
                        MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                        return;
                    }
#endif
                    this.btn_login.Content = "Logout";
                    this.txtBox_username.IsEnabled = false;
                    this.txtBox_password.IsEnabled = false;
                    this.txtBox_serverIP.IsEnabled = false;

                    // Enable all other functionality
                    this.btn_NewAuction.IsEnabled = true;
                    this.btn_getAuctions.IsEnabled = true;
                    this.getMessageTimer.Start();
                }
                catch (Exception except)
                {
#if COM
                    MessageBoxResult result = MessageBox.Show(comError.convertException(except), "Fehler", MessageBoxButton.OK);
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

                    // Empty Messagelist
                    this.listBox_messages.Items.Clear();
                }
                catch (Exception except)
                {
#if COM
                    MessageBoxResult result = MessageBox.Show(comError.convertException(except), "Fehler", MessageBoxButton.OK);
#else
                    MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
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
                _comServer.offer(this.sessionID, this.txtBox_articleName.Text, tempStartBid, out auctionNumber);
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
            catch (Exception except)
            {
#if COM
                MessageBoxResult result = MessageBox.Show(comError.convertException(except), "Fehler", MessageBoxButton.OK);
#else
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
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
                _comServer.bid(this.sessionID, auctionNumber, bidValue);
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
            catch (Exception except)
            {
#if COM
                MessageBoxResult result = MessageBox.Show(comError.convertException(except), "Fehler", MessageBoxButton.OK);
#else
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
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
                _comServer.interested(this.sessionID, auctionNumber);

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
            catch (Exception except)
            {
#if COM
                MessageBoxResult result = MessageBox.Show(comError.convertException(except), "Fehler", MessageBoxButton.OK);
#else
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
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
                _comServer.endauction(this.sessionID, auctionNumber);
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
            catch (Exception except)
            {
#if COM
                MessageBoxResult result = MessageBox.Show(comError.convertException(except), "Fehler", MessageBoxButton.OK);
#else
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
            }
        }

        private void btn_getAuctions_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                List<AuctionTransfer> newListAuctions;

                UInt32 flags = 0;
                UInt32 countAuctions = 0;

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
                Array com_auctions;
                _comServer.getAuctions(this.sessionID, flags, this.txtBox_search.Text, out countAuctions, out com_auctions);
                newListAuctions = new List<AuctionTransfer>();
                if (com_auctions != null && countAuctions > 0)
                {
                    for (int i = 0; i < (countAuctions * 5); i++)
                    {
                        AuctionTransfer newItem = new AuctionTransfer();

                        newItem.AuctNumber = Convert.ToUInt32(com_auctions.GetValue(i));
                        newItem.ArtName = Convert.ToString(com_auctions.GetValue(++i));
                        newItem.HighestBid = Convert.ToDouble(com_auctions.GetValue(++i), new CultureInfo("en-US"));
                        newItem.AuctionState = Convert.ToUInt32(com_auctions.GetValue(++i));
                        newItem.CountBids = Convert.ToUInt32(com_auctions.GetValue(++i));

                        newListAuctions.Add(newItem);
                    }
                }
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
            catch (Exception except)
            {
#if COM
                MessageBoxResult result = MessageBox.Show(comError.convertException(except), "Fehler", MessageBoxButton.OK);
#else
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
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
                Array com_allbids;
                _comServer.details(this.sessionID, auctionNumber, out com_allbids, out countBids);
                newListBids = new List<BidTransfer>();
                if (com_allbids != null && countBids > 0)
                {
                    for (int i = 0; i < (countBids * 3); i++)
                    {
                        BidTransfer newItem = new BidTransfer();

                        newItem.BidNumber = Convert.ToUInt32(com_allbids.GetValue(i));
                        newItem.Bidder = Convert.ToString(com_allbids.GetValue(++i));
                        newItem.BidValue = Convert.ToDouble(com_allbids.GetValue(++i), new CultureInfo("en-US"));

                        newListBids.Add(newItem);
                    }
                }
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
            catch (Exception except)
            {
#if COM
                MessageBoxResult result = MessageBox.Show(comError.convertException(except), "Fehler", MessageBoxButton.OK);
#else
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
            }
        }

        // Needed for the messages to show the newest Messages automatically
        private void myMessageListBoxUpdateEvent(object sender, EventArgs e)
        {
            listBox_messages.SelectedIndex = listBox_messages.Items.Count - 1;
            listBox_messages.ScrollIntoView(listBox_messages.SelectedItem);
        }
    }
}
