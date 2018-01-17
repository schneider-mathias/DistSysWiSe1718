using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyBayWCFCln
{
    static class comError
    {
        static public bool convertException(Exception ex, out string retString)
        {
            retString = "";
            switch ((UInt32)ex.HResult)
            {
                case 0xA0110001: // ERROR_AUCTIONNUMBER_NOT_AVAILABLE
                    retString = "Die angegebene Auktionsnummer ist nicht verfügbar";
                    break;
                case 0xA0110002: // ERROR_USER_ALREADY_INTERESTED
                    retString = "Sie verfolgen bereits den Status dieser Auktion";
                    break;
                // Bid
                case 0xA0110003: // ERROR_AUCTIONNUMBER_DOES_NOT_ESXIST
                    retString = "Die angegebene Auktionsnummer ist nicht verfügbar";
                    break;
                case 0xA0110004: // ERROR_AUCTION_CLOSED
                    retString = "Die Auktion ist beendet";
                    break;
                case 0xA0110005: // ERROR_BID_TOO_LOW
                    retString = "Das angegebene Gebot ist zu niedrig";
                    break;
                case 0xA0110013: // ERROR_BID_NEGATIVE
                    retString = "Das eingegebene Gebot ist negativ";
                    break;
                // Login
                case 0xA0110006: // ERROR_ALREADY_LOGGED_IN
                    retString = "Sie sind bereits eingeloggt";
                    break;
                case 0xA0110007: // ERROR_FILE_COULD_NOT_BE_OPENED
                    retString = "Die Datei konnte nicht geöffnet werden";
                    break;
                case 0xA0110008: // ERROR_USERNAME_OR_PASSWORD_WRONG
                    retString = "Der Benutzername oder das Passwort sind falsch";
                    break;
                case 0xA0110009: // ERROR_USER_NOT_LOGGED_IN
                    retString = "Der Benutzer ist nicht eingeloggt";
                    break;
                // Offer
                case 0xA0110010: // ERROR_ARTICLENAME_IS_EMPTY
                    retString = "Der Artikelname darf nicht leer sein";
                    break;
                case 0xA0110014: // ERROR_VALUE_NEGATIVE
                    retString = "Das Startgebot darf nicht negativ sein";
                    break;
                // getAuctions
                case 0xA0110011: // ERROR_NO_AUCTIONS_AVAILABLE
                    retString = "Es sind keine Auktionen verfügbar";
                    break;
                // Details
                case 0xA0110012: // ERROR_USER_IS_NOT_AUCTIONEER
                    retString = "Der angemeldete Benutzer ist nicht der Auktionator dieser Auktion";
                    break;
                default:
                    retString = "Unbekannte COM Exception";
                    // the return value is false, if the convert function doesnt know the exception, to write a different message text for each 
                    // catch, where an exception is catched
                    return false;
            }
            return true;
        }
    }
}
