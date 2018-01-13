/************************************************************/
/*                                                          */
/* Inhalt:    DialogExtensions: Klasse zur einfacheren      */
/*            Handhabung von Dialoghierarchien              */
/*            unter Windows Phone 7/8 / Silverlight         */
/*                                                          */
/* Autor(en): Josef Pösl (jp), <XXX>                        */
/* Firma:     OTH Hochschule Amberg-Weiden                  */
/* Stand:     20. Nov 2015                                  */
/*                                                          */
/* Historie:  21. Dez 2011 jp  erstellt                     */
/*            14. Nov 2012 jp  ChildPageType bei NaviDel    */
/*            20. Nov 2015 jp  Navi to pages in classlib    */
/*            xx. xxx xxxx     modifiziert...               */
/*                                                          */
/* Copyright 2001-2050 HAW Amberg-Weiden ... usw.           */
/*                                                          */
/************************************************************/

using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace MyBayWSPhoneCln
{
    public delegate void ChildNaviDel<ChildPageType>(ChildPageType childPage) where ChildPageType : PhoneApplicationPage;

    class Navigator<ChildPageType> where ChildPageType : PhoneApplicationPage
    {
        PhoneApplicationPage parentPage;
        Guid childGuid = Guid.NewGuid();
        ChildPageType childPage;
        ChildNaviDel<ChildPageType> childOpened, childClosed;

        public Navigator(PhoneApplicationPage basePage)
        {
            this.parentPage = basePage;
        }

        public Navigator(PhoneApplicationPage basePage, ChildNaviDel<ChildPageType> childOpened, ChildNaviDel<ChildPageType> childClosed)
        {
            this.parentPage = basePage;
            this.childOpened = childOpened;
            this.childClosed = childClosed;
        }

        public void Navigate()
        {
            //Assembly.GetName does not work!!
            string assName = typeof(ChildPageType).Assembly.FullName;
            assName = assName.Substring(0, assName.IndexOf(','));
            parentPage.NavigationService.Navigated += new System.Windows.Navigation.NavigatedEventHandler(NavigationService_Navigated);
            parentPage.NavigationService.Navigate(new Uri("/" + assName + ";component/" + typeof(ChildPageType).Name + ".xaml?guid=" + childGuid, UriKind.Relative));
        }

        void NavigationService_Navigated(object sender, System.Windows.Navigation.NavigationEventArgs e)
        {
            PhoneApplicationPage page = e.Content as PhoneApplicationPage;

            if (page == null) return;

            Guid naviGuid = new Guid();

            if (page.NavigationContext.QueryString.ContainsKey("guid")) naviGuid = new Guid(page.NavigationContext.QueryString["guid"]);

            if (naviGuid == childGuid && childPage == null)
            {
                childPage = page as ChildPageType;
                if (childOpened != null) childOpened(childPage);
            }
            else if (e.Content == parentPage && childPage != null)
            {
                page.NavigationService.Navigated -= NavigationService_Navigated;
                if (childClosed != null) childClosed(childPage);
                childPage = null;
            }
        }
    }

    public enum DialogResult
    {
        Ok, Cancel
    }

    public static class DialogExtensionsMethods
    {
        public static void ShowNewDialog<ChildPageType>(this PhoneApplicationPage This) where ChildPageType : PhoneApplicationPage
        {
            new Navigator<ChildPageType>(This).Navigate();
        }
        public static void ShowNewDialog<ChildPageType>(this PhoneApplicationPage This, ChildNaviDel<ChildPageType> childOpened, ChildNaviDel<ChildPageType> childClosed) where ChildPageType : PhoneApplicationPage
        {
            new Navigator<ChildPageType>(This, childOpened, childClosed).Navigate();
        }
        public static void Close(this PhoneApplicationPage This)
        {
            This.NavigationService.GoBack();
        }

        /* Is not conform to navigation with phone keys
         * public static void ShowPage(this PhoneApplicationPage This, PhoneApplicationPage page)
        {
            if (!(This.Parent is PhoneApplicationFrame)) return;
            ((PhoneApplicationFrame)This.Parent).Content = page;
        }*/
    }
}
