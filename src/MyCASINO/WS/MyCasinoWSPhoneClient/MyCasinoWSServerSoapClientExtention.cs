using MyCasinoWSPhoneClient.MyCasinoWSServer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
            if (e.UserState == tcs)
            {
                if (e.Cancelled) tcs.TrySetCanceled();
                else if (e.Error != null) tcs.TrySetException(e.Error);
                else tcs.TrySetResult(getResult());
                if (unregisterHandler != null) unregisterHandler();
            }
        }

        //wrapper for refresh

    }
}
