/*************************************************************************/
/*                                                                       */
/*    Inhalt:    Program.cs des MyBay WCF-Servers                        */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     03.01.2018                                              */
/*                                                                       */
/*************************************************************************/

#define SINGLETON

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ServiceModel;
using AuthenticationService;

namespace MyBayWCFSrv
{
    class Program
    {
        static void Main(string[] args)
        {
#if SINGLETON

            // Create myBaySrv - Objekt (Singleton)
            MyBayWCFSrv myBaySrv = new MyBayWCFSrv();

            // Servicehost zur Objektveröffentlichung anlegen

            //ServiceHost svh = new ServiceHost(myInventSrv, new Uri("http://192.168.52.210:3333/MyBayWCF"));
            ServiceHost serviceh = new ServiceHost(myBaySrv, new Uri("http://localhost:3333/MyBayWCF"));

            // Einstellungen für ein Singleton mit mehreren Clients vornehmen
            ServiceBehaviorAttribute serviceBehaviourAttrib = serviceh.Description.Behaviors.Find<ServiceBehaviorAttribute>();

            serviceBehaviourAttrib.ConcurrencyMode = ConcurrencyMode.Multiple;

            serviceBehaviourAttrib.InstanceContextMode = InstanceContextMode.Single;

#else
            ServiceHost serviceh = new ServiceHost(typeof(MyBayWCFSrv), new Uri("http://192.168.52.210:3333"));

            svh.AddServiceEndpoint(typeof(INETMyInvent), new WSHttpBinding(), "http://192.168.52.210:3333/MyBayWCF");
#endif
            // MyBay Server-Objekt veröffentlichen
            serviceh.Open();

            // MyBay Server-Objekt ist jetzt nach Außen verfügbar
            Console.ReadLine();

            // Veröffentlichung des MyBay Server-Objekt rückgängig machen
            serviceh.Close();
        }
    }
}
