#include "MyBay_i_h.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "CharStringConverter.h"
#include "RPCMyBaySrvImpl.h"
#include "MyBayDefines.h"
//#include "CritSectionWrapper.h"
//#include"HelperFunktions.h"

using namespace std;


// Globale Variablen
//users* userList;
long numUser = 0;
unsigned long numAuct = 0;
//CritSectionWrapper critSecWrapper;

error_status_t login(unsigned char *username, unsigned char *password, unsigned long* sessionId)
{
	wifstream csvread;
	srand((unsigned)time(NULL)); // Zufallsgenerator initialisieren.

	/* Abfrage ob User nicht schon eingeloggt ist.*/
	if (BOOL isLoggedIn = loginCheck(*username) == TRUE)
	{
		//cout << "Sie sind bereits angemeldet" << endl;
		return ERROR_ALREADY_LOGGED_IN;
	}
	
	csvread.open("user.csv", ios::in);
	if (csvread) {
		wstring fline, fname, fpassword;
		wstring suser = char_to_wstring((char*)username);
		wstring spassword = char_to_wstring((char*)password);
		while (getline(csvread, fline))
		{
			wstring delimiter = L";";
			size_t delimPos = fline.find(delimiter);
			fname = fline.substr(0, delimPos);
			fline.erase(0, delimPos + delimiter.length());
			fpassword = fline;

			if (!(suser.compare(fname) || spassword.compare(fpassword)))
			{
				*sessionId = rand();
				users.insert(std::make_pair(fname, *sessionId));
				return RPC_S_OK;
			}
		}
		csvread.close();
	}
	else
		return ERROR_FILE_COULD_NOT_BE_OPENED;

	return ERROR_USERNAME_OR_PASSWORD_WRONG;
}

error_status_t logout(unsigned long sessionId)
{
	//überprüfen ob user angemeldet und dann aus Liste entfernen
	std::map<std::wstring, unsigned long>::iterator it;
	for (it = users.begin(); it != users.end(); ++it)
	{
		if (it->second == sessionId)
		{
			users.erase(it);
			return RPC_S_OK;
		}
	}
	return ERROR_USER_NOT_LOGGED_IN; //user nicht angemeldet
}

error_status_t offer(unsigned long sessionId, unsigned char *articleName, double startBid, unsigned long* auctionNumber)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);

	wstring sarticleName = char_to_wstring((char*)articleName);
	if (sarticleName == L"")		// Artikelname darf nicht leer sein
	{
		return ERROR_ARTICLENAME_IS_EMPTY;
	}
	
	// neue Auktion erzeugen
	unsigned long newAuctNumb = addNewAuction(sarticleName, startBid);
	// User der Liste der interessierten User für diese Auktion hinzufügen
	BOOL userAdded = addUserToInterestedUserList(user, newAuctNumb);

	// TODO: Schreibe Auctionen in Datei um persistent zu halten (in .h schon vorhanden) 
	writeAuctionToFile();

	*auctionNumber = newAuctNumb;		// Auktionsnummer zurückliefern
	
	return RPC_S_OK;
}

error_status_t interested(unsigned long sessionId, unsigned long auctionNumber)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);

	// Prüft, ob Auktionsnummer vorhanden
	BOOL auctionAvailable = checkAuctionNumber(auctionNumber);
	if (auctionAvailable == FALSE)						// Auktion nicht vorhanden
	{
		return ERROR_AUCTIONNUMBER_NOT_AVAILABLE;
	}
	BOOL userAdded = addUserToInterestedUserList(user, auctionNumber);
	if (userAdded == FALSE)								// bereits interessiert
	{
		return ERROR_USER_ALREADY_INTERESTED;
	}
	return RPC_S_OK;
}

error_status_t getAuctions(unsigned long sessionId, unsigned long flags, unsigned char *articleName, unsigned long* countAuctions, String_t* auctions)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);
	wstring sarticleName = char_to_wstring((char*)articleName);

	// Gebote serialisieren
	vector<auction> interestingAuctions = getInterestingAuctions(user, flags, sarticleName);
	if (interestingAuctions.size() == 0)
	{
		return NO_AUCTIONS_AVAILABLE;
	}
	wstring serStr = serializeAuctions(interestingAuctions);

	int serStrLen = serStr.size();
	auctions->str = (unsigned char*)midl_user_allocate(10000);			//TODO: Größe noch berechnen 
	auctions->size = serStrLen;
	auctions->len = serStrLen;

	strcpy((char*)auctions->str, (char*)wstring_to_char(serStr.c_str()));

	*countAuctions = interestingAuctions.size();

	return RPC_S_OK;
}

error_status_t bid(unsigned long sessionId, unsigned long auctionNumber, double bidVal)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);
	wstring articleName = getArticleName(auctionNumber);

	if (articleName == L"")		// Auktionsnummer nicht vorhanden
	{
		return ERROR_AUCTIONNUMBER_DOES_NOT_ESXIST;
	}
	int auctionState = getAuctionState(auctionNumber);
	if (auctionState == 2)		// Auktion geschlossen
	{
		return ERROR_AUCTION_CLOSED;
	}
	BOOL bidSuccessfull = addBidToAuction(auctionNumber, bidVal, user);
	if (bidSuccessfull == FALSE)	// Gebot muss höher als Startgebot und Höchstgebot sein
	{
		return ERROR_BID_TOO_LOW;
	}
	addNewBidToMessages(articleName, bidVal, user);

	return RPC_S_OK;
}

error_status_t details(unsigned long sessionId, unsigned long auctionNumber, String_t *allBids, unsigned long* countBids)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);

	// Check ob User die Auktion selbst gestartet hat
	BOOL usersAuction = userCreatedAuctioncheck(user, auctionNumber);
	if (usersAuction == FALSE)		
	{
		return ERROR_USER_IS_NOT_AUCTIONEER;
	}

	// Gebote serialisieren
	wstring serStr = serializeAuctionDetails(auctionNumber);

	int serStrLen = serStr.size();
	allBids->str = (unsigned char*)midl_user_allocate(10000);			//TODO: Größe noch berechnen 
	allBids->size = serStrLen;
	allBids->len = serStrLen;
	
	strcpy((char*)allBids->str, (char*)wstring_to_char(serStr.c_str()));

	// Anzahl der Auktionen bestimmen
	*countBids = countBidsOfAuction(auctionNumber);

	return RPC_S_OK;
}

error_status_t endauction(unsigned long sessionId, unsigned long auctionNumber)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);
	BOOL userIsAuctioneer = userCreatedAuctioncheck(user, auctionNumber);
	if (userIsAuctioneer == FALSE)
		return ERROR_USER_IS_NOT_AUCTIONEER;
	BOOL endAuctionSuccessfull = endAuction(user, auctionNumber);

	return RPC_S_OK;
}

error_status_t getMessage(unsigned long sessionId, boolean* messageAvailable, unsigned long* messageType, String_t* message)
{
	//addNewBidToMessages(L"car", 100); // TEST
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);
	int msgCnt = countMessages(user);
	// eine neue Nachricht für den User wurde gefunden
	if (msgCnt > 0)
	{
		vector<wstring> newMessage = searchForMessage(user);				// Neue Nachticht für den user suchen
		*messageType = stoul(newMessage.at(1));								// MessageType festlegen
		// Nachricht serialisieren
		wstring serStr = serializeMessage(newMessage);

		message->str = (unsigned char*)midl_user_allocate(10000);			// TODO: Größe noch berechnen 
		message->size = 10000;
		message->len = 10000;

		strcpy((char*)message->str, (char*)wstring_to_char(serStr.c_str()));
	}

	// Falls noch mehr Nachrichten für den Nutzer bereit liegen, wird der User mit messageAvailable darüber informiert.
	// Damit weiß er, dass er nochmal Pullen muss um sich die restlichen Nachrichten abzuholen
	// und nicht wartet bis der Timer bis zum nächsten Pull abgelaufen ist.
	if (msgCnt > 1)
	{
		*messageAvailable = TRUE;
	}
	else
		*messageAvailable = FALSE;
	

	return RPC_S_OK;
}