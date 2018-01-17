/************************************************************/
/* Content:   LocalizedStrings class                        */
/* Autor:     Martin Obermeier                              */
/* Time:      23. Jan 2018                                  */
/************************************************************/
using MyCasinoWSPhoneClient.Resources;

namespace MyCasinoWSPhoneClient
{
    /// <summary>
    /// Bietet Zugriff auf Zeichenfolgenressourcen.
    /// </summary>
    public class LocalizedStrings
    {
        private static AppResources _localizedResources = new AppResources();

        public AppResources LocalizedResources { get { return _localizedResources; } }
    }
}