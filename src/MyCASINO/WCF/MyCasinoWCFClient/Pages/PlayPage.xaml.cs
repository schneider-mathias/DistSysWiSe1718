//#define COM
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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
    #region dummy!!!!!
    /// <summary>
    /// Dummy implementierung
    /// </summary>
    public class NameList : ObservableCollection<PersonName>
    {
        public NameList() : base()
        {
            Add(new PersonName("Manuel"));
            Add(new PersonName("Johannes"));
            Add(new PersonName("Mathias"));
            Add(new PersonName("Martin"));
        }
    }

    public class PersonName
    {
        private string name;

        public PersonName(string name)
        {
            this.name = name;
        }

        public string Name
        {
            get { return name; }
            set { name = value; }
        }
    }

    public class BetAmount : INotifyPropertyChanged
    {
        private double betAmount;

        public event PropertyChangedEventHandler PropertyChanged;

        private void NotifyPropertyChanged(String info = "")
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }

        public BetAmount()
        {
            this.betAmount = 1.00;
        }

        public double Amount
        {
            get { return betAmount; }
            set
            {
                betAmount = value;
                NotifyPropertyChanged();
            }
        }
    }

    #endregion
    /// <summary>
    /// Interaction logic for PlayPage.xaml
    /// </summary>
    public partial class PlayPage : Page
    {
        //List<Bet> betlist = new List<Bet>();

        private string errMsg;


#if COM
        private short numberOne;

        public short NumberOne
        {
            get { return numberOne; }
            set { numberOne = value; }
        }

        private short numberTwo;

        public short NumberTwo
        {
            get { return numberTwo; }
            set { numberTwo = value; }
        }

        //First drawn number
        private short firstNumber;

        public short FirstNumber
        {
            get { return firstNumber; }
            set { firstNumber = value; }
        }

        //Second drawn Number
        private short secondNumber;

        public short SecondNumber
        {
            get { return secondNumber; }
            set { secondNumber = value; }
        }

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

        public PlayPage(COMMyCasinoSrvLib.COMMyCasino _comSrvTmp, string usernameTmp, uint sessionIdTmp, short typeTmp)
        {
            _ComSrv = _comSrvTmp;
            InitializeComponent();
            //BetAmount betAmount = new BetAmount();
            btnRowOneNumberFive.IsEnabled = false;
            SessionId = sessionIdTmp;

            //disable operator-only features
            if (typeTmp == 1)
            {
                tblLastDrawing.Visibility = Visibility.Hidden;
                btnDraw.Visibility = Visibility.Hidden;
                btnDrawNumbers.Visibility = Visibility.Hidden;
            }
            //disable  gamer only- features
            if (typeTmp == 0)
            {

                tbxBetSum.Visibility = Visibility.Hidden;
                btnBet.Visibility = Visibility.Hidden;
                tblSetAmount.Visibility = Visibility.Hidden;
            }

        }
#else
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

        public PlayPage(INETMyCasino _RemSrvMyCasinoMain, string usernameTmp, int sessionIdTmp, MyCasinoUserTypes typeTmp)
        {
            _RemSrvMyCasino = _RemSrvMyCasinoMain;
            InitializeComponent();
            BetAmount betAmount = new BetAmount();
            btnRowOneNumberFive.IsEnabled = false;
            SessionId = sessionIdTmp;

            //disable operator-only features
            if (typeTmp == MyCasinoUserTypes.Gamer)
            {
                btnDraw.Visibility = Visibility.Hidden;
                btnDrawNumbers.Visibility = Visibility.Hidden;
                tblLastDrawing.Visibility = Visibility.Hidden;
            }
            //disable  gamer only- features
            if (typeTmp == MyCasinoUserTypes.Operator)
            {
                tbxBetSum.Visibility = Visibility.Hidden;
                btnBet.Visibility = Visibility.Hidden;
                tblSetAmount.Visibility = Visibility.Hidden;
            }

        }
#endif
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
        private void tbxBetSum_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            //Allow only numbers between "0-9","." and backspace
            e.Handled = !(e.Key >= Key.D0 && e.Key <= Key.D9 ||
                e.Key >= Key.NumPad0 && e.Key <= Key.NumPad9 ||
                e.Key == Key.Back || e.Key == Key.OemPeriod
                );
        }

        private void tbxBetSum_KeyDown(object sender, KeyEventArgs e)
        {
            //Enable betbutton
            if ((((TextBox)sender).Text.ToString().Length >= 0) && (btnRowOneNumberOne.IsChecked.Value || btnRowOneNumberTwo.IsChecked.Value || btnRowOneNumberThree.IsChecked.Value
                || btnRowOneNumberFour.IsChecked.Value || btnRowOneNumberFive.IsChecked.Value))
            {
                btnBet.IsEnabled = true;
            }

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

        private void btnBet_Click(object sender, RoutedEventArgs e)
        {
            double amount;
            double.TryParse(tbxBetSum.Text, out amount);
#if COM
            try
            {
                _ComSrv.bet(SessionId, amount, NumberOne, NumberTwo, out errMsg);
            }
            catch (Exception ex)
            {
                if (ex is COMException)
                    errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                else
                    errMsg = "Unknown";

                MessageBox.Show(errMsg);
            }
#else
            try
            {
                _RemSrvMyCasino.bet(SessionId, amount, NumberOne, NumberTwo, out errMsg);
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
                else if ("NOT_ENOUGH_MONEY_CASINO" == errMsg)
                {
                    MessageBox.Show("Das Casino besitzt nicht genügend Geld!");
                }
                else if (errMsg == "INVALID_SESSION_ID")
                {
                    MessageBox.Show("Ungültige ID!");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Fehler beim Anzeigen der Wetten: " + ex.ToString());
            }
#endif
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
            btnRefresh_Click(new object(), new RoutedEventArgs());
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

        private void btnRefresh_Click(object sender, RoutedEventArgs e)
        {
            //init
            string errMsg=null;
            //Clear showbets
            lbNameList.Items.Clear();
            lbFirstNumberList.Items.Clear();
            lbSecondNumberList.Items.Clear();
            lbAmountWinList.Items.Clear();
#if COM
            //init COM
            uint count = 0;
#else
            //init lists
            int count=0;
            List<string> names = new List<string>();
            List<int> firstNumberBetList = new List<int>();
            List<int> secondNumberBetList = new List<int>();
            List<double> amountBetList = new List<double>();
#endif

#if COM
            System.Array bets=null;
            try
            {
                _comSrv.showbets(SessionId, out bets, out count, out errMsg);
            }
            catch (Exception ex)
            {
                if (ex is COMException)
                    errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                else
                    errMsg = "Unknown";
            }
#else
            try
            {
                _RemSrvMyCasino.showbets(SessionId, out names, out firstNumberBetList, out secondNumberBetList, out amountBetList, out count, out errMsg);
                if (errMsg == "INVALID_SESSION_ID")
                {
                    MessageBox.Show("Ungültige ID!");
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("Fehler beim Anzeigen der Wetten: " + ex.ToString());
            }
#endif

#if COM
            for (int i = 0; i < count*4; i+=4)
            {
                double profitForOneMatch = 0, profitForTwoMatches = 0;
                try
                {
                    _ComSrv.calculateProfit(SessionId,(double)bets.GetValue(i+3) , (short)bets.GetValue(i+1), (short)bets.GetValue(i+2), out profitForOneMatch, out profitForTwoMatches, out errMsg);
                    lbNameList.Items.Add(bets.GetValue(i));
                    lbFirstNumberList.Items.Add(bets.GetValue(i+1).ToString());
                    lbSecondNumberList.Items.Add(bets.GetValue(i+2).ToString());
                    lbAmountWinList.Items.Add("Eine richtige: " + profitForOneMatch.ToString() + "   Zwei richtige: " + profitForTwoMatches.ToString());
                }
                catch (Exception ex)
                {
                    if (ex is COMException)
                        errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                    else
                        errMsg = "Unknown";
                }
#else
            for (int i = 0; i < count ; i++)
            {
                double profitForOneMatch = 0, profitForTwoMatches = 0;
                try
                {
                    _RemSrvMyCasino.calculateProfit(SessionId, amountBetList.ElementAt(i), firstNumberBetList.ElementAt(i), secondNumberBetList.ElementAt(i), out profitForOneMatch, out profitForTwoMatches, out errMsg);
                    if (errMsg == "INVALID_SESSION_ID")
                    {
                        MessageBox.Show("Ungültige ID!");
                    }
                }
                catch(Exception ex)
                {
                    MessageBox.Show("Fehler bei Transactionen: " + ex.ToString());
                }
                lbNameList.Items.Add(names.ElementAt(i));
                lbFirstNumberList.Items.Add(firstNumberBetList.ElementAt(i).ToString());
                lbSecondNumberList.Items.Add(secondNumberBetList.ElementAt(i).ToString());
                lbAmountWinList.Items.Add("Eine richtige: "+ profitForOneMatch.ToString() + "   Zwei richtige: " + profitForTwoMatches.ToString());
#endif
            }
        }

        private void btnDraw_Click(object sender, RoutedEventArgs e)
        {
            //init
            string errMsg = null;
#if COM
            try
            {
                _ComSrv.draw(SessionId, out firstNumber, out secondNumber, out errMsg);
            }
            catch (Exception ex)
            {
                if (ex is COMException)
                    errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                else
                    errMsg = "Unknown";
            }
#else
            try
            {
                _RemSrvMyCasino.draw(SessionId, out firstNumber, out secondNumber, out errMsg);
                if (errMsg == "INVALID_SESSION_ID")
                {
                    MessageBox.Show("Ungültige ID!");
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("Fehler beim Ziehen: " + ex.ToString());
            }
#endif
            //set numbers that have been drawn
            tblDrawnNumberOne.Text = firstNumber.ToString();
            tblDrawnNumberTwo.Text = secondNumber.ToString();

            //Refresh showbets
            btnRefresh_Click(new object(), new RoutedEventArgs());
        }

        private void btnDrawNumbers_Click(object sender, RoutedEventArgs e)
        {
            //init
            string errMsg = null;
            firstNumber=NumberOne;
            secondNumber=NumberTwo;
#if COM
            try
            {
                _ComSrv.drawTest(SessionId, firstNumber, secondNumber, out errMsg);
            }
            catch (Exception ex)
            {
                if (ex is COMException)
                    errMsg = Codes.ResolveCode((ex as COMException).ErrorCode);
                else
                    errMsg = "Unknown";
            }
#else
            try
            {
                _RemSrvMyCasino.drawtest(SessionId, firstNumber, secondNumber, out errMsg);
                if (errMsg == "ERROR_MY_CASINO_BET_INVALID_NUMBER") MessageBox.Show("Zahlen auswählen!");
                else if (errMsg == "INVALID_SESSION_ID")
                {
                    MessageBox.Show("Ungültige ID!");
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("Fehler beim Ziehen: " + ex.ToString());
            }
#endif
            //set numbers that have been drawn
            tblDrawnNumberOne.Text = firstNumber.ToString();
            tblDrawnNumberTwo.Text = secondNumber.ToString();

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

            //Refresh showbets
            btnRefresh_Click(new object(), new RoutedEventArgs());
        }
    }
}
