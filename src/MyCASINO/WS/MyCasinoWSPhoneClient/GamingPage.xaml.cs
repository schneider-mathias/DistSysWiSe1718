using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using System.Windows.Input;
using DialogExtensions;

namespace MyCasinoWSPhoneClient
{
    public partial class GamingPage : PhoneApplicationPage
    {
        private TransportData myCasinoSvcGaming = new TransportData();
        public TransportData MyCasinoSvcGaming
        {
            get { return myCasinoSvcGaming; }
            set { myCasinoSvcGaming = value; }
        }

        private string errMsg;

        private int numberOne;
        public int NumberOne
        {
            get { return numberOne; }
            set { numberOne = value; }
        }

        private int numberTwo;

        public int NumberTwo
        {
            get { return numberTwo; }
            set { numberTwo = value; }
        }

        //First drawn number
        private int firstNumber;

        public int FirstNumber
        {
            get { return firstNumber; }
            set { firstNumber = value; }
        }

        //Second drawn Number
        private int secondNumber;

        public int SecondNumber
        {
            get { return secondNumber; }
            set { secondNumber = value; }
        }

        public GamingPage()
        {
            InitializeComponent();
            //tblUsername.Text = MyCasinoSvcGaming.UserName;

            ////disable gamer-only content
            //if (MyCasinoSvcGaming.UserType == 0)
            //{
            //    btnDraw.Visibility = Visibility.Collapsed;
            //    btnDrawNumbers.Visibility = Visibility.Collapsed;
            //    //tblLastDrawing.Visibility = Visibility.Collapsed;
            //}
            ////disable operator-only content
            //if (MyCasinoSvcGaming.UserType == 1)
            //{
            //    tbxBetSum.Visibility = Visibility.Collapsed;
            //    btnBet.Visibility = Visibility.Collapsed;
            //    tblSetAmount.Visibility = Visibility.Collapsed;
            //    BtnPayIn.Visibility = Visibility.Collapsed;
            //}
        }

        #region Page buttons
        private void BtnPlay_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<GamingPage>(
                    cp => { cp.MyCasinoSvcGaming = myCasinoSvcGaming; },
                    //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                    cp => { });
        }
        private void BtnHistory_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<HistoryPage>(
                     cp => { cp.MyCasinoSvcHistory = myCasinoSvcGaming; },
                     //cp => { MyCasinoSvcLogin = cp.MyCasinoSvcGamingPage; });
                     cp => { });
        }
        private void BtnPayIn_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<PayInPage>(
                    cp => { cp.MyCasinoSvcPayIn = myCasinoSvcGaming; },
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
                //    cp => { cp.MyCasinoSvcLogin = myCasinoSvcGaming; },
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

        #region Number buttons
        private void btnRowOneNumberOne_Click(object sender, RoutedEventArgs e)
        {
            //Check if another number is pressed already
            if (btnRowOneNumberTwo.IsChecked.Value || btnRowOneNumberThree.IsChecked.Value || btnRowOneNumberFour.IsChecked.Value || btnRowOneNumberFive.IsChecked.Value)
            {
                btnRowOneNumberTwo.IsChecked = false;
                btnRowOneNumberThree.IsChecked = false;
                btnRowOneNumberFour.IsChecked = false;
                btnRowOneNumberFive.IsChecked = false;
            }

            //Enable row2 buttons if one button from row 1 is active
            if (btnRowOneNumberOne.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsEnabled = false;
                btnRowTwoNumberTwo.IsEnabled = true;
                btnRowTwoNumberThree.IsEnabled = true;
                btnRowTwoNumberFour.IsEnabled = true;
                btnRowTwoNumberFive.IsEnabled = true;
            }

            //If no button in row1 is active, disable all row2 buttons
            if (!btnRowOneNumberOne.IsChecked.Value && !btnRowOneNumberTwo.IsChecked.Value && !btnRowOneNumberThree.IsChecked.Value
                && !btnRowOneNumberFour.IsChecked.Value && !btnRowOneNumberFive.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsEnabled = false;
                btnRowTwoNumberTwo.IsEnabled = false;
                btnRowTwoNumberThree.IsEnabled = false;
                btnRowTwoNumberFour.IsEnabled = false;
                btnRowTwoNumberFive.IsEnabled = false;

                btnRowTwoNumberOne.IsChecked = false;
                btnRowTwoNumberTwo.IsChecked = false;
                btnRowTwoNumberThree.IsChecked = false;
                btnRowTwoNumberFour.IsChecked = false;
                btnRowTwoNumberFive.IsChecked = false;
            }

            //set property for bet
            NumberOne = 1;

            //If no button is active, disable the betbutton
            if (!btnRowOneNumberOne.IsChecked.Value && !btnRowOneNumberTwo.IsChecked.Value && !btnRowOneNumberThree.IsChecked.Value
                && !btnRowOneNumberFour.IsChecked.Value && !btnRowOneNumberFive.IsChecked.Value)
            {
                btnBet.IsEnabled = false;
            }

            //If one button is active and a betamout is entered, enable the bet button
            if ((btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value
                || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value) && tbxBetSum.Text.ToString() != "")
            {
                btnBet.IsEnabled = true;
            }
        }

        private void btnRowOneNumberTwo_Click(object sender, RoutedEventArgs e)
        {
            //Check if another number is pressed already
            if (btnRowOneNumberOne.IsChecked.Value || btnRowOneNumberThree.IsChecked.Value || btnRowOneNumberFour.IsChecked.Value || btnRowOneNumberFive.IsChecked.Value)
            {
                btnRowOneNumberOne.IsChecked = false;
                btnRowOneNumberThree.IsChecked = false;
                btnRowOneNumberFour.IsChecked = false;
                btnRowOneNumberFive.IsChecked = false;
            }

            //Enable row2 buttons if one button from row 1 is active and disable specific buttons in row 2
            if (btnRowOneNumberTwo.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsEnabled = false;
                btnRowTwoNumberTwo.IsEnabled = false;
                btnRowTwoNumberThree.IsEnabled = true;
                btnRowTwoNumberFour.IsEnabled = true;
                btnRowTwoNumberFive.IsEnabled = true;
            }

            //set property for bet
            NumberOne = 2;

            //If no button in row1 is active, disable all row2 buttons
            if (!btnRowOneNumberOne.IsChecked.Value && !btnRowOneNumberTwo.IsChecked.Value && !btnRowOneNumberThree.IsChecked.Value
                && !btnRowOneNumberFour.IsChecked.Value && !btnRowOneNumberFive.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsEnabled = false;
                btnRowTwoNumberTwo.IsEnabled = false;
                btnRowTwoNumberThree.IsEnabled = false;
                btnRowTwoNumberFour.IsEnabled = false;
                btnRowTwoNumberFive.IsEnabled = false;

                btnRowTwoNumberOne.IsChecked = false;
                btnRowTwoNumberTwo.IsChecked = false;
                btnRowTwoNumberThree.IsChecked = false;
                btnRowTwoNumberFour.IsChecked = false;
                btnRowTwoNumberFive.IsChecked = false;

            }

            //If no button is active, disable the betbutton
            if (!btnRowOneNumberOne.IsChecked.Value && !btnRowOneNumberTwo.IsChecked.Value && !btnRowOneNumberThree.IsChecked.Value
                && !btnRowOneNumberFour.IsChecked.Value && !btnRowOneNumberFive.IsChecked.Value)
            {
                btnBet.IsEnabled = false;
            }

            //If one button is active and a betamout is entered, enable the bet button
            if ((btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value
                || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value) && tbxBetSum.Text.ToString() != "")
            {
                btnBet.IsEnabled = true;
            }
        }

        private void btnRowOneNumberThree_Click(object sender, RoutedEventArgs e)
        {
            //Check if another number is pressed already
            if (btnRowOneNumberOne.IsChecked.Value || btnRowOneNumberTwo.IsChecked.Value || btnRowOneNumberFour.IsChecked.Value || btnRowOneNumberFive.IsChecked.Value)
            {
                btnRowOneNumberOne.IsChecked = false;
                btnRowOneNumberTwo.IsChecked = false;
                btnRowOneNumberFour.IsChecked = false;
                btnRowOneNumberFive.IsChecked = false;
            }

            //Enable row2 buttons if one button from row 1 is active
            if (btnRowOneNumberThree.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsEnabled = false;
                btnRowTwoNumberTwo.IsEnabled = false;
                btnRowTwoNumberThree.IsEnabled = false;
                btnRowTwoNumberFour.IsEnabled = true;
                btnRowTwoNumberFive.IsEnabled = true;
            }

            //If no button in row1 is active, disable all row2 buttons
            if (!btnRowOneNumberOne.IsChecked.Value && !btnRowOneNumberTwo.IsChecked.Value && !btnRowOneNumberThree.IsChecked.Value
                && !btnRowOneNumberFour.IsChecked.Value && !btnRowOneNumberFive.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsEnabled = false;
                btnRowTwoNumberTwo.IsEnabled = false;
                btnRowTwoNumberThree.IsEnabled = false;
                btnRowTwoNumberFour.IsEnabled = false;
                btnRowTwoNumberFive.IsEnabled = false;

                btnRowTwoNumberOne.IsChecked = false;
                btnRowTwoNumberTwo.IsChecked = false;
                btnRowTwoNumberThree.IsChecked = false;
                btnRowTwoNumberFour.IsChecked = false;
                btnRowTwoNumberFive.IsChecked = false;
            }

            //set property for bet
            NumberOne = 3;

            //If no button is active, disable the betbutton
            if (!btnRowOneNumberOne.IsChecked.Value && !btnRowOneNumberTwo.IsChecked.Value && !btnRowOneNumberThree.IsChecked.Value
                && !btnRowOneNumberFour.IsChecked.Value && !btnRowOneNumberFive.IsChecked.Value)
            {
                btnBet.IsEnabled = false;
            }

            //If one button is active and a betamout is entered, enable the bet button
            if ((btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value
                || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value) && tbxBetSum.Text.ToString() != "")
            {
                btnBet.IsEnabled = true;
            }
        }

        private void btnRowOneNumberFour_Click(object sender, RoutedEventArgs e)
        {
            //Check if another number is pressed already
            if (btnRowOneNumberOne.IsChecked.Value || btnRowOneNumberTwo.IsChecked.Value || btnRowOneNumberThree.IsChecked.Value || btnRowOneNumberFive.IsChecked.Value)
            {
                btnRowOneNumberOne.IsChecked = false;
                btnRowOneNumberTwo.IsChecked = false;
                btnRowOneNumberThree.IsChecked = false;
                btnRowOneNumberFive.IsChecked = false;
            }

            //Enable row2 buttons if one button from row 1 is active
            if (btnRowOneNumberFour.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsEnabled = false;
                btnRowTwoNumberTwo.IsEnabled = false;
                btnRowTwoNumberThree.IsEnabled = false;
                btnRowTwoNumberFour.IsEnabled = false;
                btnRowTwoNumberFive.IsEnabled = true;
            }

            //If no button in row1 is active, disable all row2 buttons
            if (!btnRowOneNumberOne.IsChecked.Value && !btnRowOneNumberTwo.IsChecked.Value && !btnRowOneNumberThree.IsChecked.Value
                && !btnRowOneNumberFour.IsChecked.Value && !btnRowOneNumberFive.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsEnabled = false;
                btnRowTwoNumberTwo.IsEnabled = false;
                btnRowTwoNumberThree.IsEnabled = false;
                btnRowTwoNumberFour.IsEnabled = false;
                btnRowTwoNumberFive.IsEnabled = false;

                btnRowTwoNumberOne.IsChecked = false;
                btnRowTwoNumberTwo.IsChecked = false;
                btnRowTwoNumberThree.IsChecked = false;
                btnRowTwoNumberFour.IsChecked = false;
                btnRowTwoNumberFive.IsChecked = false;
            }
            //set property for bet
            NumberOne = 4;

            //If no button is active, disable the betbutton
            if (!btnRowOneNumberOne.IsChecked.Value && !btnRowOneNumberTwo.IsChecked.Value && !btnRowOneNumberThree.IsChecked.Value
                && !btnRowOneNumberFour.IsChecked.Value && !btnRowOneNumberFive.IsChecked.Value)
            {
                btnBet.IsEnabled = false;
            }

            //If one button is active and a betamout is entered, enable the bet button
            if ((btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value
                || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value) && tbxBetSum.Text.ToString() != "")
            {
                btnBet.IsEnabled = true;
            }
        }

        private void btnRowOneNumberFive_Click(object sender, RoutedEventArgs e)
        {
            //Check if another number is pressed already
            if (btnRowOneNumberOne.IsChecked.Value || btnRowOneNumberTwo.IsChecked.Value || btnRowOneNumberThree.IsChecked.Value || btnRowOneNumberFour.IsChecked.Value)
            {
                btnRowOneNumberOne.IsChecked = false;
                btnRowOneNumberTwo.IsChecked = false;
                btnRowOneNumberThree.IsChecked = false;
                btnRowOneNumberFour.IsChecked = false;
            }

            //If no button in row1 is active, disable all row2 buttons
            if (!btnRowOneNumberOne.IsChecked.Value && !btnRowOneNumberTwo.IsChecked.Value && !btnRowOneNumberThree.IsChecked.Value
                && !btnRowOneNumberFour.IsChecked.Value && !btnRowOneNumberFive.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsEnabled = false;
                btnRowTwoNumberTwo.IsEnabled = false;
                btnRowTwoNumberThree.IsEnabled = false;
                btnRowTwoNumberFour.IsEnabled = false;
                btnRowTwoNumberFive.IsEnabled = false;

                btnRowTwoNumberOne.IsChecked = false;
                btnRowTwoNumberTwo.IsChecked = false;
                btnRowTwoNumberThree.IsChecked = false;
                btnRowTwoNumberFour.IsChecked = false;
                btnRowTwoNumberFive.IsChecked = false;
            }

            //set property for bet
            NumberOne = 5;

            //If no button is active, disable the betbutton
            if (!btnRowOneNumberOne.IsChecked.Value && !btnRowOneNumberTwo.IsChecked.Value && !btnRowOneNumberThree.IsChecked.Value
                && !btnRowOneNumberFour.IsChecked.Value && !btnRowOneNumberFive.IsChecked.Value)
            {
                btnBet.IsEnabled = false;
            }

            //If one button is active and a betamout is entered, enable the bet button
            if ((btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value
                || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value) && tbxBetSum.Text.ToString() != "")
            {
                btnBet.IsEnabled = true;
            }
        }

        private void btnRowTwoNumberOne_Click(object sender, RoutedEventArgs e)
        {
            //Check if another number is pressed already
            if (btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value)
            {
                btnRowTwoNumberTwo.IsChecked = false;
                btnRowTwoNumberThree.IsChecked = false;
                btnRowTwoNumberFour.IsChecked = false;
                btnRowTwoNumberFive.IsChecked = false;
            }

            //set property for bet
            NumberTwo = 1;

            //If no button is active, disable the betbutton
            if (!btnRowTwoNumberOne.IsChecked.Value && !btnRowTwoNumberTwo.IsChecked.Value && !btnRowTwoNumberThree.IsChecked.Value
                && !btnRowTwoNumberFour.IsChecked.Value && !btnRowTwoNumberFive.IsChecked.Value)
            {
                btnBet.IsEnabled = false;
            }

            //If one button is active and a betamout is entered, enable the bet button
            if ((btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value
                || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value) && tbxBetSum.Text.ToString() != "")
            {
                btnBet.IsEnabled = true;
            }
        }

        private void btnRowTwoNumberTwo_Click(object sender, RoutedEventArgs e)
        {
            //Check if another number is pressed already
            if (btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsChecked = false;
                btnRowTwoNumberThree.IsChecked = false;
                btnRowTwoNumberFour.IsChecked = false;
                btnRowTwoNumberFive.IsChecked = false;
            }
            //set property for bet
            NumberTwo = 2;

            //If no button is active, disable the betbutton
            if (!btnRowTwoNumberOne.IsChecked.Value && !btnRowTwoNumberTwo.IsChecked.Value && !btnRowTwoNumberThree.IsChecked.Value
                && !btnRowTwoNumberFour.IsChecked.Value && !btnRowTwoNumberFive.IsChecked.Value)
            {
                btnBet.IsEnabled = false;
            }

            //If one button is active and a betamout is entered, enable the bet button
            if ((btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value
                || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value) && tbxBetSum.Text.ToString() != "")
            {
                btnBet.IsEnabled = true;
            }
        }

        private void btnRowTwoNumberThree_Click(object sender, RoutedEventArgs e)
        {
            //Check if another number is pressed already
            if (btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsChecked = false;
                btnRowTwoNumberTwo.IsChecked = false;
                btnRowTwoNumberFour.IsChecked = false;
                btnRowTwoNumberFive.IsChecked = false;
            }
            //set property for bet
            NumberTwo = 3;

            //If no button is active, disable the betbutton
            if (!btnRowTwoNumberOne.IsChecked.Value && !btnRowTwoNumberTwo.IsChecked.Value && !btnRowTwoNumberThree.IsChecked.Value
                && !btnRowTwoNumberFour.IsChecked.Value && !btnRowTwoNumberFive.IsChecked.Value)
            {
                btnBet.IsEnabled = false;
            }

            //If one button is active and a betamout is entered, enable the bet button
            if ((btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value
                || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value) && tbxBetSum.Text.ToString() != "")
            {
                btnBet.IsEnabled = true;
            }
        }

        private void btnRowTwoNumberFour_Click(object sender, RoutedEventArgs e)
        {
            //Check if another number is pressed already
            if (btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsChecked = false;
                btnRowTwoNumberTwo.IsChecked = false;
                btnRowTwoNumberThree.IsChecked = false;
                btnRowTwoNumberFive.IsChecked = false;
            }
            //set property for bet
            NumberTwo = 4;

            //If no button is active, disable the betbutton
            if (!btnRowTwoNumberOne.IsChecked.Value && !btnRowTwoNumberTwo.IsChecked.Value && !btnRowTwoNumberThree.IsChecked.Value
                && !btnRowTwoNumberFour.IsChecked.Value && !btnRowTwoNumberFive.IsChecked.Value)
            {
                btnBet.IsEnabled = false;
            }

            //If one button is active and a betamout is entered, enable the bet button
            if ((btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value
                || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value) && tbxBetSum.Text.ToString() != "")
            {
                btnBet.IsEnabled = true;
            }
        }

        private void btnRowTwoNumberFive_Click(object sender, RoutedEventArgs e)
        {
            //Check if another number is pressed already
            if (btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value || btnRowTwoNumberFour.IsChecked.Value)
            {
                btnRowTwoNumberOne.IsChecked = false;
                btnRowTwoNumberTwo.IsChecked = false;
                btnRowTwoNumberThree.IsChecked = false;
                btnRowTwoNumberFour.IsChecked = false;
            }
            //set property for bet
            NumberTwo = 5;

            //If no button is active, disable the betbutton
            if (!btnRowTwoNumberOne.IsChecked.Value && !btnRowTwoNumberTwo.IsChecked.Value && !btnRowTwoNumberThree.IsChecked.Value
                && !btnRowTwoNumberFour.IsChecked.Value && !btnRowTwoNumberFive.IsChecked.Value)
            {
                btnBet.IsEnabled = false;
            }

            //If one button is active and a betamout is entered, enable the bet button
            if ((btnRowTwoNumberOne.IsChecked.Value || btnRowTwoNumberTwo.IsChecked.Value || btnRowTwoNumberThree.IsChecked.Value
                || btnRowTwoNumberFour.IsChecked.Value || btnRowTwoNumberFive.IsChecked.Value) && tbxBetSum.Text.ToString() != "")
            {
                btnBet.IsEnabled = true;
            }
        }

        #endregion

        #region Bet box and button

        private void tbxBetSum_KeyDown(object sender, KeyEventArgs e)
        {
            //Enable betbutton
            if ((((TextBox)sender).Text.ToString().Length >= 0) && (btnRowOneNumberOne.IsChecked.Value || btnRowOneNumberTwo.IsChecked.Value || btnRowOneNumberThree.IsChecked.Value
                || btnRowOneNumberFour.IsChecked.Value || btnRowOneNumberFive.IsChecked.Value))
            {
                btnBet.IsEnabled = true;
            }

            //TODO: check decimal //Only one period is allowed
            if (e.Key == Key.Decimal)
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

        private void btnBet_Click(object sender, RoutedEventArgs e)
        {
            double amount;
            double.TryParse(tbxBetSum.Text, out amount);

            if (myCasinoSvcGaming.UserType == 0)
            {
                MessageBox.Show("Betreiber darf nicht Wetten!", "Webservices Fehler", MessageBoxButton.OK);
                return;
            }

            try
            {
                myCasinoSvcGaming.MyCasinoSvc.betCompleted += MyCasinoSvc_betCompleted;
                myCasinoSvcGaming.MyCasinoSvc.betAsync(myCasinoSvcGaming.SessionId, amount, NumberOne, NumberTwo);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Fehler beim Anzeigen der Wetten: " + ex.ToString());
            }

           
        }

        private void MyCasinoSvc_betCompleted(object sender, MyCasinoWSServer.betCompletedEventArgs e)
        {
            errMsg = e.errMsg;
            if ("BET_ALREADY_SET" == errMsg)
            {
                MessageBox.Show("Ein anderer Benutzer hat auf diese Zahlen schon gewettet");
            }
            else if ("NOT_ENOUGH_MONEY" == errMsg)
            {
                MessageBox.Show("Nicht genügend Geld vorhanden!");
            }
            else if ("NO_OPERATOR_LOGGED_IN" == errMsg)
            {
                MessageBox.Show("Kein Betreiber eingeloggt!");
            }
            else if ("BET_AMOUNT_TOO_HIGH" == errMsg)
            {
                MessageBox.Show("Einsatz zu hoch!");
            }
            else if (errMsg == "INVALID_SESSION_ID")
            {
                MessageBox.Show("Ungültige ID!");
            }

            //Disable all row2 buttons
            btnRowTwoNumberOne.IsEnabled = false;
            btnRowTwoNumberTwo.IsEnabled = false;
            btnRowTwoNumberThree.IsEnabled = false;
            btnRowTwoNumberFour.IsEnabled = false;
            btnRowTwoNumberFive.IsEnabled = false;

            //Clear all row2 buttons
            btnRowTwoNumberOne.IsChecked = false;
            btnRowTwoNumberTwo.IsChecked = false;
            btnRowTwoNumberThree.IsChecked = false;
            btnRowTwoNumberFour.IsChecked = false;
            btnRowTwoNumberFive.IsChecked = false;

            //Clear all row1 buttons
            btnRowOneNumberOne.IsChecked = false;
            btnRowOneNumberTwo.IsChecked = false;
            btnRowOneNumberThree.IsChecked = false;
            btnRowOneNumberFour.IsChecked = false;
            btnRowOneNumberFive.IsChecked = false;

            //Disable own button
            btnBet.IsEnabled = false;

            //Refresh showbets
            //btnRefresh_Click(new object(), new RoutedEventArgs());
            myCasinoSvcGaming.MyCasinoSvc.betCompleted -= MyCasinoSvc_betCompleted;

        }

        private void tbxBetSum_KeyUp(object sender, KeyEventArgs e)
        {
            //Disable betbutton
            if (((TextBox)sender).Text.ToString() == "")
            {
                btnBet.IsEnabled = false;
            }
        }

        #endregion

        #region Refresh button

        int count = 0;

        private void btnRefresh_Click(object sender, RoutedEventArgs e)
        {
            //Clear showbets
            lbNameList.Items.Clear();
            lbFirstNumberList.Items.Clear();
            lbSecondNumberList.Items.Clear();
            lbAmountWinList.Items.Clear();
           
            try
            {
                myCasinoSvcGaming.MyCasinoSvc.showbetsCompleted += MyCasinoSvc_showbetsCompleted;
                myCasinoSvcGaming.MyCasinoSvc.showbetsAsync(myCasinoSvcGaming.SessionId);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Fehler beim Anzeigen der Wetten: " + ex.ToString());
            }

        }

        private void MyCasinoSvc_showbetsCompleted(object sender, MyCasinoWSServer.showbetsCompletedEventArgs e)
        {
            string errMsgShowBets;
            errMsgShowBets = e.errMsg;
            if (errMsgShowBets == "INVALID_SESSION_ID")
            {
                MessageBox.Show("Ungültige ID!");
            }
            count = e.count;
            //TODO: posi
            myCasinoSvcGaming.MyCasinoSvc.showbetsCompleted -= MyCasinoSvc_showbetsCompleted;
            
            for (int i = 0; i < count; i++)
            {
                myCasinoSvcGaming.MyCasinoSvc.calculateProfitCompleted += MyCasinoSvc_calculateProfitCompleted;
                myCasinoSvcGaming.MyCasinoSvc.calculateProfitAsync(myCasinoSvcGaming.SessionId, e.amount.ElementAt(i),
                    e.firstNumber.ElementAt(i), e.secondNumber.ElementAt(i));

                lbNameList.Items.Add(e.names.ElementAt(i));
                lbFirstNumberList.Items.Add(e.firstNumber.ElementAt(i).ToString());
                lbSecondNumberList.Items.Add(e.secondNumber.ElementAt(i).ToString());
            }
        }

        private void MyCasinoSvc_calculateProfitCompleted(object sender, MyCasinoWSServer.calculateProfitCompletedEventArgs e)
        {
            if (errMsg == "INVALID_SESSION_ID")
            {
                MessageBox.Show("Ungültige ID!");
            }

            lbAmountWinList.Items.Add("Eine richtige: " + e.profitForOneMatch.ToString() + "   Zwei richtige: " + e.profitForTwoMatches.ToString());
            myCasinoSvcGaming.MyCasinoSvc.calculateProfitCompleted -= MyCasinoSvc_calculateProfitCompleted;

        }

        #endregion

        #region Draw button

        private void btnDraw_Click(object sender, RoutedEventArgs e)
        {
            myCasinoSvcGaming.MyCasinoSvc.drawCompleted += MyCasinoSvc_drawCompleted;
            myCasinoSvcGaming.MyCasinoSvc.drawAsync(myCasinoSvcGaming.SessionId);
        }

        private void MyCasinoSvc_drawCompleted(object sender, MyCasinoWSServer.drawCompletedEventArgs e)
        {
            //init
            string errMsg = null;
            errMsg = e.errMsg;
                if (errMsg == "INVALID_SESSION_ID")
                {
                    MessageBox.Show("Ungültige ID!", "Web service Fehler", MessageBoxButton.OK);
                }
                else if(errMsg!=null)
                {
                    MessageBox.Show("Fehler beim Ziehen!", "Web service Fehler", MessageBoxButton.OK);
                }
            

            //set numbers that have been drawn
            //tblDrawnNumberOne.Text = firstNumber.ToString();
            //tblDrawnNumberTwo.Text = secondNumber.ToString();

            //Refresh showbets
            myCasinoSvcGaming.MyCasinoSvc.drawCompleted -= MyCasinoSvc_drawCompleted;
            btnRefresh_Click(new object(), new RoutedEventArgs());
        }

        private void btnDrawNumbers_Click(object sender, RoutedEventArgs e)
        {
            //init
            firstNumber = NumberOne;
            secondNumber = NumberTwo;

            myCasinoSvcGaming.MyCasinoSvc.drawtestCompleted += MyCasinoSvc_drawtestCompleted;
            myCasinoSvcGaming.MyCasinoSvc.drawtestAsync(myCasinoSvcGaming.SessionId, firstNumber, secondNumber);

           

            //set numbers that have been drawn
            //tblDrawnNumberOne.Text = firstNumber.ToString();
            //tblDrawnNumberTwo.Text = secondNumber.ToString();

            //reset Numbers
            NumberOne = 0;
            NumberTwo = 0;

            //Disable all row2 buttons
            btnRowTwoNumberOne.IsEnabled = false;
            btnRowTwoNumberTwo.IsEnabled = false;
            btnRowTwoNumberThree.IsEnabled = false;
            btnRowTwoNumberFour.IsEnabled = false;
            btnRowTwoNumberFive.IsEnabled = false;

            //Clear all row2 buttons
            btnRowTwoNumberOne.IsChecked = false;
            btnRowTwoNumberTwo.IsChecked = false;
            btnRowTwoNumberThree.IsChecked = false;
            btnRowTwoNumberFour.IsChecked = false;
            btnRowTwoNumberFive.IsChecked = false;

            //Clear all row1 buttons
            btnRowOneNumberOne.IsChecked = false;
            btnRowOneNumberTwo.IsChecked = false;
            btnRowOneNumberThree.IsChecked = false;
            btnRowOneNumberFour.IsChecked = false;
            btnRowOneNumberFive.IsChecked = false;

            
        }

        private void MyCasinoSvc_drawtestCompleted(object sender, MyCasinoWSServer.drawtestCompletedEventArgs e)
        {
            //init
            string errMsg=null;
            errMsg = e.errMsg;

            //remove event
            myCasinoSvcGaming.MyCasinoSvc.drawtestCompleted -= MyCasinoSvc_drawtestCompleted;

            //check errors
            if (errMsg == "ERROR_MY_CASINO_BET_INVALID_NUMBER") MessageBox.Show("Zahlen auswählen!");
            else if (errMsg == "INVALID_SESSION_ID")
            {
                MessageBox.Show("Ungültige ID!");
            }
            else if (errMsg != null)
            {
                MessageBox.Show("Fehler beim Ziehen");
            }

            //Refresh showbets
            btnRefresh_Click(new object(), new RoutedEventArgs());
        }

        #endregion


    }
}