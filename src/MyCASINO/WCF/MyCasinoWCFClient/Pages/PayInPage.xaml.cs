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

#if COM
using System.Runtime.InteropServices;
#endif

namespace MyCasinoWCFClient.Pages
{
    /// <summary>
    /// Interaction logic for PayInPage.xaml
    /// </summary>
    public partial class PayInPage : Page
    {
#if COM
        private COMMyCasinoSrvLib.COMMyCasino _comSrv;

        public COMMyCasinoSrvLib.COMMyCasino _ComSrv
        {
            get { return _comSrv; }
            set { _comSrv = value; }
        }

        private uint sessionId;

        public uint SessionId
        {
            get { return sessionId; }
            set { sessionId = value; }
        }


        public PayInPage(COMMyCasinoSrvLib.COMMyCasino _comSrvTmp, string usernameTmp, uint sessionIdTmp, short typeTmp)
        {
            _ComSrv = _comSrvTmp;
            SessionId = sessionIdTmp;
            InitializeComponent();
            //fill combobox (static)
            cbxPayInUsername.Items.Add("Casino");
            cbxPayInUsername.Items.Add("Gamer");
            cbxPayInUsername.Items.Add("Gamer_2");
            //cbxPayInUsername.Items.Add("Johannes");
            //cbxPayInUsername.Items.Add("Mathias");

        }
#else
        private INETMyCasino _remSrvMyCasino;

        public INETMyCasino _RemSrvMyCasino
        {
            get { return _remSrvMyCasino; }
            set { _remSrvMyCasino = value; }
        }

        private int sessionId;

        public int SessionId
        {
            get { return sessionId; }
            set { sessionId = value; }
        }


        public PayInPage(INETMyCasino _RemSrvMyCasinoMain, string usernameTmp, int sessionIdTmp, MyCasinoUserTypes typeTmp)
        {
            _RemSrvMyCasino = _RemSrvMyCasinoMain;
            SessionId = sessionIdTmp;
            InitializeComponent();
            //fill combobox (static)
            cbxPayInUsername.Items.Add("Martin");
            cbxPayInUsername.Items.Add("Manuel");
            cbxPayInUsername.Items.Add("Johannes");
            cbxPayInUsername.Items.Add("Mathias");
            
        }
#endif
        private void btnPayIn_Click(object sender, RoutedEventArgs e)
        {
            string errMsg=null;
            double amount;
            double.TryParse(tbxPayInAmount.Text, out amount);
#if COM
            try
            {
                _ComSrv.deposit(SessionId, cbxPayInUsername.Text, amount, out errMsg);
            }
            catch (Exception ex)
            {
                if (ex is COMException)
                    errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                else
                    errMsg = "Unknown";

                MessageBox.Show(errMsg);
                return;
            }
#else
            _RemSrvMyCasino.deposit(SessionId, cbxPayInUsername.Text, amount, out errMsg);
            if (errMsg == "USER_NOT_LOGGED_IN")
            {
                MessageBox.Show("User nicht eingeloggt");
                return;
            }
            else if (errMsg == "PAYIN_TOO_HIGH")
            {
                MessageBox.Show("Soviel kann nicht auf einmal Einbezahlt werden");
                return;
            }
            else if (errMsg == "MAXIMUM_BALANCE_REACHED")
            {
                MessageBox.Show("Account Geld maximum erreicht!");
                return;
            }
            else if (errMsg == "INVALID_SESSION_ID")
            {
                MessageBox.Show("Ungültige ID!");
                return;
            }


#endif
            tblPayInSuccessful.Visibility = Visibility.Visible;
            btnPayIn.IsEnabled = false;
        }

        private void tbxPayInAmount_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = !(e.Key >= Key.D0 && e.Key <= Key.D9 ||
               e.Key >= Key.NumPad0 && e.Key <= Key.NumPad9 ||
               e.Key == Key.Back || e.Key == Key.OemPeriod
               );
        }

        private void tbxPayInAmount_KeyDown(object sender, KeyEventArgs e)
        {
            tblPayInSuccessful.Visibility = Visibility.Hidden;
            btnPayIn.IsEnabled = true;
            //Only one period is allowed
            if (e.Key == Key.OemPeriod)
            {
                if (((TextBox)sender).Text.ToString().Contains('.'))
                {
                    e.Handled = true;
                    return;
                }
            }

            //Set money format
            if (((TextBox)sender).Text.ToString().Contains('.'))
            {
                string[] tmp = ((TextBox)sender).Text.ToString().Split('.');
                if (tmp[1].Length >= 2)
                {
                    e.Handled = true;
                    return;
                }

            }
        }

        private void cbxPayInUsername_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            tblPayInSuccessful.Visibility = Visibility.Hidden;
            btnPayIn.IsEnabled = true;
        }
    }
}
