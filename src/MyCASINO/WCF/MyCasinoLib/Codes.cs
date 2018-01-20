/************************************************************/
/* Content:   Codes class                                   */
/* Autor:     Martin Obermeier                              */
/* Time:      23. Jan 2018                                  */
/************************************************************/
using System;
using System.Runtime.InteropServices;

namespace MyCasinoLib
{
    /// <summary>
    /// https://limbioliong.wordpress.com/2011/06/16/returning-strings-from-a-c-api/
    /// </summary>
    public class Codes
    {
        [DllImport("MyCasinoDefines.dll", EntryPoint = "ResolveCode", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        public static extern string ResolveCode(long code);
    }
}
