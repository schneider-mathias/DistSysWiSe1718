#define PORTFWDLIBACTIVE

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
#if PORTFWDLIBACTIVE
        PortForwarder pf = new PortForwarder(3333, 3332);
#endif
        protected void Application_Start(object sender, EventArgs e)
        {
#if PORTFWDLIBACTIVE
            pf.Run();
#endif
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
#if PORTFWDLIBACTIVE
            pf.Stop();
#endif
        }
    }
}