//#define COM

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
using System.Runtime.InteropServices;

namespace MyCasinoWCFClient.Pages
{
    /// <summary>
    /// Interaction logic for LoginPage.xaml
    /// </summary>
    public partial class LoginPage : Page
    {

#if COM
        private COMMyCasinoSrvLib.COMMyCasino _comSrv;

        public COMMyCasinoSrvLib.COMMyCasino _ComSrv
        {
            get { return _comSrv; }
            set { _comSrv = value; }
        }

#else
        private ChannelFactory<INETMyCasino> _remSrvMyCasinoFactory = new ChannelFactory<INETMyCasino>(new BasicHttpBinding());
        private INETMyCasino _remSrvMyCasinoLogin;

        public INETMyCasino _RemSrvMyCasinoLogin
        {
            get { return _remSrvMyCasinoLogin; }
            set { _remSrvMyCasinoLogin = value; }
        }
#endif

        public ulong _sessionId;
        private short _userType;
        private string _errMsg;

        public LoginPage()
        {
            InitializeComponent();
        }

#region Username and Password standard values

        /// <summary>
        /// Set textbox to empty string when clicking
        /// Change Fontcolors accordingly
        /// </summary>
        /// <param name="sender">Username textbox</param>
        /// <param name="e"></param>
        private void TextBox_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (((TextBox)sender).Text == "Username")
            {
                tbxUsername.Text = "";
            }

            ((TextBox)sender).Foreground = new SolidColorBrush(Colors.Black);

        }

        /// <summary>
        /// Set textbox to Username when focus is lost and nothing is written in the Username textbox
        /// Change Fontcolors accordingly
        /// </summary>
        /// <param name="sender">Username textbox</param>
        /// <param name="e"></param>
        private void tbxUsername_LostFocus(object sender, RoutedEventArgs e)
        {
            if (((TextBox)sender).Text == "")
            {
                ((TextBox)sender).Foreground = new SolidColorBrush(Colors.LightGray);
                ((TextBox)sender).Text = "Username";
            }
        }

        /// <summary>
        /// Set Passwordbox to an empty string when clicking into the password field
        /// Change Fontcolors accordingly
        /// </summary>
        /// <param name="sender">Password box</param>
        /// <param name="e"></param>
        private void PasswordBox_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (((PasswordBox)sender).Password == "****")
            {
                pwbPassword.Password = "";
            }
            ((PasswordBox)sender).Foreground = new SolidColorBrush(Colors.Black);
        }

        /// <summary>
        /// Set Passwordbox to the standardvalue when focus is lost and nothing has been added in the password field
        /// Change Fontcolors accordingly
        /// </summary>
        /// <param name="sender">Password box</param>
        /// <param name="e"></param>
        private void pwbPassword_LostFocus(object sender, RoutedEventArgs e)
        {
            if (((PasswordBox)sender).Password == "")
            {
                ((PasswordBox)sender).Foreground = new SolidColorBrush(Colors.LightGray);
                pwbPassword.Password = "****";
            }
        }
        /// <summary>
        /// Passwordbox got focus with tab
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void pwbPassword_GotFocus(object sender, RoutedEventArgs e)
        {
            if (((PasswordBox)sender).Password == "****")
            {
                pwbPassword.Password = "";
            }
            ((PasswordBox)sender).Foreground = new SolidColorBrush(Colors.Black);
        }

        /// <summary>
        /// Ip address to server field got focus, remove standardvalue
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbxIpAddress_GotFocus(object sender, RoutedEventArgs e)
        {
            if (((TextBox)sender).Text == "Localhost")
            {
                tbxIpAddress.Text = "";
            }
            ((TextBox)sender).Foreground = new SolidColorBrush(Colors.Black);
        }
        /// <summary>
        /// Ip address to server field lost focus, set standardvalues for box if nessecery
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbxIpAddress_LostFocus(object sender, RoutedEventArgs e)
        {
            if (((TextBox)sender).Text == "")
            {
                ((TextBox)sender).Foreground = new SolidColorBrush(Colors.LightGray);
                tbxIpAddress.Text = "Localhost";
            }
        }
        /// <summary>
        /// Check if it was a number or dot
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbxIpAddress_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = !(e.Key >= Key.D0 && e.Key <= Key.D9 ||
               e.Key >= Key.NumPad0 && e.Key <= Key.NumPad9 ||
               e.Key == Key.Back || e.Key == Key.OemPeriod
               );
        }
        #endregion
        /// <summary>
        /// Checks if login is ok
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            //check if ip address is valid
            string ipAddress = tbxIpAddress.Text;
            IPAddress address;

            if (ipAddress == "Localhost")
            {
                //ip is automatically set for localhost
            }
            else if (IPAddress.TryParse(ipAddress, out address))
            {
                switch (address.AddressFamily)
                {
                    case System.Net.Sockets.AddressFamily.InterNetwork:

                        break;

                    default:
                        tblAuthentificationFailed.Text = "Falsches IP-Addressen Format";
                        return;
                }
            }
            else
            {
                tblAuthentificationFailed.Text = "Falsches IP-Addressen Format";
                return;
            }

#if COM
            try
            {
                Type comType = Type.GetTypeFromCLSID(new Guid("C45F55FC-76D5-4D30-A7D0-2DF66C22ED0D"), ipAddress, false);
                _ComSrv = (COMMyCasinoSrvLib.COMMyCasino)Activator.CreateInstance(comType);
            }
            catch(COMException ex)
            {
                tblAuthentificationFailed.Text = "COM Server nicht gefunden";
                return;
            }
#else
            try
            {
                _RemSrvMyCasinoLogin = _remSrvMyCasinoFactory.CreateChannel(new EndpointAddress("http://" + ipAddress + ":1200/MyCasinoWCF"));
            }
            catch (Exception ex)
            {
                MessageBox.Show("Fehler beim erstellen der Channel Factory!");
            }
#endif

#if COM

            short tmpUsertype;
            uint _sessionId;
            try
            {
                
                    _ComSrv.login(tbxUsername.Text, pwbPassword.Password, out _sessionId, out tmpUsertype, out _errMsg);
                this.NavigationService.Navigate(new GamingPage(_ComSrv, tbxUsername.Text, _sessionId, tmpUsertype));

            }
            catch (Exception ex)
            {
                if (ex is COMException)
                    _errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                else
                    _errMsg = "Unknown";

                tblAuthentificationFailed.Text = _errMsg;

                if(ex.HResult == -2147023174)
                {
                    System.Windows.Application.Current.Shutdown();
                }
            }
#else
            try
            {
                if (_RemSrvMyCasinoLogin.login(tbxUsername.Text, pwbPassword.Password, out _sessionId, out _userType, out _errMsg))

                {
                    this.NavigationService.Navigate(new GamingPage(_RemSrvMyCasinoLogin, tbxUsername.Text, _sessionId, _userType));
                    return;
                } 
                if (_errMsg == "WRONG_USERNAME_OR_PASSWORD")
                {
                    tblAuthentificationFailed.Text = "Falscher Benutzername oder Passwort";
                }
                else if (_errMsg == "OPERATOR_ALREADY_LOGGED_IN")
                {
                    tblAuthentificationFailed.Text = "Es ist bereits ein Betreiber eingeloggt";
                }
                else if (_errMsg == "ALREADY_LOGGED_IN")
                {
                    tblAuthentificationFailed.Text = "User ist bereits angemeldet";
                }
                else if (_errMsg == "OPERATOR_NOT_LOGGED_IN")
                {
                    MessageBox.Show("Betreiber nicht eingeloggt!");
                    this.NavigationService.Navigate(new GamingPage(_RemSrvMyCasinoLogin, tbxUsername.Text, _sessionId, _userType));
                    return;
                }
            }
            catch (EndpointNotFoundException)
            {
                MessageBox.Show("Fehler beim Login: Server nicht gefunden!");     
            }
            
            //If authentifications is ok, login to the next page
#endif
        }


    }
}
