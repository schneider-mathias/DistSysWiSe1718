/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Background logic of AuctionListBoxItem user control     */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     19.01.2018                                              */
/*                                                                       */
/*************************************************************************/

using System;
using System.Globalization;
using System.Windows.Controls;
using System.Windows.Media;

namespace MyBayWSPhoneCln
{
    /// <summary>
    /// Interaktionslogik für AuctionListBoxItem.xaml
    /// </summary>
    public partial class AuctionListBoxItem : UserControl
    {
        public UInt32 auctionNumber;

        /// <summary>
        /// Constructor of AuctionListBoxItem is setting all its properties and filling the labels content
        /// </summary>
        /// <param name="auctionName"></param>
        /// <param name="auctionNumber"></param>
        /// <param name="highestBid"></param>
        /// <param name="countBids"></param>
        /// <param name="auctionState"></param>
        public AuctionListBoxItem(string auctionName, UInt32 auctionNumber, Double highestBid, UInt32 countBids, UInt32 auctionState)
        {
            InitializeComponent();
            this.lblAuctionName.Text = auctionName;
            this.lbl_AuctionNumber.Text = auctionNumber.ToString();
            this.auctionNumber = auctionNumber;
            this.lbl_HighestBid.Text = String.Format(new CultureInfo("en-US"), "{0:C}", highestBid);
            this.lbl_CountBids.Text = "Gebote: " + countBids.ToString();

            switch (auctionState)
            {
                case 0:
                    this.AuctionListBoxGrid1.Background = new SolidColorBrush(Colors.Green); ;
                    break;
                case 1:
                    this.AuctionListBoxGrid1.Background = new SolidColorBrush(Colors.Cyan); ;
                    break;
                case 2:
                    this.AuctionListBoxGrid1.Background = new SolidColorBrush(Colors.Red);
                    break;
                default:
                    this.AuctionListBoxGrid1.Background = new SolidColorBrush(Colors.White); 
                    break;
            }
        }
    }
}
