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

            UInt32 sessionID;
            String teststring = _remoteSrvMyBay.login("BlaBlubb", "22334", out sessionID);

            UInt32 auctionnumber;
            teststring = _remoteSrvMyBay.offer(sessionID, "AUTO", 22.99, out auctionnumber);

            InitializeComponent();
        }
    }
}
