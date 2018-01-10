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

namespace MyBayWCFCln
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ChannelFactory<IMyBay> _MyBayFactory = new ChannelFactory<IMyBay>(new BasicHttpBinding());
        private IMyBay _remoteSrvMyBay;

        public MainWindow()
        {
            string srvAddress = "http://localhost:3333/MyBayWCF";
            _remoteSrvMyBay = _MyBayFactory.CreateChannel(new EndpointAddress(srvAddress));

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
    }
}
