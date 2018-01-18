/************************************************************/
/* Content:   User class and AuthService class              */
/* Autor:     Martin Obermeier                              */
/* Time:      23. Jan 2018                                  */
/************************************************************/
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace MyCasinoLib
{
    public enum MyCasinoUserTypes
    {
        Operator = 0,
        Gamer,
        ERROR
    }

    public class User
    {
        public int id;
        public string username;
        private string password;
        private short userType;
        public Account account = new Account();
        private ulong sessionId;

        public ulong SessionId
        {
            get { return sessionId; }
            set { sessionId = value; }
        }

        public short UserType
        {
            get { return userType; }
            set { userType = value; }
        }


        public string Username
        {
            get { return username; }
            set { username = value; }
        }

        public string Password
        {
            get { return password; }
            set { password = value; }
        }

        public User(int identification, string name, string pw, short type)
        {
            id = identification;
            Username = name;
            Password = pw;
            UserType = type;
        }

        public User(string name, int sessionid, short type)
        {
            Username = name;
            SessionId = sessionId;
            UserType = type;
        }

        public void OperatorLogout(ulong sessionId, Dictionary<Transaction, Draw> dictTransDraw, List<User> userListLoggedOn, User userOperatorCheck, out string name)
        {
            name = null;
            if (sessionId == userOperatorCheck.SessionId)
            {
                    for (int i = 0; i < dictTransDraw.Count; i++)
                    {
                        if (dictTransDraw.ElementAt(i).Key.TransType == MyCasinoTransactionTypes.BET_WAGER)
                        {
                            dictTransDraw.ElementAt(i).Key.TransType = MyCasinoTransactionTypes.CANCELED;
                            //set money of unfinished bets back and delete bets
                            foreach (User user in userListLoggedOn)
                            {
                                user.account.DelBets();
                                user.account.MoneyAmountLeft += dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                            }
                            userOperatorCheck.account.MoneyAmountLeft -= dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                        }
                    }
                //user from logged list
                name = userOperatorCheck.Username;
                userListLoggedOn.Remove(userOperatorCheck);
            }
        }

        public void GamerLogout(ulong sessionId, Dictionary<Transaction, Draw> dictTransDraw, List<User> userListLoggedOn, User userOperatorCheck, User user, out string name)
        {
            name = null;
            if (user.SessionId == sessionId)
            {
                                    for (int i = 0; i < dictTransDraw.Count; i++)
                    {
                        //set money of unfinished bets back
                        if (dictTransDraw.ElementAt(i).Key.Name == user.username &&
                            dictTransDraw.ElementAt(i).Key.TransType == MyCasinoTransactionTypes.BET_WAGER)
                        {
                            dictTransDraw.ElementAt(i).Key.TransType = MyCasinoTransactionTypes.CANCELED;
                            user.account.MoneyAmountLeft += dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                            userOperatorCheck.account.MoneyAmountLeft -= dictTransDraw.ElementAt(i).Value.DrawBet.M_setAmount;
                        }
                    
                }
                //delete bets and user from logged list
                user.account.DelBets();
                name = user.Username;
                userListLoggedOn.Remove(user);
            }
        }

    }

    public class AuthService
    {
        
        /// <summary>
        /// lock object for userlist
        /// </summary>
        private Object thisLockUserList = new Object();
        /// <summary>
        /// Single instanced list for all users 
        /// </summary>
        public static List<User> userList = new List<User>();
        private bool m_operator;
        /// <summary>
        /// Constructor: Read all users from the userfile
        /// </summary>
        public AuthService()
        {
            m_operator = false;
            ReadRegisteredUser();
        }

        bool ReadRegisteredUser()
        {
            try
            {
                //Read UserList.txt line by line
                using (FileStream fs = File.OpenRead(Environment.GetEnvironmentVariable("SystemDrive") + "\\_myCasinoData\\UserList.txt"))
                using (StreamReader sr = new StreamReader(fs))
                {

                    string line;
                    while ((line = sr.ReadLine()) != null)
                    {
                        int id = 0, type = 0;
                        string[] substring = line.Split();
                        Int32.TryParse(substring[0], out id);
                        Int32.TryParse(substring[3], out type);
                        lock (thisLockUserList)
                        {
                            userList.Add(new User(id, substring[1], substring[2], (short)type));
                        }
                    }
                }
                //Get initial money
                using (FileStream fsBal = File.OpenRead(Environment.GetEnvironmentVariable("SystemDrive") + "\\_myCasinoData\\UserBalance.txt"))
                using (StreamReader srBal = new StreamReader(fsBal))
                {
                    string line;
                    while ((line = srBal.ReadLine()) != null)
                    {
                        int money = 0;
                        string[] substring = line.Split();
                        Int32.TryParse(substring[1], out money);
                        foreach (User user in userList)
                        {
                            lock (thisLockUserList)
                            {
                                if (user.username == substring[0])
                                    user.account.MoneyAmountLeft = money;
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                return false;
            }
            return true;
        }
        public string Login(string username, string pw, out ulong sessionId, out short type, Dictionary<Transaction, Draw> dictTransDraw, out User currUser)
        {
            lock (thisLockUserList)
            {
                foreach (User user in userList)
                {
                    if (username == user.username && pw == user.Password && 0 == user.SessionId)
                    {
                        if (user.UserType == 0 && m_operator == true)
                        {
                            user.SessionId = unchecked(Convert.ToUInt32(GenerateId()));
                            sessionId = user.SessionId;
                            type = user.UserType;
                            currUser = user;
                            return "OPERATOR_ALREADY_LOGGED_IN";
                        }

                        if (user.UserType == 0 && m_operator == false)
                        {
                            m_operator = true;
                        }
                        //Read transaction information
                        user.account.ReadUserTransaction(user.username, dictTransDraw);
                        user.SessionId = (ulong)Math.Abs(unchecked(GenerateId()));
                        sessionId = user.SessionId;
                        type = user.UserType;
                        currUser = user;
                        return "S_OK";
                    }

                }
            }
            sessionId = 0;
            type = 2;
            lock (thisLockUserList)
            {
                foreach (User user in userList)
                {
                    if (username == user.username && pw == user.Password && 0 != user.SessionId)
                    {
                        currUser = null;
                        return "ALREADY_LOGGED_IN";
                    }
                }
            }
            currUser = null;
            return "WRONG_USERNAME_OR_PASSWORD";
        }
        private int GenerateId()
        {
            byte[] buff = Guid.NewGuid().ToByteArray();
            return BitConverter.ToUInt16(buff, 0);
        }
        public string Logout(ulong sessionId)
        {
            //save all current amounts for all users
            using (StreamWriter sw = new StreamWriter(Environment.GetEnvironmentVariable("SystemDrive") + "\\_myCasinoData\\UserBalance.txt", false))
            {
                lock (thisLockUserList)
                {
                    foreach (User userWrite in userList)
                    {
                        sw.WriteLine(userWrite.Username.ToString() + " " + userWrite.account.MoneyAmountLeft);
                    }
                }
            }
            lock (thisLockUserList)
            {
                foreach (User user in userList)
                {

                    if (sessionId == user.SessionId && user.UserType == 0)
                    {
                        m_operator = false;
                        user.SessionId = 0;
                        return "S_OK";
                    }

                    if (sessionId == user.SessionId)
                    {
                        user.SessionId = 0;
                        return "S_OK";
                    }
                }
            }
            return "COULD_NOT_FIND_USER_LOGOUT_UNSUCCESSFUL";
        }
        public bool SessionIdCheck(ulong sessionId)
        {
            lock (thisLockUserList)
            {
                foreach (User user in userList)
                {
                    if (sessionId == user.SessionId)
                    {
                        return true;
                    }
                }
            }
            return false;
        }
    }
}
