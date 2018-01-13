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
        private TransportData myCasinoSvcHistory = new TransportData();
        public TransportData MyCasinoSvcHistory
        {
            get { return myCasinoSvcHistory; }
            set { myCasinoSvcHistory = value; }
        }
        public HistoryPage()
        {
            InitializeComponent();
            lbBetAmountList.Items.Add("stringtest");
            lbPayInList.SelectedItem=("test");
        }

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
        private void btnLogout_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                //if (_RemSrvMyCasino.logout(SessionId, out errMsg))
                //{
                //    // Close Client Connection
                //    if (_RemSrvMyCasino != null)
                //    {
                //        ((IClientChannel)_RemSrvMyCasino).Close();
                //        ((IDisposable)_RemSrvMyCasino).Dispose();
                //        _RemSrvMyCasino = null;
                //    }
                //    //TODO: go to login page 
                //    //System.Windows.Application.Current.Shutdown();
                //this.ShowNewDialog<LoginPage>(
                //    cp => { cp.MyCasinoSvcLogin = myCasinoSvcHistory; },
                //    //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                //    cp => { });
                //}
                //if (errMsg == "INVALID_SESSION_ID")
                //{
                //    MessageBox.Show("Ungültige ID!");
                //}
            }
            catch (Exception ex)
            {
                MessageBox.Show("Fehler beim Logout: " + ex.ToString());
            }
        }
        #endregion

        #region Fill listbox


        #endregion





        private async void BtnRefresh_Click(object sender, RoutedEventArgs e)
        {
            bool isFinished=false;
            double opMoney = 0;
            try
            {
                do
                {

                    var result = await myCasinoSvcHistory.MyCasinoSvc.GetTransactionAsyncTask(myCasinoSvcHistory.SessionId);
                    isFinished = result.isFinished;
                    if (result.errMsg == "INVALID_SESSION_ID")
                    {
                        MessageBox.Show("Ungültige ID!");
                    }
                    //transaction is deposit
                    if (result.isFinished == true) break;
                    if (result.transactionType == 0)
                    {
                        if (MyCasinoSvcHistory.UserType == 0)
                        {
                            double opMoneyTmp = 0;
                            if (opMoney == 0)
                            {
                                double.TryParse(result.transaction.ElementAt(1), out opMoneyTmp);
                            }
                            else
                            {
                                double.TryParse(result.transaction.ElementAt(2), out opMoneyTmp);
                            }
                            opMoney += opMoneyTmp;
                            lbBalanceList.Items.Add(opMoney);
                        }
                        else
                        {
                            lbBalanceList.Items.Add(result.transaction.ElementAt(1));
                        }
                        lbPayInList.Items.Add(result.transaction.ElementAt(2));
                        lbBetAmountList.Items.Add("");
                        lbFirstNumberPerRollList.Items.Add("");
                        lbSecondNumberPerRollList.Items.Add("");
                        lbWinLossList.Items.Add("");

                    }
                    //transaction is win
                    else if (result.transactionType == 4)
                    {
                        int idTrans;
                        int.TryParse(result.transaction.ElementAt(0), out idTrans);
                        var resultTransInfo = await myCasinoSvcHistory.MyCasinoSvc.GetTransactionInformationAsyncTask(myCasinoSvcHistory.SessionId, idTrans);
                        if (resultTransInfo.errMsg == "INVALID_SESSION_ID")
                        {
                            MessageBox.Show("Ungültige ID!");
                        }
                        else if (resultTransInfo.errMsg != null)
                        {
                            MessageBox.Show("Fehler beim abholen der Informationen für die Transaktionen: ");
                        }
                        if (MyCasinoSvcHistory.UserType == 1)
                        {
                            lbBetAmountList.Items.Add(resultTransInfo.information.ElementAt(3));
                            lbFirstNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(1));
                            lbSecondNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(2));
                            lbWinLossList.Items.Add(resultTransInfo.information.ElementAt(6));
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(result.transaction.ElementAt(1));
                        }
                        else if (MyCasinoSvcHistory.UserType == 0)
                        {
                            int amount;
                            int.TryParse((resultTransInfo.information.ElementAt(6)), out amount);
                            opMoney = opMoney - amount;
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
                        var resultTransInfo = await myCasinoSvcHistory.MyCasinoSvc.GetTransactionInformationAsyncTask(myCasinoSvcHistory.SessionId, idTrans);
                        if (resultTransInfo.errMsg == "INVALID_SESSION_ID")
                        {
                            MessageBox.Show("Ungültige ID!");
                        }
                        else if (resultTransInfo.errMsg != null)
                        {
                            MessageBox.Show("Fehler beim abholen der Informationen für die Transaktionen: ");
                        }

                        if (MyCasinoSvcHistory.UserType == 0)
                        {
                            int amount;
                            int.TryParse((resultTransInfo.information.ElementAt(3)), out amount);
                            lbBetAmountList.Items.Add(resultTransInfo.information.ElementAt(3));
                            lbFirstNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(1));
                            lbSecondNumberPerRollList.Items.Add(resultTransInfo.information.ElementAt(2));
                            lbWinLossList.Items.Add((amount * (-1)).ToString());
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(result.transaction.ElementAt(1));
                        }
                        else if (MyCasinoSvcHistory.UserType == 0)
                        {
                            int amount;
                            int.TryParse((resultTransInfo.information.ElementAt(3)), out amount);
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
            catch (Exception ex)
            {
                MessageBox.Show("Fehler beim abholen der Transaktionen: " + ex);
            }
        }
             
    }
}