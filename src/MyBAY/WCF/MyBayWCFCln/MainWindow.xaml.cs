/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Background logic of the MainWindow (main functionality  */
/*               of WCF-Client - This client can also operate with a     */
/*               COM-Object if you set the define COM before compiling   */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     19.01.2018                                              */
/*                                                                       */
/*************************************************************************/

//#define COM

using System;
using System.Collections.Generic;
using System.Windows;
using MyBayLib;
using MyBayWCFLibrary;
using System.ServiceModel;
using System.Timers;
using System.Globalization;

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
        private COMMyBaySrvLib.COMMyBay _comServer;
#else
        // Create a ChannelFactory for the interface "IMyBay"
        private ChannelFactory<IMyBay> _MyBayFactory = new ChannelFactory<IMyBay>(new BasicHttpBinding());
        private IMyBay _remoteSrvMyBay;
#endif

        private UInt32 sessionID;
        private string srvAddress;

        /// <summary>
        /// This timer is enabled in the constructor of the MainWindow and polls cyclic for new messages at the server
        /// </summary>
        private Timer getMessageTimer;

        /// <summary>
        /// This EventHandler handles events for updating the listbox displaying the messages
        /// </summary>
        public event EventHandler myListBoxUpdateEventHandler;

        /// <summary>
        /// This boolean variable is needed because otherwise the client would open several messageboxes
        /// to inform the user about a connection error on return of not valid
        /// requests while cyclic getting the messages
        /// </summary>
        private bool informedUserAboutServerError = false;

        /// <summary>
        /// Constructor of the MainWindow
        /// </summary>
         public MainWindow()
         {
            this.DataContext = this;

            this.getMessageTimer = new Timer();
            this.getMessageTimer.Interval = 1500;
            this.getMessageTimer.Elapsed += OnTimedEvent;

            InitializeComponent();
            myListBoxUpdateEventHandler += myMessageListBoxUpdateEvent;
#if COM
            // Set Text of IP input textBox (for the demonstration - IP-address of other computer in the lab)
            this.txtBox_serverIP.Text = "192.168.52.208";
#else
#endif
        }

        /// <summary>
        /// This method handles the Elapsed event of the getMessageTimer
        /// It cyclic asks the server if there are new messages for the client
        /// </summary>
        /// <param name="source"></param>
        /// <param name="e"></param>
        private void OnTimedEvent(Object source, System.Timers.ElapsedEventArgs e)
        {
            // Not logged in
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
                            // For adding the messages to the listbox on the MainWindow, you need to invoke this action with the dispatcher
                            // of the MainWindow because the timer calling this method is using a different thread
                                Dispatcher.BeginInvoke(new Action(delegate ()
                                {
                                    listBox_messages.Items.Add("- Neues Gebot - Artikel: "
                                                            + message.MessageText2
                                                            + " - Gebot: "
                                                            + String.Format(new CultureInfo("en-US"), "{0:C}", message.MessageDoubleValue)
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
                                                                + String.Format(new CultureInfo("en-US"), "{0:C}", message.MessageDoubleValue)
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
                                + String.Format(new CultureInfo("en-US"), "{0:C}", message.MessageDoubleValue)
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
            catch (Exception)
            {
                // If something went wrong with getting the messages from the server, disable the polling for messages
                this.getMessageTimer.Stop();
#if COM
                if(!informedUserAboutServerError)
                {
                    MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
                    informedUserAboutServerError = true;
                }
#else
                if (!informedUserAboutServerError)
                {
                    MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
                    informedUserAboutServerError = true;
                }
#endif
            }
            
        }

        /// <summary>
        /// Method handles Click event of btn_login button 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_login_Click(object sender, RoutedEventArgs e)
        {
            // This method is changing its behaviour depending on the content of the btn_login button.
            // It is used for the login and logout functionality, if a user is successfully logged in, the
            // content of the button changes to "logout". If a user is successfully logged out, it 
            // changes back to "login"
            if (this.btn_login.Content.ToString().Contains("Login"))
            {
                // Check if textbox for inserting the server IP address is empty
                if (String.IsNullOrEmpty(this.txtBox_serverIP.Text))
                {
                    MessageBox.Show("Bitte geben Sie eine Serveradresse ein", "Fehler", MessageBoxButton.OK);
                    return;
                }

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
                    // Change button content to "Logout" if user is successfully logged in
                    // and disable the textboxes needed for the login
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
                    string retvalue;
                    if (comError.convertException(except, out retvalue))
                    {
                        MessageBox.Show(retvalue, "Fehler", MessageBoxButton.OK);
                    }
                    else
                    {
                        MessageBox.Show("Fehler beim Verbinden zum Server, haben Sie die richtige Adresse eingegeben?", "Fehler", MessageBoxButton.OK);
                    }    
#else
                    MessageBox.Show("Fehler beim Verbinden zum Server, haben Sie die richtige Adresse eingegeben?", "Warnung", MessageBoxButton.OK);
#endif
                }
            }
            // If logout button is clicked same button, different functionality
            else
            {
                try
                {
#if COM
                    _comServer.logout(this.sessionID);
                    // if logout is succesful, stop the cyclic event of polling the messages from the server
                    this.getMessageTimer.Stop();
#else
                    String returnStr = _remoteSrvMyBay.logout(this.sessionID);
                    if (!returnStr.Contains("OK"))
                    {
                        MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                        return;
                    }
                    // if logout is succesful, stop the cyclic event of polling the messages from the server
                    this.getMessageTimer.Stop();

                    // Close Client Connection to WCF-Server
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

                    // Change content of button btn_login back to "Login" if the logout is completed
                    this.btn_login.Content = "Login";

                    // Disable all other functionality on GUI
                    this.btn_NewAuction.IsEnabled = false;
                    this.btn_getAuctions.IsEnabled = false;
                    this.btn_interested.IsEnabled = false;
                    this.btn_bid.IsEnabled = false;
                    this.btn_getDetails.IsEnabled = false;
                    this.btn_endAuction.IsEnabled = false;

                    // Empty AuctionList
                    this.listBox_auctions.Items.Clear();
                    // Empty Messagelist
                    this.listBox_messages.Items.Clear();
                }
                catch (Exception except)
                {
#if COM
                    string retvalue;
                    if (comError.convertException(except, out retvalue))
                    {
                        MessageBox.Show(retvalue, "Fehler", MessageBoxButton.OK);
                    }
                    else
                    {
                        MessageBox.Show("Fehler beim Verbinden zum Server", "Fehler", MessageBoxButton.OK);
                    }
#else
                    MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
                }
            }
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
                UInt32 auctionNumber;

                Double tempStartBid;
                // Check if input in txtBox_startBid is a valid double
                if (!Double.TryParse(this.txtBox_startBid.Text, NumberStyles.Any, new CultureInfo("en-US"), out tempStartBid))
                {
                    MessageBox.Show("Bitte geben Sie einen gültigen Wert für das Startgebot an", "Warnung", MessageBoxButton.OK);
                    return;
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
                string retvalue;
                if (comError.convertException(except, out retvalue))
                {
                    MessageBox.Show(retvalue, "Fehler", MessageBoxButton.OK);
                }
                else
                {
                    MessageBox.Show("Fehler beim Verbinden zum Server", "Fehler", MessageBoxButton.OK);
                }
#else
                MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
            }
        }

        /// <summary>
        /// Method handles Click event of btn_bid button 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_bid_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // Check if any auction in the listbox is selected
                if (this.listBox_auctions.SelectedItem == null)
                {
                    MessageBox.Show("Sie haben keine Auktion ausgewählt", "Hinweis", MessageBoxButton.OK);
                    return;
                }

                // Get auction number from selected listbox item
                var selectedAuction = this.listBox_auctions.SelectedItem;
                UInt32 auctionNumber = (selectedAuction as AuctionListBoxItem).auctionNumber;

                Double bidValue;
                if (!Double.TryParse(this.txtBox_bid.Text, NumberStyles.Any, new CultureInfo("en-US"), out bidValue))
                {
                    MessageBox.Show("Bitte geben Sie einen gültigen Wert für das Gebot an", "Warnung", MessageBoxButton.OK);
                    return;
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
                string retvalue;
                if (comError.convertException(except, out retvalue))
                {
                    MessageBox.Show(retvalue, "Fehler", MessageBoxButton.OK);
                }
                else
                {
                    MessageBox.Show("Fehler beim Verbinden zum Server", "Fehler", MessageBoxButton.OK);
                }
#else
                MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
            }
        }

        /// <summary>
        /// Method handles Click event of btn_interested button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_interested_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // Check if any auction in the listbox is selected
                if (this.listBox_auctions.SelectedItem == null)
                {
                    MessageBox.Show("Sie haben keine Auktion ausgewählt", "Hinweis", MessageBoxButton.OK);
                    return;
                }

                // Get auction number from selected listbox item
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
                string retvalue;
                if (comError.convertException(except, out retvalue))
                {
                    MessageBox.Show(retvalue, "Fehler", MessageBoxButton.OK);
                }
                else
                {
                    MessageBox.Show("Fehler beim Verbinden zum Server", "Fehler", MessageBoxButton.OK);
                }
#else
                MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
            }
        }

        /// <summary>
        /// Method handles Click event of btn_endAuction button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_endAuction_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // Check if any auction in the listbox is selected
                if (this.listBox_auctions.SelectedItem == null)
                {
                    MessageBox.Show("Sie haben keine Auktion ausgewählt", "Hinweis", MessageBoxButton.OK);
                    return;
                }

                // Get auction number from selected listbox item
                var selectedAuction = this.listBox_auctions.SelectedItem;
                UInt32 auctionNumber = (selectedAuction as AuctionListBoxItem).auctionNumber;

                // Call endauction method on remote server
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
                string retvalue;
                if (comError.convertException(except, out retvalue))
                {
                    MessageBox.Show(retvalue, "Fehler", MessageBoxButton.OK);
                }
                else
                {
                    MessageBox.Show("Fehler beim Verbinden zum Server", "Fehler", MessageBoxButton.OK);
                }
#else
                MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
            }
        }

        /// <summary>
        /// Method handles Click event of btn_getAuctions button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_getAuctions_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // create reference for a list of AuctionTransfer objects which 
                // will be passed back from the server
                List<AuctionTransfer> newListAuctions;

                UInt32 flags = 0;
                UInt32 countAuctions = 0;

                // Set the flags for the request for auctions at the server 
                // depening on which radio button is checked
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

                // Clear the listbox of auctions to display the new received list
                this.listBox_auctions.Items.Clear();

#if COM
                Array com_auctions;
                _comServer.getAuctions(this.sessionID, flags, this.txtBox_search.Text, out countAuctions, out com_auctions);
                newListAuctions = new List<AuctionTransfer>();

                // for the communication with a com object, the values of the received array have to be parsed to the correct
                // datatypes first
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
                // if auctions available, enable the buttons to interact with the auctions
                if (this.listBox_auctions.Items.Count > 0)
                {
                    this.btn_getDetails.IsEnabled = true;
                    this.btn_bid.IsEnabled = true;
                    this.btn_endAuction.IsEnabled = true;
                    this.btn_interested.IsEnabled = true;
                }
                // if no auctions with the requested criteria have been found, disable the buttons to interact with the auctions
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
                string retvalue;
                if (comError.convertException(except, out retvalue))
                {
                    MessageBox.Show(retvalue, "Fehler", MessageBoxButton.OK);
                }
                else
                {
                    MessageBox.Show("Fehler beim Verbinden zum Server", "Fehler", MessageBoxButton.OK);
                }
#else
                MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
            }
        }

        /// <summary>
        /// Method handles Click event of btn_getDetails button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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

                // Get auction number by checking which auction is selected in listbox
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
                // Clear the listBox for the auctions to display the details of an auction in the same listBox
                this.listBox_auctions.Items.Clear();

                // If there are no bids yet for this auction
                if (countBids == 0)
                {
                    this.listBox_auctions.Items.Add("Für diese Auktion wurden noch keine Gebote abgegeben");
                }

                // Sort the list of bids, that they are in the right order. They are ordered by their property BidNumber
                newListBids.Sort((a, b) => (a.BidNumber.CompareTo(b.BidNumber)));

                foreach (BidTransfer item in newListBids)
                {
                    this.listBox_auctions.Items.Add("GebotNr: "
                                                + item.BidNumber.ToString()
                                                + " - Höhe Gebot: "
                                                + String.Format(new CultureInfo("en-US"), "{0:C}", item.BidValue)
                                                + " - Bieter: "
                                                + item.Bidder.ToString());
                }

                // While showing the details of an auction, disable all the other buttons, that the user
                // can not modify other auctions
                this.btn_interested.IsEnabled = false;
                this.btn_bid.IsEnabled = false;
                this.btn_endAuction.IsEnabled = false;
                this.btn_getDetails.IsEnabled = false;
            }
            catch (Exception except)
            {
#if COM
                string retvalue;
                if (comError.convertException(except, out retvalue))
                {
                    MessageBox.Show(retvalue, "Fehler", MessageBoxButton.OK);
                }
                else
                {
                    MessageBox.Show("Fehler beim Verbinden zum Server", "Fehler", MessageBoxButton.OK);
                }
#else
                MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
#endif
            }
        }

        /// <summary>
        /// Handles the event which is fired everytime a message is added to the listbox 
        /// displaying the messages that the user always sees the newest message selected
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void myMessageListBoxUpdateEvent(object sender, EventArgs e)
        {
            listBox_messages.SelectedIndex = listBox_messages.Items.Count - 1;
            listBox_messages.ScrollIntoView(listBox_messages.SelectedItem);
        }
    }
}
