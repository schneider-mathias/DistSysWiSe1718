﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace AuthenticationService
{
    public static class AuthService
    {
        private static List<User> _allUsers = new List<User>();

        public static void initializeAuthService()
        {
            string tempPath = Path.GetTempPath();
            tempPath += "mybay_user.txt";
            string[] allLines;

            try
            {
                allLines = System.IO.File.ReadAllLines(tempPath);
                foreach (string str in allLines)
                {
                    String[] substrings = str.Split(' ');
                    User newUser = new User(Convert.ToUInt32(substrings[0]), substrings[1], substrings[2]);

                    _allUsers.Add(newUser);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Fehler beim Einlesen der LoginDaten: " + e.Message);
                throw;
            }
        }

        public static String Login(string username, string password, out UInt32 sessionID)
        {
            foreach (User usr in _allUsers)
            {
                if (username.CompareTo(usr.Name) == 0)
                {
                    if (usr.SessionID > 0)
                    {
                        Console.WriteLine("User ist bereits eingeloggt, eine neue Session ID wird vergeben\n");
                        usr.createSessionID();
                        sessionID = usr.SessionID;
                        return "OK";
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
            return "Benutzername wurde nicht in der Datenbank gefunden";
        }

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

        public static Boolean isLoggedIn(UInt32 sessionID)
        {
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

        public User(UInt32 index, String name, String password)
        {
            this._index = index;
            this._name = name;
            this._password = password;
            this._sessionID = 0;
        }

        public UInt32 createSessionID()
        {
            this._sessionID = Convert.ToUInt32(DateTime.Now.Ticks);
            return this._sessionID;
        }

        public UInt32 logout()
        {
            this._sessionID = 0;
            return this._sessionID;
        }

        public bool PasswordCheck(string password)
        {
            if (this._password.CompareTo(password) == 0) return true;
            return false;
        }
    }
}
