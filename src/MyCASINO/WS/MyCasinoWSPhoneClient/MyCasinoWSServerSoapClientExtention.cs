using MyCasinoWSPhoneClient.MyCasinoWSServer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace MyCasinoWSPhoneClient
{
    public static partial class MyCasinoWSServerSoapClient
    { 
        //wrapper to synchronise asynchron calls

            //generic function
        private static TaskCompletionSource<T> CreateSource<T>(object state)
        {
            return new TaskCompletionSource<T>(
                state, TaskCreationOptions.None);
        }

        //wrapper for Transaction 
        public static Task<MyCasinoWSServer.getTransactionsCompletedEventArgs> GetTransactionAsyncTask(this MyCasinoWSServer.MyCasinoWSServerSoapClient client, int sessionId)
        {
            var taskCompletionSource = CreateSource<MyCasinoWSServer.getTransactionsCompletedEventArgs>(null);
            try
            {
                client.getTransactionsCompleted += (sender, e) => TransferCompletion(taskCompletionSource, e, () => e, null); ;
                client.getTransactionsAsync(sessionId, taskCompletionSource);
            }
            catch
            {
                MessageBox.Show("Fehler beim Transaction übertragen: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            }
            return taskCompletionSource.Task;
        }


        private static void TransferCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.getTransactionsCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }

        //wrapper for Transaction Information
        public static Task<MyCasinoWSServer.getTransactionInformationCompletedEventArgs> GetTransactionInformationAsyncTask(this MyCasinoWSServer.MyCasinoWSServerSoapClient client, int sessionId, int idTrans)
        {
            var taskCompletionSource = CreateSource<MyCasinoWSServer.getTransactionInformationCompletedEventArgs>(null);
            try
            {
                client.getTransactionInformationCompleted += (sender, e) => TransferInformationCompletion(taskCompletionSource, e, () => e, null); ;
                client.getTransactionInformationAsync(sessionId, idTrans, taskCompletionSource);
            }
            catch
            {
                MessageBox.Show("Fehler beim Transaction Information abholen: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            }
            return taskCompletionSource.Task;
        }

        private static void TransferInformationCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.getTransactionInformationCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }

        //wrapper for login

        public static Task<MyCasinoWSServer.loginCompletedEventArgs> GetLoginAsyncTask(this MyCasinoWSServer.MyCasinoWSServerSoapClient client, string username, string password)
        {
            var taskCompletionSource = CreateSource<MyCasinoWSServer.loginCompletedEventArgs>(null);
            try
            {
                client.loginCompleted += (sender, e) => LoginCompletion(taskCompletionSource, e, () => e, null); ;
                client.loginAsync(username, password, taskCompletionSource);
            }
            catch
            {
                MessageBox.Show("Fehler beim Login: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            }
            return taskCompletionSource.Task;
        }


        private static void LoginCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.loginCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }

        //wrapper for bet
        public static Task<MyCasinoWSServer.betCompletedEventArgs> GetBetAsyncTask(this MyCasinoWSServer.MyCasinoWSServerSoapClient client, int sessionId, double amountMoney, int firstNumber, int secondNumber)
        {
            var taskCompletionSource = CreateSource<MyCasinoWSServer.betCompletedEventArgs>(null);
            try
            {
                client.betCompleted += (sender, e) => BetCompletion(taskCompletionSource, e, () => e, null); ;
                client.betAsync(sessionId, amountMoney, firstNumber, secondNumber, taskCompletionSource);
            }
            catch
            {
                MessageBox.Show("Fehler beim Wetten: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            }
            return taskCompletionSource.Task;
        }


        private static void BetCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.betCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }

        //wrapper for showbets
        public static Task<MyCasinoWSServer.showbetsCompletedEventArgs> ShowbetsAsyncTask(this MyCasinoWSServer.MyCasinoWSServerSoapClient client, int sessionId)
        {
            var taskCompletionSource = CreateSource<MyCasinoWSServer.showbetsCompletedEventArgs>(null);
            try
            {
                client.showbetsCompleted += (sender, e) => ShowbetsCompletion(taskCompletionSource, e, () => e, null); ;
                client.showbetsAsync(sessionId, taskCompletionSource);
            }
            catch
            {
                MessageBox.Show("Fehler beim anzeigen der Wetten: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            }
            return taskCompletionSource.Task;
        }


        private static void ShowbetsCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.showbetsCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }

        //wrapper for calculateprofit
        public static Task<MyCasinoWSServer.calculateProfitCompletedEventArgs> CalculateProfitAsyncTask(this MyCasinoWSServer.MyCasinoWSServerSoapClient client, int sessionId, double amountMoney, int firstNumber, int secondNumber)
        {
            var taskCompletionSource = CreateSource<MyCasinoWSServer.calculateProfitCompletedEventArgs>(null);
            try
            {
                client.calculateProfitCompleted += (sender, e) => CalculateProfitCompletion(taskCompletionSource, e, () => e, null); ;
                client.calculateProfitAsync(sessionId, amountMoney, firstNumber, secondNumber, taskCompletionSource);
            }
            catch
            {
                MessageBox.Show("Fehler bei der Gewinn kalkulation: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            }
            return taskCompletionSource.Task;
        }


        private static void CalculateProfitCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.calculateProfitCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }

        //wrapper for draw
        public static Task<MyCasinoWSServer.drawCompletedEventArgs> DrawAsyncTask(this MyCasinoWSServer.MyCasinoWSServerSoapClient client, int sessionId)
        {
            var taskCompletionSource = CreateSource<MyCasinoWSServer.drawCompletedEventArgs>(null);
            try
            {
                client.drawCompleted += (sender, e) => DrawCompletion(taskCompletionSource, e, () => e, null); ;
                client.drawAsync(sessionId, taskCompletionSource);
            }
            catch
            {
                MessageBox.Show("Fehler beim Ziehen: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            }
            return taskCompletionSource.Task;
        }

        private static void DrawCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.drawCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }

        //wrapper for drawtest

            public static Task<MyCasinoWSServer.drawtestCompletedEventArgs> DrawTestAsyncTask(this MyCasinoWSServer.MyCasinoWSServerSoapClient client, int sessionId, int firstNumberTest, int secondNumberTest)
        {
            var taskCompletionSource = CreateSource<MyCasinoWSServer.drawtestCompletedEventArgs>(null);
            try
            {
                client.drawtestCompleted += (sender, e) => DrawTestCompletion(taskCompletionSource, e, () => e, null); ;
                client.drawtestAsync(sessionId, firstNumberTest, secondNumberTest, taskCompletionSource);
            }
            catch
            {
                MessageBox.Show("Fehler beim Ziehen mit Zahlen: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            }
            return taskCompletionSource.Task;
        }

        private static void DrawTestCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.drawtestCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }

        //wrapper for logout

        public static Task<MyCasinoWSServer.logoutCompletedEventArgs> LogoutAsyncTask(this MyCasinoWSServer.MyCasinoWSServerSoapClient client, int sessionId)
        {
            var taskCompletionSource = CreateSource<MyCasinoWSServer.logoutCompletedEventArgs>(null);
            try
            {
                client.logoutCompleted += (sender, e) => LogoutCompletion(taskCompletionSource, e, () => e, null); ;
                client.logoutAsync(sessionId, taskCompletionSource);
            }
            catch
            {
                MessageBox.Show("Fehler beim Logout: Server nicht gefunden", "Webservices Fehler", MessageBoxButton.OK);
            }
            return taskCompletionSource.Task;
        }

        private static void LogoutCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.logoutCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }
    }
}
