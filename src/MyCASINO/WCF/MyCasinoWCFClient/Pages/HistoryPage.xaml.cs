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
    /// Interaction logic for HistoryPage.xaml
    /// </summary>
    public partial class HistoryPage : Page
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
#endif
        //0 dep 1 with 2canc 3betwa 4win 5loss
#if COM
        public HistoryPage(COMMyCasinoSrvLib.COMMyCasino _comSrvTmp, string usernameTmp, uint sessionIdTmp, short typeTmp)
        {
            _ComSrv = _comSrvTmp;
            InitializeComponent();
            //Init
            SessionId = sessionIdTmp;
            string errMsg = null;
            int isFinished;
            System.Array transaction = null;
            System.Array information = null;
            uint informationType;
            uint transactionType;
            
            try
            {
                do
                {
                    _ComSrv.getTransactions(SessionId, out isFinished, out transaction, out transactionType, out errMsg);
                    //transaction is deposit
                    if (transactionType == 0)
                    {
                        lbBetAmountList.Items.Add("");
                        lbFirstNumberPerRollList.Items.Add("");
                        lbSecondNumberPerRollList.Items.Add("");
                        lbFirstNumberDrawn.Items.Add("");
                        lbSecondNumberDrawn.Items.Add("");
                        lbWinLossList.Items.Add("");
                        lbPayInList.Items.Add(transaction.GetValue(2));
                        lbBalanceList.Items.Add(transaction.GetValue(1));
                        
                    }
                    //transaction is win
                    else if (transactionType == 4)
                    {
                        try
                        {
                            _ComSrv.getTransactionInformation(SessionId,(uint)transaction.GetValue(0), out information, out informationType,out errMsg);
                        }
                        catch (Exception ex)
                        {
                            if (ex is COMException)
                                errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                            else
                                errMsg = "Unknown";
                        }
                        if (typeTmp == 1)
                        {
                            lbBetAmountList.Items.Add(information.GetValue(3));
                            lbFirstNumberPerRollList.Items.Add(information.GetValue(1));
                            lbSecondNumberPerRollList.Items.Add(information.GetValue(2));
                            lbFirstNumberDrawn.Items.Add(information.GetValue(4));
                            lbSecondNumberDrawn.Items.Add(information.GetValue(5));
                            lbWinLossList.Items.Add(information.GetValue(6));
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(transaction.GetValue(1));
                        }
                        else if (typeTmp == 0)
                        {
                            lbBetAmountList.Items.Add(information.GetValue(3));
                            lbFirstNumberPerRollList.Items.Add(information.GetValue(1));
                            lbSecondNumberPerRollList.Items.Add(information.GetValue(2));
                            lbFirstNumberDrawn.Items.Add(information.GetValue(4));
                            lbSecondNumberDrawn.Items.Add(information.GetValue(5));
                            lbWinLossList.Items.Add(((double)information.GetValue(3)).ToString());
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(transaction.GetValue(1));
                        }
                    }
                    //transaction is loss
                    else if (transactionType == 5)
                    {
                        try
                        {
                            _ComSrv.getTransactionInformation(SessionId, (uint)transaction.GetValue(0), out information, out informationType, out errMsg);
                        }
                        catch (Exception ex)
                        {
                            if (ex is COMException)
                                errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                            else
                            errMsg = "Unknown";
                        }
                        if (typeTmp == 1)
                        {
                            lbBetAmountList.Items.Add(information.GetValue(3));
                            lbFirstNumberPerRollList.Items.Add(information.GetValue(1));
                            lbSecondNumberPerRollList.Items.Add(information.GetValue(2));
                            lbFirstNumberDrawn.Items.Add(information.GetValue(4));
                            lbSecondNumberDrawn.Items.Add(information.GetValue(5));
                            lbWinLossList.Items.Add(((double)information.GetValue(3) * (-1)).ToString());
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(transaction.GetValue(1));
                        }
                        else if (typeTmp == 0)
                        {
                            lbBetAmountList.Items.Add(information.GetValue(3));
                            lbFirstNumberPerRollList.Items.Add(information.GetValue(1));
                            lbSecondNumberPerRollList.Items.Add(information.GetValue(2));
                            lbFirstNumberDrawn.Items.Add(information.GetValue(4));
                            lbSecondNumberDrawn.Items.Add(information.GetValue(5));
                            lbWinLossList.Items.Add(((double)information.GetValue(6) * (-1)).ToString());
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(transaction.GetValue(1));
                        }
                    }
                }
                while (isFinished != 1);
                
            }
            catch (Exception ex)
            {
                if (ex is COMException)
                    errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                else
                    errMsg = "Unknown";
            }

            
                  
       }
#else
        public HistoryPage(INETMyCasino _RemSrvMyCasinoMain, string usernameTmp, int sessionIdTmp, MyCasinoUserTypes typeTmp)
        {
            _RemSrvMyCasino = _RemSrvMyCasinoMain;
            InitializeComponent();
            //Init
            SessionId = sessionIdTmp;
            bool isFinished;
            List<string> transaction;
            List<string> information = null;
            int informationType;
            int transactionType;
            string errMsg="";
            double opMoney=0;
            try
            {
                do
                {
                    _RemSrvMyCasino.getTransactions(SessionId, out isFinished, out transaction, out transactionType, out errMsg);
                    if (errMsg == "INVALID_SESSION_ID")
                    {
                        MessageBox.Show("Ungültige ID!");
                    }
                    //transaction is deposit
                    if (isFinished == true) break;
                    if (transactionType == 0)
                    {
                        if (typeTmp == MyCasinoUserTypes.Operator)
                        {
                            double opMoneyTmp = 0;
                            if (opMoney == 0)
                            {
                                double.TryParse(transaction.ElementAt(1), out opMoneyTmp);
                            }
                            else
                            {
                                double.TryParse(transaction.ElementAt(2), out opMoneyTmp);
                            }
                            opMoney += opMoneyTmp;
                            lbBalanceList.Items.Add(opMoney);
                        }
                        else
                        {
                            lbBalanceList.Items.Add(transaction.ElementAt(1));
                        }
                        lbPayInList.Items.Add(transaction.ElementAt(2));
                        lbBetAmountList.Items.Add("");
                        lbFirstNumberPerRollList.Items.Add("");
                        lbSecondNumberPerRollList.Items.Add("");
                        lbFirstNumberDrawn.Items.Add("");
                        lbSecondNumberDrawn.Items.Add("");
                        lbWinLossList.Items.Add("");

                    }
                    //transaction is win
                    else if (transactionType == 4)
                    {
                        try
                        {
                            int idTrans;
                            int.TryParse(transaction.ElementAt(0), out idTrans);
                            _RemSrvMyCasino.getTransactionInformation(SessionId, idTrans, out information, out informationType, out errMsg);
                            if (errMsg == "INVALID_SESSION_ID")
                            {
                                MessageBox.Show("Ungültige ID!");
                            }
                        }
                        catch(Exception ex)
                        {
                            MessageBox.Show("Fehler beim abholen der Informationen für die Transaktionen: "+ ex);
                        }
                        if (typeTmp == MyCasinoUserTypes.Gamer)
                        {
                            lbBetAmountList.Items.Add(information.ElementAt(3));
                            lbFirstNumberPerRollList.Items.Add(information.ElementAt(1));
                            lbSecondNumberPerRollList.Items.Add(information.ElementAt(2));
                            lbFirstNumberDrawn.Items.Add(information.ElementAt(4));
                            lbSecondNumberDrawn.Items.Add(information.ElementAt(5));
                            lbWinLossList.Items.Add(information.ElementAt(6));
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(transaction.ElementAt(1));
                        }
                        else if (typeTmp == MyCasinoUserTypes.Operator)
                        {
                            int amount;
                            int.TryParse((information.ElementAt(6)), out amount);
                            opMoney = opMoney - amount;
                            lbBetAmountList.Items.Add(information.ElementAt(3));
                            lbFirstNumberPerRollList.Items.Add(information.ElementAt(1));
                            lbSecondNumberPerRollList.Items.Add(information.ElementAt(2));
                            lbFirstNumberDrawn.Items.Add(information.ElementAt(4));
                            lbSecondNumberDrawn.Items.Add(information.ElementAt(5));
                            lbWinLossList.Items.Add(((amount * (-1)).ToString()));
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(opMoney);
                        }
                    }
                    //transaction is loss
                    else if (transactionType == 5)
                    {
                        try
                        {
                            int idTrans;
                            int.TryParse(transaction.ElementAt(0), out idTrans);
                            _RemSrvMyCasino.getTransactionInformation(SessionId, idTrans, out information, out informationType, out errMsg);
                            if (errMsg == "INVALID_SESSION_ID")
                            {
                                MessageBox.Show("Ungültige ID!");
                            }
                        }
                        catch(Exception ex)
                        {
                            MessageBox.Show("Fehler beim abholen der Informationen für die Transaktionen: " + ex);
                        }
                        if (typeTmp == MyCasinoUserTypes.Gamer)
                        {
                            int amount;
                            int.TryParse((information.ElementAt(3)),out amount);
                            lbBetAmountList.Items.Add(information.ElementAt(3));
                            lbFirstNumberPerRollList.Items.Add(information.ElementAt(1));
                            lbSecondNumberPerRollList.Items.Add(information.ElementAt(2));
                            lbFirstNumberDrawn.Items.Add(information.ElementAt(4));
                            lbSecondNumberDrawn.Items.Add(information.ElementAt(5));
                            lbWinLossList.Items.Add((amount * (-1)).ToString());
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(transaction.ElementAt(1));
                        }
                        else if (typeTmp == MyCasinoUserTypes.Operator)
                        {
                            int amount;
                            int.TryParse((information.ElementAt(3)), out amount);
                            opMoney = opMoney + amount;
                            lbBetAmountList.Items.Add(information.ElementAt(3));
                            lbFirstNumberPerRollList.Items.Add(information.ElementAt(1));
                            lbSecondNumberPerRollList.Items.Add(information.ElementAt(2));
                            lbFirstNumberDrawn.Items.Add(information.ElementAt(4));
                            lbSecondNumberDrawn.Items.Add(information.ElementAt(5));
                            lbWinLossList.Items.Add((amount).ToString());
                            lbPayInList.Items.Add("");
                            lbBalanceList.Items.Add(opMoney);
                        }
                    }
                }
                while (isFinished != true);
                opMoney = 0;
            }
            catch(Exception ex)
            {
                MessageBox.Show("Fehler beim abholen der Transaktionen: " + ex);
            }
        }
#endif
    }

}
