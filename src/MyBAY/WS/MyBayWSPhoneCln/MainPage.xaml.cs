using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using MyBayWSPhoneCln.Resources;

namespace MyBayWSPhoneCln
{
    public partial class MainPage : PhoneApplicationPage
    {
        private DataObject _myDataObjectMain = new DataObject();

        public DataObject MyDataObjectMain
        {
            get { return _myDataObjectMain; }
            set { _myDataObjectMain = value; }
        }


        // Konstruktor
        public MainPage()
        {
           // this._myDataObject = new DataObject();
            InitializeComponent();

            _myDataObjectMain.RemoteSrvMyBay = new MyBayWSSrv.MyBayWSSrvASMXSoapClient();
            // MyBayWSSrv.MyBayWSSrvASMXSoapClient _remoteSrvMyBay = new MyBayWSSrv.MyBayWSSrvASMXSoapClient();

            // Beispielcode zur Lokalisierung der ApplicationBar
            //BuildLocalizedApplicationBar();
        }

        private void btn_login_Click(object sender, RoutedEventArgs e)
        {
            this.ShowNewDialog<LoginPage>(
                cp => { cp.MyDataObject = _myDataObjectMain; },
                cp =>
                {
                    MyDataObjectMain = cp.MyDataObject;
                });
        }

        // Beispielcode zur Erstellung einer lokalisierten ApplicationBar
        //private void BuildLocalizedApplicationBar()
        //{
        //    // ApplicationBar der Seite einer neuen Instanz von ApplicationBar zuweisen
        //    ApplicationBar = new ApplicationBar();

        //    // Eine neue Schaltfläche erstellen und als Text die lokalisierte Zeichenfolge aus AppResources zuweisen.
        //    ApplicationBarIconButton appBarButton = new ApplicationBarIconButton(new Uri("/Assets/AppBar/appbar.add.rest.png", UriKind.Relative));
        //    appBarButton.Text = AppResources.AppBarButtonText;
        //    ApplicationBar.Buttons.Add(appBarButton);

        //    // Ein neues Menüelement mit der lokalisierten Zeichenfolge aus AppResources erstellen
        //    ApplicationBarMenuItem appBarMenuItem = new ApplicationBarMenuItem(AppResources.AppBarMenuItemText);
        //    ApplicationBar.MenuItems.Add(appBarMenuItem);
        //}
    }
}