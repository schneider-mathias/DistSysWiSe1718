/************************************************************/
/* Content:   History Page code-behind                      */
/* Autor:     Martin Obermeier                              */
/* Time:      23. Jan 2018                                  */
/************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using DialogExtensions;
using System.Threading.Tasks;

namespace MyCasinoWSPhoneClient
{
    public partial class HistoryPage : PhoneApplicationPage
    {
        #region Data
        private TransportData myCasinoSvcHistory = new TransportData();
        public TransportData MyCasinoSvcHistory
        {
            get { return myCasinoSvcHistory; }
            set { myCasinoSvcHistory = value; }
        }

        #endregion

        #region Constructor
        public HistoryPage()
        {
            InitializeComponent();
        }

        #endregion

        #region Page buttons
        private void BtnPlay_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<GamingPage>(
                    cp => { cp.MyCasinoSvcGaming = myCasinoSvcHistory; },
                    //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                    cp => { });
        }
        private void BtnHistory_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<HistoryPage>(
                     cp => { cp.MyCasinoSvcHistory = myCasinoSvcHistory; },
                     //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                     cp => { });
        }
        private void BtnPayIn_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<PayInPage>(
                    cp => { cp.MyCasinoSvcPayIn = myCasinoSvcHistory; },
                    //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                    cp => { });
        }

        private async void btnLogout_Click(object sender, RoutedEventArgs e)
        {
            var result = await myCasinoSvcHistory.MyCasinoSvc.LogoutAsyncTask(myCasinoSvcHistory.SessionId);

            if (result.errMsg == "S_OK" || result.errMsg == "OPERATOR_NOT_LOGGED_IN")
            {
                if (result.errMsg == "OPERATOR_NOT_LOGGED_IN")
                {
                    MessageBox.Show("Betreiber nicht eingelogged");
                }
                this.ShowNewDialog<LoginPage>(
                  cp => { cp.MyCasinoSvcLogin = myCasinoSvcHistory; },
                  //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                  cp => { });
            }
            else if (result.errMsg == "INVALID_SESSION_ID")
            {
                MessageBox.Show("Ungültige ID! Schließe App");
                this.ShowNewDialog<LoginPage>(
                  cp => { cp.MyCasinoSvcLogin = myCasinoSvcHistory; },
                  //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                  cp => { });
            }
            else if (result.errMsg != null)
            {
                MessageBox.Show("Fehler beim Logout");
            }
            
        }

        #endregion

        #region Fill listbox

        private async void BtnRefresh_Click(object sender, RoutedEventArgs e)
        {
            //clear listboxes
            lbBetAmountList.Items.Clear();
            lbFirstNumberPerRollList.Items.Clear();
            lbSecondNumberPerRollList.Items.Clear();
            lbWinLossList.Items.Clear();
            lbPayInList.Items.Clear();
            lbBalanceList.Items.Clear();

            bool operatorLogged = true;
            bool isFinished = false;
            double opMoney = 0;
            try
            {
                do
                {
                    //make asynchron call to GetTransaction, but wait till it is synchronized
                    var result = await myCasinoSvcHistory.MyCasinoSvc.GetTransactionAsyncTask(myCasinoSvcHistory.SessionId);
                    isFinished = result.isFinished;
                    if (result.errMsg == "INVALID_SESSION_ID")
                    {
                        MessageBox.Show("Ungültige ID!");
                    }
                    else if (result.errMsg == "OPERATOR_NOT_LOGGED_IN")
                    {
                        operatorLogged = false;
                    }
                    //transaction is deposit -> save all elements for listboxes
                    if (result.isFinished == true) break;
                    if (result.transactionType == 0)
                    {
                        if (MyCasinoSvcHistory.UserType == 0)
                        {
                            double opMoneyTmp = 0;
                            if (opMoney == 0)
                            {
                                double.TryParse(result.transaction.ElementAt(1).Replace(',', '.'), out opMoneyTmp);
                            }
                            else
                            {
                                double.TryParse(result.transaction.ElementAt(2).Replace(',', '.'), out opMoneyTmp);
                            }
                            opMoney += opMoneyTmp;
                            lbBalanceList.Items.Add(string.Format("{0:N2}", opMoney));
                        }
                        else
                        {
                            lbBalanceList.Items.Add(result.transaction.ElementAt(1));
                        }
                        lbPayInList.Items.Add(result.transaction.ElementAt(2).Replace(',', '.'));
                        lbBetAmountList.Items.Add("");
                        lbFirstNumberPerRollList.Items.Add("");
                        lbSecondNumberPerRollList.Items.Add("");
                        lbWinLossList.Items.Add("");
                    }
                    //transaction is win -> save all elements for listboxes
                    else if (result.transactionType == 4)
                    {
                        int idTrans;
                        int.TryParse(result.transaction.ElementAt(0), out idTrans);
                        //make asynchron call to GetTransactionInformation, but wait till it is synchronized
                        var resultTransInfo = await myCasinoSvcHistory.MyCasinoSvc.GetTransactionInformationAsyncTask(myCasinoSvcHistory.SessionId, idTrans);
                        if (resultTransInfo.errMsg == "INVALID_SESSION_ID")
                        {
                            MessageBox.Show("Ungültige ID!");
                        }
                        else if (resultTransInfo.errMsg != null)
                        {
                            MessageBox.Show("Fehler beim abholen der Informationen für die Transaktionen!");
                        }
                        //save all transaction information to the listboxes for gamer
                        if (MyCasinoSvcHistory.UserType == 1)
                        {
                            lbBetAmountList.Items.Add(resultTransInfo.information.ElementAt(3).Replace(',', '.'));
                            lbFirstNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(1));
                            lbSecondNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(2));
                            lbWinLossList.Items.Add(resultTransInfo.information.ElementAt(6).Replace(',', '.'));
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(result.transaction.ElementAt(1).Replace(',', '.'));
                        }
                        //save all transaction information to the listboxes for operator
                        else if (MyCasinoSvcHistory.UserType == 0)
                        {
                            double amount;
                            double.TryParse((resultTransInfo.information.ElementAt(6).Replace(',', '.')), out amount);
                            opMoney = opMoney - (amount/2);
                            lbBetAmountList.Items.Add(resultTransInfo.information.ElementAt(3));
                            lbFirstNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(1));
                            lbSecondNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(2));
                            lbWinLossList.Items.Add(((amount * (-1)).ToString()));
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(opMoney);
                        }
                    }
                    //transaction is loss
                    else if (result.transactionType == 5)
                    {
                        int idTrans;
                        int.TryParse(result.transaction.ElementAt(0), out idTrans);
                        //make asynchron call to GetTransactionInformation, but wait till it is synchronized
                        var resultTransInfo = await myCasinoSvcHistory.MyCasinoSvc.GetTransactionInformationAsyncTask(myCasinoSvcHistory.SessionId, idTrans);
                        if (resultTransInfo.errMsg == "INVALID_SESSION_ID")
                        {
                            MessageBox.Show("Ungültige ID!");
                        }
                        else if (resultTransInfo.errMsg != null)
                        {
                            MessageBox.Show("Fehler beim abholen der Informationen für die Transaktionen");
                        }
                        //save all transaction information to the listboxes for gamer
                        if (MyCasinoSvcHistory.UserType == 1)
                        {
                            double amount;
                            double.TryParse((resultTransInfo.information.ElementAt(3)), out amount);
                            lbBetAmountList.Items.Add(resultTransInfo.information.ElementAt(3));
                            lbFirstNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(1));
                            lbSecondNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(2));
                            lbWinLossList.Items.Add((amount * (-1)).ToString());
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(result.transaction.ElementAt(1));
                        }
                        //save all transaction information to the listboxes for operator
                        else if (MyCasinoSvcHistory.UserType == 0)
                        {
                            double amount;
                            double.TryParse((resultTransInfo.information.ElementAt(3)), out amount);
                            opMoney = opMoney + amount;
                            lbBetAmountList.Items.Add(resultTransInfo.information.ElementAt(3));
                            lbFirstNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(1));
                            lbSecondNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(2));
                            lbWinLossList.Items.Add((amount).ToString());
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(opMoney);
                        }
                    }
                }
                while (isFinished != true);
                opMoney = 0;
            }
            catch (Exception)
            {
                MessageBox.Show("Fehler beim abholen der Transaktionen");
            }
            if (operatorLogged == false)
            {
                MessageBox.Show("Kein Betreiber angemeldet");
            }
        }


        #endregion

        #region Page loaded
        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            BtnRefresh_Click(new object(), new RoutedEventArgs());
            if (MyCasinoSvcHistory.UserType == 1)
            {

            }
            else if (MyCasinoSvcHistory.UserType == 0)
            {
                BtnPayIn.Visibility = Visibility.Visible;

            }
        }
        #endregion
    }
}