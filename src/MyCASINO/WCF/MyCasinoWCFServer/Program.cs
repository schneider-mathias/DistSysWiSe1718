/************************************************************/
/* Content:   Program                                       */
/* Autor:     Martin Obermeier                              */
/* Time:      23. Jan 2018                                  */
/************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace MyCasinoWCFServer
{
    /// <summary>
    /// WCF-Server main program
    /// </summary>

    class Program
    {
        static void Main(string[] args)
        {

            //Server-Object creation 
            MyCasinoWCFServerImplementation myCasinoSrv = new MyCasinoWCFServerImplementation();

            ServiceHost svh = new ServiceHost(myCasinoSrv, new Uri("http://localhost:1200/MyCasinoWCF"));

            //Set properties for a Singleton with more than one Client
            ServiceBehaviorAttribute sba = svh.Description.Behaviors.Find<ServiceBehaviorAttribute>();
            sba.InstanceContextMode = InstanceContextMode.Single;
            sba.ConcurrencyMode = ConcurrencyMode.Multiple;

            //Publish MyCasino Server-Object
            svh.Open();

            //Server is now available
            Console.ReadLine();

            //Reverse publishing of MyCasino Server-Object
            svh.Close();
        }
    }
}
