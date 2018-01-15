using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Security;
using System.Web.SessionState;
using PortForwardLib;

namespace MyBayWSSrv
{
    public class Global : System.Web.HttpApplication
    {
        PortForwarder pf = new PortForwarder(7776, 7777);

        protected void Application_Start(object sender, EventArgs e)
        {
            pf.Run();

        }

        protected void Session_Start(object sender, EventArgs e)
        {

        }

        protected void Application_BeginRequest(object sender, EventArgs e)
        {

        }

        protected void Application_AuthenticateRequest(object sender, EventArgs e)
        {

        }

        protected void Application_Error(object sender, EventArgs e)
        {

        }

        protected void Session_End(object sender, EventArgs e)
        {

        }

        protected void Application_End(object sender, EventArgs e)
        {
            pf.Stop();

        }
    }
}