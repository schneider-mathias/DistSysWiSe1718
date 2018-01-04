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
using MyCasinoLib;
using MyCasinoWCFLib;
using System.ServiceModel;
using System.Net;

namespace MyCasinoWCFClient
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ChannelFactory<INETMyCasino> _remSrvMyCasinoFactory = new ChannelFactory<INETMyCasino>(new BasicHttpBinding());
        private INETMyCasino _remSrvMyCasino;
        public INETMyCasino _RemSrvMyCasino
        {
            get { return _remSrvMyCasino; }
            set { _remSrvMyCasino = value; }
        }

        public MainWindow()
        {
            InitializeComponent();
            string srvAddress = "http://localhost:1200/MyCasinoWCF";
            _RemSrvMyCasino = _remSrvMyCasinoFactory.CreateChannel(new EndpointAddress(srvAddress));
            MainFrame.Navigate(new MyCasinoWCFClient.Pages.LoginPage(_RemSrvMyCasino));
        }
    }
}
