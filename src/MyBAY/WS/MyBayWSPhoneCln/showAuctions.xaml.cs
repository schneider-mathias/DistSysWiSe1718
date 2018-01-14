using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using MyBayWSPhoneCln.MyBayWSSrv;
using System.Windows.Media;
using System.Globalization;

namespace MyBayWSPhoneCln
{
    public partial class showAuctions : PhoneApplicationPage
    {
        public showAuctions()
        {
            InitializeComponent();
        }

        #region GETAUCTIONS
        private void btn_getAuctions_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                UInt32 flags = 0;

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

                App.MyDataObject.RemoteSrvMyBay.getAuctionsCompleted += myBaySvc_getAuctions_completed;

                if (String.IsNullOrEmpty(this.txtBox_search.Text))
                {
                    App.MyDataObject.RemoteSrvMyBay.getAuctionsAsync(App.MyDataObject.SessionID, flags, "");
                }
                else App.MyDataObject.RemoteSrvMyBay.getAuctionsAsync(App.MyDataObject.SessionID, flags, this.txtBox_search.Text);
            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
        }

        private void myBaySvc_getAuctions_completed(object sender, getAuctionsCompletedEventArgs args)
        {
            List<MyBayWSSrv.AuctionTransfer> newListAuctions;
            this.stackPanelAuctions.Children.Clear();

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
                    newListAuctions = new List<MyBayWSSrv.AuctionTransfer>();
                    foreach (MyBayWSSrv.AuctionTransfer item in args.auctions)
                    {
                        AuctionListBoxItem auctionItem;
                        auctionItem = new AuctionListBoxItem(item.ArtName, item.AuctNumber, item.HighestBid, item.CountBids, item.AuctionState);
                        auctionItem.VerticalAlignment = VerticalAlignment.Top;
                        auctionItem.Tap += AuctionItem_Tap; ;

                        this.stackPanelAuctions.Children.Add(auctionItem);
                    }
                }
            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                App.MyDataObject.RemoteSrvMyBay.getAuctionsCompleted -= myBaySvc_getAuctions_completed;
            }
        }
        #endregion

        private void AuctionItem_Tap(object sender, System.Windows.Input.GestureEventArgs e)
        {
            int auctionIndexInStackpanel;
            AuctionListBoxItem senderItem = (AuctionListBoxItem)sender;

            auctionIndexInStackpanel = stackPanelAuctions.Children.IndexOf(senderItem);

            auctionTapElement newTapElement = new auctionTapElement(senderItem);

            // register all events of tapelement
            newTapElement.btn_cancel.Click += Btn_cancel_Click;
            newTapElement.btn_bid.Click += Btn_bid_Click;
            newTapElement.btn_endAuction.Click += Btn_endAuction_Click;
            newTapElement.btn_interested.Click += Btn_interested_Click;
            newTapElement.btn_details.Click += Btn_details_Click;
            stackPanelAuctions.Children[auctionIndexInStackpanel] = newTapElement;
            
        }

        #region DETAILS
        private void Btn_details_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // Find parent for getting AuctionNumber
                Button senderButton = (Button)sender;
                auctionTapElement sourceTapElement;

                // Walk through the visual tree of the senderbutton to find the auctionTapElement
                DependencyObject current = senderButton.Parent;
                Type targetType = typeof(auctionTapElement);

                while (current != null)
                {
                    if (current.GetType() == targetType)
                    {
                        sourceTapElement = (auctionTapElement)current;
                        UInt32 auctionNumber = sourceTapElement.OldAuctionListBoxItem.auctionNumber;

                        App.MyDataObject.RemoteSrvMyBay.detailsCompleted += myBaySvc_details_completed;
                        App.MyDataObject.RemoteSrvMyBay.detailsAsync(App.MyDataObject.SessionID, auctionNumber);

                        break;
                    }
                    current = VisualTreeHelper.GetParent(current);
                }

            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
        }
        private void myBaySvc_details_completed(object sender, detailsCompletedEventArgs args)
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
                    List<MyBayWSSrv.BidTransfer> newListBids = new List<MyBayWSSrv.BidTransfer>();
                    foreach (MyBayWSSrv.BidTransfer item in args.allBids)
                    {
                        MyBayWSSrv.BidTransfer newItem = new MyBayWSSrv.BidTransfer();
                        newItem.Bidder = item.Bidder;
                        newItem.BidNumber = item.BidNumber;
                        newItem.BidValue = item.BidValue;
                        newListBids.Add(newItem);
                    }


                    if (args.countBids == 0)
                    {
                        MessageBox.Show("Für diese Auktion wurden noch keine Gebote abgegeben", "Hinweis", MessageBoxButton.OK);
                    }
                    else
                    {
                        this.stackPanelAuctions.Children.Clear();
                        newListBids.Sort((a, b) => (a.BidNumber.CompareTo(b.BidNumber)));

                        foreach (MyBayWSSrv.BidTransfer item in newListBids)
                        {
                            TextBlock bid = new TextBlock();
                            bid.Text = "GebotNr: "
                                                        + item.BidNumber.ToString()
                                                        + " - Höhe Gebot: "
                                                        + item.BidValue.ToString("C")
                                                        + " - Bieter: "
                                                        + item.Bidder.ToString();
                            bid.HorizontalAlignment = HorizontalAlignment.Stretch;
                            bid.VerticalAlignment = VerticalAlignment.Top;
                            this.stackPanelAuctions.Children.Add(bid);
                        }
                    }
                }
            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                App.MyDataObject.RemoteSrvMyBay.detailsCompleted -= myBaySvc_details_completed;
            }
        }
        #endregion

        #region INTERESTED
        private void Btn_interested_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // Find parent for getting AuctionNumber
                Button senderButton = (Button)sender;
                auctionTapElement sourceTapElement;

                // Walk through the visual tree of the senderbutton to find the auctionTapElement
                DependencyObject current = senderButton.Parent;
                Type targetType = typeof(auctionTapElement);

                while (current != null)
                {
                    if (current.GetType() == targetType)
                    {
                        sourceTapElement = (auctionTapElement)current;
                        UInt32 auctionNumber = sourceTapElement.OldAuctionListBoxItem.auctionNumber;

                        App.MyDataObject.RemoteSrvMyBay.interestedCompleted += myBaySvc_interested_completed;
                        App.MyDataObject.RemoteSrvMyBay.interestedAsync(App.MyDataObject.SessionID, auctionNumber);

                        break;
                    }
                    current = VisualTreeHelper.GetParent(current);
                }

            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
        }
        private void myBaySvc_interested_completed(object sender, interestedCompletedEventArgs args)
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
                    MessageBox.Show("Sie folgen dieser Auktion jetzt", "Hinweis", MessageBoxButton.OK);
                }
            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                App.MyDataObject.RemoteSrvMyBay.interestedCompleted -= myBaySvc_interested_completed;
            }
        }
        #endregion

        #region ENDAUCTION
        private void Btn_endAuction_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // Find parent for getting AuctionNumber
                Button senderButton = (Button)sender;
                auctionTapElement sourceTapElement;

                // Walk through the visual tree of the senderbutton to find the auctionTapElement
                DependencyObject current = senderButton.Parent;
                Type targetType = typeof(auctionTapElement);

                while (current != null)
                {
                    if (current.GetType() == targetType)
                    {
                        sourceTapElement = (auctionTapElement)current;
                        UInt32 auctionNumber = sourceTapElement.OldAuctionListBoxItem.auctionNumber;

                        App.MyDataObject.RemoteSrvMyBay.endauctionCompleted += myBaySvc_endauction_completed;
                        App.MyDataObject.RemoteSrvMyBay.endauctionAsync(App.MyDataObject.SessionID, auctionNumber);

                        break;
                    }
                    current = VisualTreeHelper.GetParent(current);
                }

            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
        }
        private void myBaySvc_endauction_completed(object sender, endauctionCompletedEventArgs args)
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
                    MessageBox.Show("Auktion wird beendet", "Hinweis", MessageBoxButton.OK);
                }
            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                App.MyDataObject.RemoteSrvMyBay.endauctionCompleted -= myBaySvc_endauction_completed;
            }
        }
        #endregion

        #region BID
        private void Btn_bid_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // Find parent for getting AuctionNumber and Value of BID
                Button senderButton = (Button)sender;
                auctionTapElement sourceTapElement;

                // Walk through the visual tree of the senderbutton to find the auctionTapElement
                DependencyObject current = senderButton.Parent;
                Type targetType = typeof(auctionTapElement);

                while (current != null)
                {
                    if (current.GetType() == targetType)
                    {
                        sourceTapElement = (auctionTapElement)current;
                        UInt32 auctionNumber = sourceTapElement.OldAuctionListBoxItem.auctionNumber;
                        Double bidValue;
                        if (!Double.TryParse(sourceTapElement.txtBox_bid.Text,NumberStyles.Any,new CultureInfo("en-US"), out bidValue))
                        {
                            MessageBox.Show("Bitte geben Sie einen gültigen Wert für das Gebot an", "Warnung", MessageBoxButton.OK);
                        }
                        App.MyDataObject.RemoteSrvMyBay.bidCompleted += myBaySvc_bid_completed;
                        App.MyDataObject.RemoteSrvMyBay.bidAsync(App.MyDataObject.SessionID, auctionNumber, bidValue);

                        break;
                    }
                    current = VisualTreeHelper.GetParent(current);
                }

            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
        }
        private void myBaySvc_bid_completed(object sender, bidCompletedEventArgs args)
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
                    MessageBox.Show("Gebot Abgegeben", "Hinweis", MessageBoxButton.OK);
                }
            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
            finally
            {
                // Event im Eventhandler abmelden
                App.MyDataObject.RemoteSrvMyBay.bidCompleted -= myBaySvc_bid_completed;
            }
        }
        #endregion
        private void Btn_cancel_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                int tapElementIndexInStackpanel = -1;
                Button senderButton = (Button)sender;
                auctionTapElement sourceTapElement;

                // Walk through the visual tree of the senderbutton to find the auctionTapElement

                DependencyObject current = senderButton.Parent;
                Type targetType = typeof(auctionTapElement);

                while (current != null)
                {
                    if (current.GetType() == targetType)
                    {
                        sourceTapElement = (auctionTapElement)current;
                        tapElementIndexInStackpanel = stackPanelAuctions.Children.IndexOf(sourceTapElement);

                        if (tapElementIndexInStackpanel != -1)
                        {
                            stackPanelAuctions.Children[tapElementIndexInStackpanel] = sourceTapElement.OldAuctionListBoxItem;
                        }
                        break;
                    }
                    current = VisualTreeHelper.GetParent(current);
                }
            }
            catch (Exception except)
            {
                MessageBox.Show(except.Message, "Fehler", MessageBoxButton.OK);
            }
        }
    }
}