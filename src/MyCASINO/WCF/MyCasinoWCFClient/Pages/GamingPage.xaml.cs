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

namespace MyCasinoWCFClient.Pages
{
    /// <summary>
    /// Interaction logic for GamingPage.xaml
    /// </summary>
    public partial class GamingPage : Page
    {
        private string username;

        public string Username
        {
            get { return username; }
            set { username = value; }
        }

        private int sessionId;

        public int SessionId
        {
            get { return sessionId; }
            set { sessionId = value; }
        }

        private MyCasinoUserTypes userType;

        public MyCasinoUserTypes UserType
        {
            get { return userType; }
            set { userType = value; }
        }


        private INETMyCasino _remSrvMyCasino;

        public INETMyCasino _RemSrvMyCasino
        {
            get { return _remSrvMyCasino; }
            set { _remSrvMyCasino = value; }
        }

        public GamingPage(INETMyCasino _RemSrvMyCasinoMain, string usernameTmp, int sessionIdTmp, MyCasinoUserTypes typeTmp)
        {
            InitializeComponent();
            Username = usernameTmp;
            SessionId = sessionIdTmp;
            UserType = typeTmp;
            _RemSrvMyCasino = _RemSrvMyCasinoMain;
            PlayHistoryPage.Content = new PlayPage(_RemSrvMyCasino);
            tblUsername.Text = Username; 
            
        }

        private void BtnPlay_Click(object sender, RoutedEventArgs e)
        {
            PlayHistoryPage.Content = new PlayPage(_RemSrvMyCasino);
        }

        private void BtnHistory_Click(object sender, RoutedEventArgs e)
        {
            PlayHistoryPage.Content = new HistoryPage(_RemSrvMyCasino);
        }

        private void BtnPayIn_Click(object sender, RoutedEventArgs e)
        {
            PlayHistoryPage.Content = new PayInPage(_RemSrvMyCasino);
        }

        private void btnLogout_Click(object sender, RoutedEventArgs e)
        {
            string errMsg;
            if(_RemSrvMyCasino.logout(SessionId, out errMsg))
            {
                System.Windows.Application.Current.Shutdown();
            }
                MessageBox.Show("Fehler beim Logout");
        }
    }
}
