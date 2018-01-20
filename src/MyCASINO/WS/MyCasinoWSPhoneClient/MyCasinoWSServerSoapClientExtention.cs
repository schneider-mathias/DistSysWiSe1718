/***************************************************************************************************************/
/* Content:   MyCasinoWSServerSoapClientExtensions                                                             */
/* Autor:     Martin Obermeier                                                                                 */
/* Time:      23. Jan 2018                                                                                     */
/* Structure from: https://stackoverflow.com/questions/14103288/how-can-i-use-async-await-to-call-a-webservice */
/***************************************************************************************************************/
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
                client.getTransactionsCompleted += (sender, e) => TransferCompletion(taskCompletionSource, e, () => e, null); ;
                client.getTransactionsAsync(sessionId, taskCompletionSource);      
            return taskCompletionSource.Task;
        }


        private static void TransferCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.getTransactionsCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            //check transmission error
            transmissionError(e);
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
                client.getTransactionInformationCompleted += (sender, e) => TransferInformationCompletion(taskCompletionSource, e, () => e, null); ;
                client.getTransactionInformationAsync(sessionId, idTrans, taskCompletionSource);
            return taskCompletionSource.Task;
        }

        private static void TransferInformationCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.getTransactionInformationCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            //check transmission error
            transmissionError(e);
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
                client.loginCompleted += (sender, e) => LoginCompletion(taskCompletionSource, e, () => e, null); ;
                client.loginAsync(username, password, taskCompletionSource);
            return taskCompletionSource.Task;
        }


        private static void LoginCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.loginCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            //check transmission error
            transmissionError(e);
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
                client.betCompleted += (sender, e) => BetCompletion(taskCompletionSource, e, () => e, null); ;
                client.betAsync(sessionId, amountMoney, firstNumber, secondNumber, taskCompletionSource);
            return taskCompletionSource.Task;
        }


        private static void BetCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.betCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            //check transmission error
            transmissionError(e);
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
                client.showbetsCompleted += (sender, e) => ShowbetsCompletion(taskCompletionSource, e, () => e, null); ;
                client.showbetsAsync(sessionId, taskCompletionSource);
            return taskCompletionSource.Task;
        }


        private static void ShowbetsCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.showbetsCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            //check transmission error
            transmissionError(e);
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
                client.calculateProfitCompleted += (sender, e) => CalculateProfitCompletion(taskCompletionSource, e, () => e, null); ;
                client.calculateProfitAsync(sessionId, amountMoney, firstNumber, secondNumber, taskCompletionSource);
            return taskCompletionSource.Task;
        }


        private static void CalculateProfitCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.calculateProfitCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            //check transmission error
            transmissionError(e);
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
                client.drawCompleted += (sender, e) => DrawCompletion(taskCompletionSource, e, () => e, null); ;
                client.drawAsync(sessionId, taskCompletionSource);
            return taskCompletionSource.Task;
        }

        private static void DrawCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.drawCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            //check transmission error
            transmissionError(e);
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
                client.drawtestCompleted += (sender, e) => DrawTestCompletion(taskCompletionSource, e, () => e, null); ;
                client.drawtestAsync(sessionId, firstNumberTest, secondNumberTest, taskCompletionSource);
            return taskCompletionSource.Task;
        }

        private static void DrawTestCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.drawtestCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            //check transmission error
            transmissionError(e);
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
                client.logoutCompleted += (sender, e) => LogoutCompletion(taskCompletionSource, e, () => e, null); ;
                client.logoutAsync(sessionId, taskCompletionSource);
            return taskCompletionSource.Task;
        }

        private static void LogoutCompletion<T>(
            TaskCompletionSource<T> tcs, MyCasinoWSServer.logoutCompletedEventArgs e,
            Func<T> getResult, Action unregisterHandler)
        {
            //check transmission error
            transmissionError(e);
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }

        private static void transmissionError(System.ComponentModel.AsyncCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                //check if an error occurred
                if (e.Error.GetType() == typeof(System.ServiceModel.CommunicationException))
                {
                    MessageBox.Show("Es konnte keine Verbindung zum Server gefunden werden!", "Error", MessageBoxButton.OK);
                    Application.Current.Terminate();
                }
                else
                {
                    MessageBox.Show("Es ist ein unbekannter Fehler beim Verbindungsversuch aufgetreten", "Error", MessageBoxButton.OK);
                    Application.Current.Terminate();
                }
            }
        }
    }
}
