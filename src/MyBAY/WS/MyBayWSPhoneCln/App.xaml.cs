﻿using System;
using System.Diagnostics;
using System.Resources;
using System.Windows;
using System.Windows.Markup;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using MyBayWSPhoneCln.Resources;
using PortForwardLib;

namespace MyBayWSPhoneCln
{
    public partial class App : Application
    {
        /// <summary>
        /// Bietet einen einfachen Zugriff auf den Stammframe der Phone-Anwendung.
        /// </summary>
        /// <returns>Der Stammframe der Phone-Anwendung.</returns>
        public static PhoneApplicationFrame RootFrame { get; private set; }

        PortForwarderWP pfWebSvc, pfWCFSvc;

        public static DataObject MyDataObject;

        /// <summary>
        /// Konstruktor für das Application-Objekt.
        /// </summary>
        public App()
        {
            // Globaler Handler für nicht abgefangene Ausnahmen.
            UnhandledException += Application_UnhandledException;

            // Standard-XAML-Initialisierung
            InitializeComponent();

            // Phone-spezifische Initialisierung
            InitializePhoneApplication();

            // Initialisierung der Sprachanzeige
            InitializeLanguage();

            // Während des Debuggens Profilerstellungsinformationen zur Grafikleistung anzeigen.
            if (Debugger.IsAttached)
            {
                // Zähler für die aktuelle Bildrate anzeigen.
                Application.Current.Host.Settings.EnableFrameRateCounter = true;

                // Bereiche der Anwendung hervorheben, die mit jedem Bild neu gezeichnet werden.
                //Application.Current.Host.Settings.EnableRedrawRegions = true;

                // Nicht produktiven Visualisierungsmodus für die Analyse aktivieren,
                // in dem Bereiche einer Seite angezeigt werden, die mit einer Farbüberlagerung an die GPU übergeben wurden.
                //Application.Current.Host.Settings.EnableCacheVisualization = true;

                // Verhindert, dass der Bildschirm im Debugger ausgeschaltet wird, indem
                // die Leerlauferkennung der Anwendung deaktiviert wird.
                // Vorsicht: Nur im Debugmodus verwenden. Eine Anwendung mit deaktivierter Benutzerleerlauferkennung wird weiterhin ausgeführt
                // und verbraucht auch dann Akkuenergie, wenn der Benutzer das Telefon nicht verwendet.
                PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Disabled;
            }

            App.MyDataObject = new DataObject();

            pfWebSvc = new PortForwarderWP(7776 , 7777);

        }

        // Code zur Ausführung bei der Rückgabe einer Vertragsaktivierung wie z. B. einem "Auswahlvertrag zum Öffnen von Dateien" oder einer Dateispeicherauswahl 
        // mit der ausgewählten Datei oder anderen Rückgabewerten
        private void Application_ContractActivated(object sender, Windows.ApplicationModel.Activation.IActivatedEventArgs e)
        {
        }

        // Code, der beim Starten der Anwendung ausgeführt werden soll (z. B. über "Start")
        // Dieser Code wird beim Reaktivieren der Anwendung nicht ausgeführt
        private void Application_Launching(object sender, LaunchingEventArgs e)
        {
            pfWebSvc.Run();

        }

        // Code, der ausgeführt werden soll, wenn die Anwendung aktiviert wird (in den Vordergrund gebracht wird)
        // Dieser Code wird beim ersten Starten der Anwendung nicht ausgeführt
        private void Application_Activated(object sender, ActivatedEventArgs e)
        {
        }

        // Code, der ausgeführt werden soll, wenn die Anwendung deaktiviert wird (in den Hintergrund gebracht wird)
        // Dieser Code wird beim Schließen der Anwendung nicht ausgeführt
        private void Application_Deactivated(object sender, DeactivatedEventArgs e)
        {
        }

        // Code, der beim Schließen der Anwendung ausgeführt wird (z. B. wenn der Benutzer auf "Zurück" klickt)
        // Dieser Code wird beim Deaktivieren der Anwendung nicht ausgeführt
        private void Application_Closing(object sender, ClosingEventArgs e)
        {
       
            pfWebSvc.Stop();

        }

        // Code, der bei einem Navigationsfehler ausgeführt wird
        private void RootFrame_NavigationFailed(object sender, NavigationFailedEventArgs e)
        {
            if (Debugger.IsAttached)
            {
                // Navigationsfehler. Unterbrechen und Debugger öffnen
                Debugger.Break();
            }
        }

        // Code, der bei Ausnahmefehlern ausgeführt wird
        private void Application_UnhandledException(object sender, ApplicationUnhandledExceptionEventArgs e)
        {
            if (Debugger.IsAttached)
            {
                // Ein Ausnahmefehler ist aufgetreten. Unterbrechen und Debugger öffnen
                Debugger.Break();
            }
        }

        #region Initialisierung der Phone-Anwendung

        // Doppelte Initialisierung vermeiden
        private bool phoneApplicationInitialized = false;

        // Fügen Sie keinen zusätzlichen Code zu dieser Methode hinzu
        private void InitializePhoneApplication()
        {
            if (phoneApplicationInitialized)
                return;

            // Frame erstellen, aber noch nicht als RootVisual festlegen. Dadurch kann der Begrüßungsbildschirm
            // aktiv bleiben, bis die Anwendung bereit für das Rendern ist.
            RootFrame = new PhoneApplicationFrame();
            RootFrame.Navigated += CompleteInitializePhoneApplication;

            // Navigationsfehler behandeln
            RootFrame.NavigationFailed += RootFrame_NavigationFailed;

            // Behandeln Sie Rücksetzanforderungen zum Löschen des Backstack
            RootFrame.Navigated += CheckForResetNavigation;

            // Vertragsaktivierung wie z. B. einen "Auswahlvertrag zum Öffnen von Dateien" oder eine Dateispeicherauswahl behandeln
            PhoneApplicationService.Current.ContractActivated += Application_ContractActivated;

            // Sicherstellen, dass keine erneute Initialisierung erfolgt
            phoneApplicationInitialized = true;
        }

        // Fügen Sie keinen zusätzlichen Code zu dieser Methode hinzu
        private void CompleteInitializePhoneApplication(object sender, NavigationEventArgs e)
        {
            // Visuelle Stammkomponente festlegen, sodass die Anwendung gerendert werden kann
            if (RootVisual != RootFrame)
                RootVisual = RootFrame;

            // Dieser Handler wird nicht mehr benötigt und kann entfernt werden
            RootFrame.Navigated -= CompleteInitializePhoneApplication;
        }

        private void CheckForResetNavigation(object sender, NavigationEventArgs e)
        {
            // Wenn die App eine 'Reset'-Navigation empfangen hat, müssen wir prüfen
            // bei der nächsten Navigation, um festzustellen, ob der Seitenstapel zurückgesetzt werden muss
            if (e.NavigationMode == NavigationMode.Reset)
                RootFrame.Navigated += ClearBackStackAfterReset;
        }

        private void ClearBackStackAfterReset(object sender, NavigationEventArgs e)
        {
            // Heben Sie die Registrierung des Ereignisses auf, damit es nicht erneut aufgerufen wird.
            RootFrame.Navigated -= ClearBackStackAfterReset;

            // Löschen Sie den Stapel nur bei den Navigationen "neu" (vorwärts) und "Aktualisieren"
            if (e.NavigationMode != NavigationMode.New && e.NavigationMode != NavigationMode.Refresh)
                return;

            // Löschen Sie zur Sicherstellung der UI-Konsistenz den gesamten Seitenstapel
            while (RootFrame.RemoveBackEntry() != null)
            {
                ; // unternehmen Sie nichts
            }
        }

        #endregion

        // Initialisieren Sie die Schriftart und Flussrichtung der App wie in den lokalisierten Ressourcenzeichenfolgen angegeben.
        //
        // Um sicherzustellen, dass die Schriftart der Anwendung mit den unterstützten Sprachen abgestimmt ist und dass
        // FlowDirection der einzelnen Sprachen der herkömmlichen Richtung folgt, müssen ResourceLanguage
        // und ResourceFlowDirection in jeder RESX-Datei initialisiert werden, damit sie den Werten
        // der Kultur dieser Datei entsprechen. Zum Beispiel:
        //
        // AppResources.es-ES.resx
        //    Der Wert von ResourceLanguage muss "es-ES" sein.
        //    Der Wert von ResourceFlowDirection muss "LeftToRight" sein.
        //
        // AppResources.ar-SA.resx
        //     Der Wert von ResourceLanguage muss "ar-SA" sein.
        //     Der Wert von ResourceFlowDirection muss "RightToLeft" sein.
        //
        // Weitere Informationen über die Lokalisierung von Windows Phone-Apps finden Sie unter http://go.microsoft.com/fwlink/?LinkId=262072.
        //
        private void InitializeLanguage()
        {
            try
            {
                // Legen Sie die Schriftart so fest, dass sie der Anzeigesprache entspricht, die
                // in der ResourceLanguage-Ressourcenzeichenfolge der einzelnen unterstützten Sprachen definiert ist.
                //
                // Greifen Sie auf die Schriftart der neutralen Sprache zurück, wenn die
                // Anzeigesprache des Telefons nicht unterstützt wird.
                //
                // Wenn ein Compilerfehler auftritt, fehlt ResourceLanguage in
                // der Ressourcendatei.
                RootFrame.Language = XmlLanguage.GetLanguage(AppResources.ResourceLanguage);

                // Legen Sie FlowDirection aller Elemente im Stammframe fest und zwar auf Grundlage
                // der ResourceFlowDirection-Ressourcenzeichenfolge der einzelnen
                // unterstützten Sprachen.
                //
                // Wenn ein Compilerfehler auftritt, fehlt ResourceFlowDirection in
                // der Ressourcendatei.
                FlowDirection flow = (FlowDirection)Enum.Parse(typeof(FlowDirection), AppResources.ResourceFlowDirection);
                RootFrame.FlowDirection = flow;
            }
            catch
            {
                // Tritt hier eine Ausnahme auf, liegt das wahrscheinlich entweder an
                // ResourceLangauge ist nicht korrekt auf eine unterstützte Sprache festgelegt
                // Code oder ResourceFlowDirection ist auf einen anderen Wert als LeftToRight festgelegt
                // oder RightToLeft.

                if (Debugger.IsAttached)
                {
                    Debugger.Break();
                }

                throw;
            }
        }
    }
}