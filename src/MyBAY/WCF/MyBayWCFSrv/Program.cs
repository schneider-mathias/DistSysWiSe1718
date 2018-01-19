/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Program.cs des MyBay WCF-Servers                        */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     12.01.2018                                              */
/*                                                                       */
/*************************************************************************/

using System;
using System.ServiceModel;

namespace MyBayWCFSrv
{
    class Program
    {
        /// <summary>
        /// Main method for the WCF server
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            // Create myBaySrv object (Singleton)
            MyBayWCFSrv myBaySrv = new MyBayWCFSrv();

            // Create service host for publishing object
            ServiceHost serviceh = new ServiceHost(myBaySrv, new Uri("http://localhost:3333/MyBayWCF"));

            // Setting the settings for a singleton object and multiple clients
            ServiceBehaviorAttribute serviceBehaviourAttrib = serviceh.Description.Behaviors.Find<ServiceBehaviorAttribute>();
            serviceBehaviourAttrib.ConcurrencyMode = ConcurrencyMode.Multiple;
            serviceBehaviourAttrib.InstanceContextMode = InstanceContextMode.Single;

            // publish the object
            serviceh.Open();

            // wait for user input to close the server
            Console.ReadLine();

            // close the object
            serviceh.Close();
        }
    }
}
