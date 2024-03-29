﻿/************************************************************/
/* Content:   Gaming Page code-behind                       */
/* Autor:     Martin Obermeier                              */
/* Time:      23. Jan 2018                                  */
/************************************************************/

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
    /// Interaction logic for GamingPage.xaml
    /// </summary>
    public partial class GamingPage : Page
    {
        private string errMsg;

        private string username;

        public string Username
        {
            get { return username; }
            set { username = value; }
        }

        private short userType;

        public short UserType
        {
            get { return userType; }
            set { userType = value; }
        }

        private ulong sessionId;

        public ulong SessionId
        {
            get { return sessionId; }
            set { sessionId = value; }
        }
#if COM


        private COMMyCasinoSrvLib.COMMyCasino _comSrv;

        public COMMyCasinoSrvLib.COMMyCasino _ComSrv
        {
            get { return _comSrv; }
            set { _comSrv = value; }
        }

       
        public GamingPage(COMMyCasinoSrvLib.COMMyCasino _comSrvTmp, string usernameTmp, ulong sessionIdTmp, short typeTmp)
        {
            InitializeComponent();
            _ComSrv = _comSrvTmp;
            Username = usernameTmp;
            SessionId = sessionIdTmp;
            UserType = typeTmp;

            //start with playpage
            PlayHistoryPage.Content = new PlayPage(_ComSrv, Username, SessionId, UserType);
            //set username in gamingpage
            tblUsername.Text = Username;

            //disable gamer-only content TODO: check numbers
            if (UserType == 0)
            {

            }
            //disable operator-only content
            if (UserType == 1)
            {
                BtnPayIn.Visibility = Visibility.Hidden;
            }
        }
#else

        private INETMyCasino _remSrvMyCasino;
        public INETMyCasino _RemSrvMyCasino
        {
            get { return _remSrvMyCasino; }
            set { _remSrvMyCasino = value; }
        }
        public GamingPage(INETMyCasino _RemSrvMyCasinoMain, string usernameTmp, ulong sessionIdTmp, short typeTmp)
        {
            InitializeComponent();
            //save information for different pages
            Username = usernameTmp;
            SessionId = sessionIdTmp;
            UserType = typeTmp;
            _RemSrvMyCasino = _RemSrvMyCasinoMain;
            //start with playpage
            PlayHistoryPage.Content = new PlayPage(_RemSrvMyCasino, Username, SessionId, UserType);
            //set username in gamingpage
            tblUsername.Text = Username;

            //disable gamer-only content
            if (UserType == 0)
            {

            }
            //disable operator-only content
            if (UserType == 1)
            {
                BtnPayIn.Visibility = Visibility.Hidden;
            }
        }
#endif
        private void BtnPlay_Click(object sender, RoutedEventArgs e)
        {
#if COM
            PlayHistoryPage.Content = new PlayPage(_ComSrv, Username, SessionId, UserType);
#else
            PlayHistoryPage.Content = new PlayPage(_RemSrvMyCasino, Username, SessionId, UserType);
#endif
        }
        private void BtnHistory_Click(object sender, RoutedEventArgs e)
        {
#if COM
            PlayHistoryPage.Content = new HistoryPage(_ComSrv, Username, SessionId, UserType);
#else
            PlayHistoryPage.Content = new HistoryPage(_RemSrvMyCasino, Username, SessionId, UserType);
#endif
        }
        private void BtnPayIn_Click(object sender, RoutedEventArgs e)
        {
#if COM
            PlayHistoryPage.Content = new PayInPage(_ComSrv, Username, SessionId, UserType);
#else
            PlayHistoryPage.Content = new PayInPage(_RemSrvMyCasino, Username, SessionId, UserType);
#endif
        }
        private void btnLogout_Click(object sender, RoutedEventArgs e)
        {
#if COM
            try
            {
                _ComSrv.logout((uint)SessionId, out errMsg);
                if (errMsg != null)
                {
                    MessageBox.Show("No operator is logged in!");
                }

                System.Windows.Application.Current.Shutdown();
            }
           catch (Exception ex)
            {
                if (ex is COMException)
                    errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                else
                    errMsg = "Unknown";

                MessageBox.Show(errMsg);

                if(ex.HResult == -2147023174)
                {
                    System.Windows.Application.Current.Shutdown();
                }
            }
#else
            try
            {
                if (_RemSrvMyCasino.logout(SessionId, out errMsg))
                {
                    if(errMsg== "OPERATOR_NOT_LOGGED_IN")
                    {
                        MessageBox.Show("Betreiber nicht eingelogged");
                    }
                    // Close Client Connection
                    if (_RemSrvMyCasino != null)
                    {
                        ((IClientChannel)_RemSrvMyCasino).Close();
                        ((IDisposable)_RemSrvMyCasino).Dispose();
                        _RemSrvMyCasino = null;
                    }
                    System.Windows.Application.Current.Shutdown();
                }
                if (errMsg == "INVALID_SESSION_ID")
                {
                    MessageBox.Show("Ungültige ID!");
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("Fehler beim Logout: Server nicht gefunden!");
            }
#endif
        }
    }
}
