using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace MyBayWP8Cln
{
    public partial class auctionTapElement : UserControl
    {
        private AuctionListBoxItem oldAuctionListBoxItem;

        public AuctionListBoxItem OldAuctionListBoxItem
        {
            get {
                return this.oldAuctionListBoxItem;
            }
        }


        public auctionTapElement(AuctionListBoxItem sourceItem)
        {
            this.oldAuctionListBoxItem = sourceItem;
            InitializeComponent();
        }

    }
}
