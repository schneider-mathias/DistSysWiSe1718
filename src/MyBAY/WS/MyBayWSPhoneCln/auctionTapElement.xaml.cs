/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Background logic of auctionTapElement usercontrol       */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     19.01.2018                                              */
/*                                                                       */
/*************************************************************************/

using System.Windows.Controls;

namespace MyBayWSPhoneCln
{
    public partial class auctionTapElement : UserControl
    {
        private AuctionListBoxItem oldAuctionListBoxItem;

        /// <summary>
        /// auctionTapElement stores AuctionListBoxItem which was creating this 
        /// auctionTapElement in its Tap eventhandler
        /// </summary>
        public AuctionListBoxItem OldAuctionListBoxItem
        {
            get {
                return this.oldAuctionListBoxItem;
            }
        }

        /// <summary>
        /// Constructor of class auctionTapElement
        /// </summary>
        /// <param name="sourceItem"></param>
        public auctionTapElement(AuctionListBoxItem sourceItem)
        {
            this.oldAuctionListBoxItem = sourceItem;
            InitializeComponent();
        }

    }
}
