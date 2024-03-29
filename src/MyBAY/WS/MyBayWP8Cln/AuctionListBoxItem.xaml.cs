﻿using System;
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

namespace MyBayWP8Cln
{
    /// <summary>
    /// Interaktionslogik für AuctionListBoxItem.xaml
    /// </summary>
    public partial class AuctionListBoxItem : UserControl
    {
        public UInt32 auctionNumber;
        public AuctionListBoxItem(string auctionName, UInt32 auctionNumber, Double highestBid, UInt32 countBids, UInt32 auctionState)
        {
            InitializeComponent();
            this.lblAuctionName.Text = auctionName;
            this.lbl_AuctionNumber.Text = auctionNumber.ToString();
            this.auctionNumber = auctionNumber;
            this.lbl_HighestBid.Text = highestBid.ToString("C");
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
