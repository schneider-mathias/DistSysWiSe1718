//#define COM
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
        public MainWindow()
        {
            InitializeComponent();
#if COM
            Type comType = Type.GetTypeFromCLSID(new Guid("C45F55FC-76D5-4D30-A7D0-2DF66C22ED0D"), "127.0.0.1", false);
            COMMyCasinoSrvLib.COMMyCasino _comSrv = (COMMyCasinoSrvLib.COMMyCasino)Activator.CreateInstance(comType);
            MainFrame.Navigate(new MyCasinoWCFClient.Pages.LoginPage(_comSrv));
#else
            MainFrame.Navigate(new MyCasinoWCFClient.Pages.LoginPage());
#endif

        }
    }
}
