/*************************************************************************/
/*                                                                       */
/*    Inhalt:    AuthenticationService in seperate DLL                    */
/*                                                                       */
/*    Autor(en): Manuel Schlemelch                                       */
/*    Stand:     11.01.2018                                              */
/*                                                                       */
/*************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace AuthenticationService
{
    /// <summary>
    /// Authentication Service.
    /// The authentication service is reading it's setttings from a file mybay_user.txt where all the 
    /// usernames and their passwords are stored
    /// </summary>
    public static class AuthService
    {
        private static List<User> _allUsers = new List<User>();
        private static bool isInitialized = false;

        /// <summary>
        /// Initialization of the authentication service
        /// </summary>
        public static void initializeAuthService()
        {
            if (!isInitialized)
            {
                // Get path of systemdrive (most often C:\\)
                string tempPath = Path.GetPathRoot(Environment.SystemDirectory);

                tempPath += "_MyBayData\\mybay_user.txt";
                string[] allLines;

                try
                {
                    // Read the whole file in an array of strings
                    allLines = System.IO.File.ReadAllLines(tempPath);

                    // Every string is split into index, name and password and added to the list of allusers
                    foreach (string str in allLines)
                    {
                        String[] substrings = str.Split(' ');
                        User newUser = new User(Convert.ToUInt32(substrings[0]), substrings[1], substrings[2]);

                        _allUsers.Add(newUser);
                    }
                    isInitialized = true;
                }
                catch (Exception)
                {
                    Console.WriteLine("Fehler beim Einlesen der LoginDaten");
                }
            }
        }

        /// <summary>
        /// Login function for the user, if the user is passing the correct username and passwort, a new random sessionID is generated
        /// and passed back to the user
        /// </summary>
        /// <param name="username"></param>
        /// <param name="password"></param>
        /// <param name="sessionID"></param>
        /// <returns></returns>
        public static String Login(string username, string password, out UInt32 sessionID)
        {
            foreach (User usr in _allUsers)
            {
                if (username.CompareTo(usr.Name) == 0)
                {
                    if (usr.SessionID > 0)
                    {
                        usr.createSessionID();
                        sessionID = usr.SessionID;
                        return "User ist bereits eingeloggt, eine neue Session ID wird vergeben";
                    }

                    if (usr.PasswordCheck(password))
                    {
                        usr.createSessionID();
                        sessionID = usr.SessionID;
                        return "OK";
                    }
                    else {
                        sessionID = 0;
                        return "Ein falsches Passwort wurde eingegeben";
                    }
                }
            }
            sessionID = 0;
            return "Benutzername-/Passwort-Kombination wurde nicht in der Datenbank gefunden";
        }

        /// <summary>
        /// Logout function receives a sessionID and sets it back to 0
        /// sessionID 0 means, the user is not logged in
        /// </summary>
        /// <param name="sessionID"></param>
        /// <returns></returns>
        public static String Logout(UInt32 sessionID)
        {
            foreach (User usr in _allUsers)
            {
                if (usr.SessionID == sessionID)
                {
                    usr.logout();
                    if (usr.SessionID == 0) return "OK";
                }
            }
            return "Logout ist fehlgeschlagen";
        }

        /// <summary>
        /// getIndexBySessionID receives a sessionID and returns the index of the user, which is defined in the 
        /// settings-file. Since every user is getting a new sessionID with every login, all the transactions 
        /// and auctions are stored with the user index.
        /// </summary>
        /// <param name="sessionID"></param>
        /// <returns></returns>
        public static UInt32 getIndexBySessionID(UInt32 sessionID)
        {
            foreach (User usr in _allUsers)
            {
                if (usr.SessionID == sessionID)
                {
                    return usr.Index;
                }
            }
            return 0;
        }

        /// <summary>
        /// getNameByIndex receives the index of a user and returns it's name
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public static String getNameByIndex(UInt32 index)
        {
            foreach (User usr in _allUsers)
            {
                if (usr.Index == index)
                {
                    return usr.Name;
                }
            }
            return String.Empty;
        }

        /// <summary>
        /// This method gets a sessionID and checks, if this ID is valid and belongs
        /// to a current logged in user
        /// </summary>
        /// <param name="sessionID"></param>
        /// <returns></returns>
        public static Boolean isLoggedIn(UInt32 sessionID)
        {
            if (sessionID == 0) return false;

            foreach (User usr in _allUsers)
            {
                if (usr.SessionID == sessionID)
                {
                    return true;
                }
            }
            return false;
        }
    }

    /// <summary>
    /// Class for users, a user in the authentication service is 
    /// defined by its index, name, password and if the user is logged in, 
    /// the sessionID is greater than zero
    /// </summary>
    public class User
    {
        private UInt32 _index;

        public UInt32 Index
        {
            get { return _index; }
        }

        private String _name;

        public String Name
        {
            get { return _name; }
        }

        private String _password;

        private UInt32 _sessionID;

        public UInt32 SessionID
        {
            get { return _sessionID; }
        }

        /// <summary>
        /// Constructor of the class User
        /// </summary>
        /// <param name="index"></param>
        /// <param name="name"></param>
        /// <param name="password"></param>
        public User(UInt32 index, String name, String password)
        {
            this._index = index;
            this._name = name;
            this._password = password;
            this._sessionID = 0;
        }

        /// <summary>
        /// Method for creating a sessionID. A sessionID is a randomly generated 
        /// UInt32 value
        /// </summary>
        /// <returns></returns>
        public UInt32 createSessionID()
        {
            Random rand = new Random();
            this._sessionID = (uint)(rand.Next(1 << 30)) << 2 | (uint)(rand.Next(1 << 2)); 
            return this._sessionID;
        }

        /// <summary>
        /// This method logs off a user by setting its sessionID to zero
        /// </summary>
        /// <returns></returns>
        public UInt32 logout()
        {
            this._sessionID = 0;
            return this._sessionID;
        }

        /// <summary>
        /// This method receives a password and checks, if its the same as stored in the database
        /// </summary>
        /// <param name="password"></param>
        /// <returns></returns>
        public bool PasswordCheck(string password)
        {
            if (this._password.CompareTo(password) == 0) return true;
            return false;
        }
    }
}
