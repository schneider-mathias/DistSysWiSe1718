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
        private ChannelFactory<IMyBay> _MyBayFactory = new ChannelFactory<IMyBay>(new BasicHttpBinding());
        private IMyBay _remoteSrvMyBay;
        private UInt32 sessionID;
        private string srvAddress;

        private Timer getMessageTimer;

        public MainWindow()
        {
            this.DataContext = this;

            this.getMessageTimer = new Timer();
            this.getMessageTimer.Interval = 40000.0;
            this.getMessageTimer.Elapsed += OnTimedEvent;
            //string srvAddress = "http://localhost:3333/MyBayWCF";
            //_remoteSrvMyBay = _MyBayFactory.CreateChannel(new EndpointAddress(srvAddress));

            /*
            UInt32 sessionIDUser1;
            UInt32 sessionIDUser2;
            UInt32 sessionIDUser3;
            String teststring = _remoteSrvMyBay.login("Martin", "Passwort1", out sessionIDUser1);
             teststring = _remoteSrvMyBay.login("Manuel", "Passwort2", out sessionIDUser2);
             teststring = _remoteSrvMyBay.login("Mathias", "Passwort3", out sessionIDUser3);

            UInt32 auctionnumber1;
            UInt32 auctionnumber2;
            UInt32 auctionnumber3;
            teststring = _remoteSrvMyBay.offer(sessionIDUser1, "AUTO", 22.99, out auctionnumber1);
            teststring = _remoteSrvMyBay.offer(sessionIDUser2, "Haus", 1.99, out auctionnumber2);
            teststring = _remoteSrvMyBay.offer(sessionIDUser3, "Ente", 10.00, out auctionnumber3);

            List<AuctionTransfer> newList;
            UInt32 count;
            teststring = _remoteSrvMyBay.getAuctions(sessionIDUser1, 1, "", out count, out newList);

            teststring = _remoteSrvMyBay.getAuctions(sessionIDUser1, 0, "", out count, out newList);

            teststring = _remoteSrvMyBay.getAuctions(sessionIDUser1, 0, "H", out count, out newList);

            teststring = _remoteSrvMyBay.getAuctions(sessionIDUser1, 2, "H", out count, out newList);
            */
            InitializeComponent();
        }

        private void OnTimedEvent(Object source, System.Timers.ElapsedEventArgs e)
        {
            try
            {
                if (sessionID == 0) return;

               // this.getMessageTimer.Interval = 1500;
                Boolean messageAvailable;
                UInt32 messageType;
                MessageTransfer message;

                String returnStr = _remoteSrvMyBay.getMessage(sessionID,out messageAvailable,out messageType, out message);
                if (returnStr.Contains("OK"))
                {
                    switch (messageType)
                    {
                        case 0: // New Bid
                            if (!String.IsNullOrEmpty(message.MessageText2))
                            {
                                this.messageListBox.Items.Add("Neues Gebot für Artikel: "
                                                            + message.MessageText2
                                                            + " Gebot: "
                                                            + message.MessageDoubleValue.ToString()
                                                            + " Auktionsstatus: "
                                                            + message.MessageIntValue.ToString()
                                                            + " Höchstbietender: "
                                                            + message.MessageText);
                            }
                            else
                            {
                                this.messageListBox.Items.Add("Neues Gebot für Artikel: "
                                                                + message.MessageText
                                                                + "\t Gebot: "
                                                                + message.MessageDoubleValue.ToString()
                                                                + "\t Auktionsstatus: "
                                                                + message.MessageIntValue.ToString());
                            }
                            break;
                        case 1:
                            this.messageListBox.Items.Add("Auktion: "
                                                            + message.MessageText
                                                            + " mit der Auktionsnummer: "
                                                            + message.MessageIntValue.ToString()
                                                            + "\t endet bald, dies ist die "
                                                            + message.MessageIntValue2.ToString()
                                                            + ". Warnung");
                            break;
                        case 2:
                            this.messageListBox.Items.Add("Auktion beendet. Käufer: "
                                + message.MessageText
                                + ". Preis: "
                                + message.MessageDoubleValue.ToString()
                                + ". Auktionsstatus: "
                                + message.MessageIntValue.ToString());
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

        private void loginBtn_Click(object sender, RoutedEventArgs e)
        {
            this.srvAddress = this.srvIPTxtBox.Text + "MyBayWCF";
            if (this.loginBtn.Content.ToString().Contains("Login"))
            {
                try
                {
                    _remoteSrvMyBay = _MyBayFactory.CreateChannel(new EndpointAddress(srvAddress));

                    String returnStr = _remoteSrvMyBay.login(this.usernameTxtBox.Text, this.passwordBox.Password, out sessionID);

                    if (returnStr.Contains("OK"))
                    {
                        MessageBox.Show("User wurde erfolgreich mit der Session ID: " + sessionID.ToString() + " angemeldet", "Hinweis", MessageBoxButton.OK);
                        this.loginBtn.Content = "Logout";
                        this.usernameTxtBox.IsEnabled = false;
                        this.passwordBox.IsEnabled = false;
                        this.srvIPTxtBox.IsEnabled = false;

                        // Enable all other functionality
                        this.btn_NewAuction.IsEnabled = true;
                        this.getAuctions.IsEnabled = true;

                        this.getMessageTimer.Start();
                    }
                    else
                    {
                        MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                    }
                }
                catch (Exception)
                {
                    MessageBoxResult result = MessageBox.Show("Fehler beim Verbinden zum Server, haben Sie die richtige Adresse eingegeben?", "Warnung", MessageBoxButton.OK);
                }
            }
            else
            {
                try
                {
                    _remoteSrvMyBay = _MyBayFactory.CreateChannel(new EndpointAddress(srvAddress));

                    String returnStr = _remoteSrvMyBay.logout(this.sessionID);

                    if (returnStr.Contains("OK"))
                    {
                        MessageBox.Show("User wurde erfolgreich abgemeldet", "Hinweis", MessageBoxButton.OK);
                        this.sessionID = 0;
                        this.loginBtn.Content = "Login";
                        this.usernameTxtBox.IsEnabled = true;
                        this.passwordBox.IsEnabled = true;
                        this.srvIPTxtBox.IsEnabled = true;

                        // Disable all other functionality
                        this.btn_NewAuction.IsEnabled = false;
                        this.getAuctions.IsEnabled = false;

                        this.getMessageTimer.Stop();

                    }
                    else
                    {
                        MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                    }
                }
                catch (Exception)
                {
                    MessageBoxResult result = MessageBox.Show("Fehler beim Verbinden zum Server, haben Sie die richtige Adresse eingegeben?", "Warnung", MessageBoxButton.OK);
                }
            }

        }

        private void btn_NewAuction_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                UInt32 auctionNumber;

                Double tempStartBid;
                if (!Double.TryParse(this.startBid_TxtBox.Text, out tempStartBid))
                {
                   MessageBox.Show("Bitte geben Sie einen gültigen Wert für das Startgebot an", "Warnung", MessageBoxButton.OK);
                }

                String returnStr = _remoteSrvMyBay.offer(sessionID,this.artNameTxtBox.Text,tempStartBid, out auctionNumber);

                if (returnStr.Contains("OK"))
                {
                    MessageBox.Show("Auktion erfolgreich mit der Auktionsnummer: " + auctionNumber.ToString() + " erstellt", "Hinweis", MessageBoxButton.OK);

                }
                else
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                }
            }
            catch (Exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }

        //private void startBid_TxtBox_PreviewTextInput(object sender, TextCompositionEventArgs e)
        //{
        //    Regex regex = new Regex("^$?[0-9][0-9.]*(.d{1,2})?$|^$?[.]([d][d]?)$");
        //    e.Handled = !regex.IsMatch((sender as TextBox).Text.Insert((sender as TextBox).SelectionStart, e.Text));
        //}


        private void getAuctions_Click_1(object sender, RoutedEventArgs e)
        {
            try
            {
                List<AuctionTransfer> newListAuctions;

                UInt32 flags = 0;
                UInt32 countAuctions;

                if ((bool)radioBtnInterested.IsChecked)
                {
                    flags = 0;
                }
                else if ((bool)radioBtnAllOpen.IsChecked)
                {
                    flags = 1;
                }
                else if ((bool)radioBtnAll.IsChecked)
                {
                    flags = 2;
                }
                
                String returnStr = _remoteSrvMyBay.getAuctions(sessionID,flags,"", out countAuctions, out newListAuctions);
                if (returnStr.Contains("OK"))
                {
                    this.eventListBox.Items.Clear();

                    foreach (AuctionTransfer item in newListAuctions)
                    {
                        this.eventListBox.Items.Add(new AuctionListBoxItem(item.ArtName,item.AuctNumber,item.HighestBid,item.CountBids, item.AuctionState));
                    }
                    if (this.eventListBox.Items.Count > 0)
                    {
                        this.getDetails.IsEnabled = true;
                        this.newBid_Btn.IsEnabled = true;
                        this.auctionEnd_Btn.IsEnabled = true;
                    }
                    else
                    {
                        this.getDetails.IsEnabled = false;
                        this.newBid_Btn.IsEnabled = false;
                        this.auctionEnd_Btn.IsEnabled = false;
                    }
                }
                else
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                }
            }
            catch (Exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }

        private void getDetails_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                List<BidTransfer> newListBids;

                UInt32 countBids;

                if (this.eventListBox.SelectedItem == null)
                {
                    MessageBox.Show("Sie haben keine Auktion ausgewählt", "Hinweis", MessageBoxButton.OK);
                    return;
                }

                var selectedAuction = this.eventListBox.SelectedItem;
                UInt32 auctionNumber = (selectedAuction as AuctionListBoxItem).auctionNumber;

                String returnStr = _remoteSrvMyBay.details(sessionID,auctionNumber, out countBids, out newListBids);
                if (returnStr.Contains("OK"))
                {
                    this.eventListBox.Items.Clear();

                    if (countBids == 0)
                    {
                        this.eventListBox.Items.Add("Für diese Auktion wurden noch keine Gebote abgegeben");
                    }

                    newListBids.Sort((a, b) => (a.BidNumber.CompareTo(b.BidNumber)));

                    foreach (BidTransfer item in newListBids)
                    {
                        this.eventListBox.Items.Add("GebotNr: "
                                                    + item.BidNumber.ToString()
                                                    + " - Höhe Gebot: "
                                                    + item.BidValue.ToString()
                                                    + " - Bieter: "
                                                    + item.Bidder.ToString());
                    }
                    this.newBid_Btn.IsEnabled = false;
                    this.auctionEnd_Btn.IsEnabled = false;
                }
                else
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                }
            }
            catch (Exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }

        private void newBid_Btn_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (this.eventListBox.SelectedItem == null)
                {
                    MessageBox.Show("Sie haben keine Auktion ausgewählt", "Hinweis", MessageBoxButton.OK);
                    return;
                }

                var selectedAuction = this.eventListBox.SelectedItem;
                UInt32 auctionNumber = (selectedAuction as AuctionListBoxItem).auctionNumber;

                Double bidValue;
                if (!Double.TryParse(this.newBid_txtBox.Text, out bidValue))
                {
                    MessageBox.Show("Bitte geben Sie einen gültigen Wert für das Gebot an", "Warnung", MessageBoxButton.OK);
                }

                String returnStr = _remoteSrvMyBay.bid(sessionID, auctionNumber, bidValue);
                if (returnStr.Contains("OK"))
                {
                    MessageBox.Show("Gebot Abgegeben", "Hinweis", MessageBoxButton.OK);
                }
                else
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                }
            }
            catch (Exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }

        private void auctionEnd_Btn_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (this.eventListBox.SelectedItem == null)
                {
                    MessageBox.Show("Sie haben keine Auktion ausgewählt", "Hinweis", MessageBoxButton.OK);
                    return;
                }

                var selectedAuction = this.eventListBox.SelectedItem;
                UInt32 auctionNumber = (selectedAuction as AuctionListBoxItem).auctionNumber;

                String returnStr = _remoteSrvMyBay.endauction(sessionID, auctionNumber);
                if (returnStr.Contains("OK"))
                {
                    MessageBox.Show("Auktion wird beendet", "Hinweis", MessageBoxButton.OK);
                }
                else
                {
                    MessageBox.Show(returnStr, "Fehler", MessageBoxButton.OK);
                }
            }
            catch (Exception)
            {
                MessageBoxResult result = MessageBox.Show("Fehler bei der Verbindung zum Server", "Warnung", MessageBoxButton.OK);
            }
        }
    }
}
