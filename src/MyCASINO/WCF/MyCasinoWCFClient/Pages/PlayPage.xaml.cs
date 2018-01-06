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
        List<Bet> betlist = new List<Bet>();

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


        public PlayPage(INETMyCasino _RemSrvMyCasinoMain, string usernameTmp, int sessionIdTmp, MyCasinoUserTypes typeTmp)
        {
            _RemSrvMyCasino = _RemSrvMyCasinoMain;
            InitializeComponent();
            NameList nameList = new NameList();
            //lbNameList.ItemsSource = nameList;
            //lbAmountWinList.ItemsSource = nameList;
            //lbFirstNumberList.ItemsSource = nameList;
            //lbSecondNumberList.ItemsSource = nameList;
            BetAmount betAmount = new BetAmount();
            btnRowOneNumberFive.IsEnabled = false;
            SessionId = sessionIdTmp;

            //disable operator-only features
            if (typeTmp == MyCasinoUserTypes.Gamer)
            {
                btnDraw.Visibility = Visibility.Hidden;
            }
            //disable  gamer only- features
            if (typeTmp == MyCasinoUserTypes.Operator)
            {
                btnRowOneNumberOne.Visibility = Visibility.Hidden;
                btnRowOneNumberTwo.Visibility = Visibility.Hidden;
                btnRowOneNumberThree.Visibility = Visibility.Hidden;
                btnRowOneNumberFour.Visibility = Visibility.Hidden;
                btnRowOneNumberFive.Visibility = Visibility.Hidden;

                btnRowTwoNumberOne.Visibility = Visibility.Hidden;
                btnRowTwoNumberTwo.Visibility = Visibility.Hidden;
                btnRowTwoNumberThree.Visibility = Visibility.Hidden;
                btnRowTwoNumberFour.Visibility = Visibility.Hidden;
                btnRowTwoNumberFive.Visibility = Visibility.Hidden;


                tbxBetSum.Visibility = Visibility.Hidden;
                btnBet.Visibility = Visibility.Hidden;
                tblSetAmount.Visibility = Visibility.Hidden;
            }

        }

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
                //btnRowTwoNumberOne.IsEnabled = true;
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

            //Enable row2 buttons if one button from row 1 is active
            if (btnRowOneNumberTwo.IsChecked.Value)
            {
                //btnRowTwoNumberOne.IsEnabled = true;
                //btnRowTwoNumberTwo.IsEnabled = true;
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
                //btnRowTwoNumberOne.IsEnabled = true;
                //btnRowTwoNumberTwo.IsEnabled = true;
                //btnRowTwoNumberThree.IsEnabled = true;
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
                //btnRowTwoNumberOne.IsEnabled = true;
                //btnRowTwoNumberTwo.IsEnabled = true;
                //btnRowTwoNumberThree.IsEnabled = true;
                //btnRowTwoNumberFour.IsEnabled = true;
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

            //Enable row2 buttons if one button from row 1 is active
            if (btnRowOneNumberFive.IsChecked.Value)
            {
                //btnRowTwoNumberOne.IsEnabled = true;
                //btnRowTwoNumberTwo.IsEnabled = true;
                //btnRowTwoNumberThree.IsEnabled = true;
                //btnRowTwoNumberFour.IsEnabled = true;
                //btnRowTwoNumberFive.IsEnabled = true;
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
            string errMsg;
            double amount;
            double.TryParse(tbxBetSum.Text, out amount);
            _RemSrvMyCasino.bet(SessionId, amount, NumberOne, NumberTwo, out errMsg);

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
            int count;
            string errMsg;

            //List<Bet> betlist = new List<Bet>();
            //_RemSrvMyCasino.showbets(SessionId, out betlist, out count, out errMsg);
            
            //init lists
            List<string> names = new List<string>();
            List<int> firstNumberBetList = new List<int>();
            List<int> secondNumberBetList = new List<int>();
            List<double> amountBetList = new List<double>();

            _RemSrvMyCasino.showbets(SessionId, out names, out firstNumberBetList, out secondNumberBetList, out amountBetList,  out count, out errMsg);

            //lbNameList.ItemsSource = names;
            //lbNameList.Items.Add(names)

            for (int i = 0; i < count; i++)
            {
                double profitForOneMatch = 0, profitForTwoMatches = 0;
                //_RemSrvMyCasino.calculateProfit(SessionId, betlist.ElementAt(i).M_setAmount, out profitForOneMatch, out profitForTwoMatches, out errMsg);
                //lbNameList.ItemsSource = betlist.ElementAt(i).M_name;
                //lbFirstNumberList.ItemsSource = betlist.ElementAt(i).M_firstNumber.ToString();
                //lbSecondNumberList.ItemsSource = betlist.ElementAt(i).M_secondNumber.ToString();
                //lbAmountWinList.ItemsSource = betlist.ElementAt(i).;

                //lbNameList.ItemsSource = names.ElementAt(i).ToString();
                lbNameList.Items.Add(names.ElementAt(i));
                //lbFirstNumberList.ItemsSource = firstNumberBetList.ElementAt(i).ToString();
                //lbSecondNumberList.ItemsSource = secondNumberBetList.ElementAt(i).ToString();
                //lbAmountWinList.ItemsSource = betlist.ElementAt(i).


                //lbSecondNumberList.ItemsSource = betlist.ElementAt(i).ToString();
            }

        }
    }
}
