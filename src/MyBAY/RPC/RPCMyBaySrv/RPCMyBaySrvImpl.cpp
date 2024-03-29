/************************************************************/
/*                                                          */
/* Inhalt:    Serverimplementierung				            */
/*                                                          */
/* Autor:	  Johannes Sauer		                        */
/* Stand:     23. Jan 2018                                  */
/*															*/
/************************************************************/

#pragma once
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

using namespace std;


// Globale Variablen
long numUser = 0;
unsigned long numAuct = 0;

/// <summary>
/// Login
/// </summary>
/// <param name="username"> Username </param>
/// <param name="password"> Passwort </param>
/// <param name="sessionId"> SessionId des Users </param>
/// <returns> Error Status </returns>
error_status_t login(unsigned char *username, unsigned char *password, unsigned long* sessionId)
{
	wifstream csvread;
	srand((unsigned)time(NULL)); // Zufallsgenerator initialisieren.

	// Wenn sich der erste Nutzer einloggt, werden die persistent gespeicherten Auktionen aus der Datei ausgelesen
	if (users.size() == 0)
	{
		AuctionList.clear();
		readAuctionsFromFile();
	}

	// Pr�fen ob User bereits eingeloggt ist. Return sessionId falls User bereits eingeloggt ist, sonst 0
	unsigned long existSessionId = loginCheck(username);
	if (existSessionId != 0)
	{
		*sessionId = existSessionId;
		return RPC_S_OK;
	}
	
	// User-File einlesen
	csvread.open("C:/_MyBayData/user.csv", ios::in);
	if (csvread) {
		wstring fline, fname, fpassword;
		wstring suser = char_to_wstring((char*)username);
		wstring spassword = char_to_wstring((char*)password);

		// in jeder Zeile pr�fen ob der Username und Passwort mit den Eingegebenen �bereinstimmen
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

/// <summary>
/// Logout
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <returns> Error Status </returns>
error_status_t logout(unsigned long sessionId)
{
	//�berpr�fen ob user angemeldet und dann aus Liste entfernen
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

/// <summary>
/// Auktion starten
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="articleName"> Artikelname </param>
/// <param name="startBid"> Startgebot </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Error Status </returns>
error_status_t offer(unsigned long sessionId, unsigned char *articleName, double startBid, unsigned long* auctionNumber)
{
	// Pr�fen ob User eingeloggt ist
	if (BOOL isLoggedIn = loggedInCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);

	wstring sarticleName = char_to_wstring((char*)articleName);
	if (sarticleName == L"")		// Artikelname darf nicht leer sein
	{
		return ERROR_ARTICLENAME_IS_EMPTY;
	}
	// auf negatives Startgebot pr�fen
	if (startBid < 0)
	{
		return ERROR_VALUE_NEGATIVE;
	}
	
	// neue Auktion erzeugen
	unsigned long newAuctNumb = addNewAuction(sarticleName, startBid);
	// User der Liste der interessierten User f�r diese Auktion hinzuf�gen
	BOOL userAdded = addUserToInterestedUserList(user, newAuctNumb);
	
	// aktualisiert die MyBayAuctions.txt
	writeAuctionsToFile();

	*auctionNumber = newAuctNumb;		// Auktionsnummer zur�ckliefern
	
	return RPC_S_OK;
}

/// <summary>
/// Auktion starten
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Error Status </returns>
error_status_t interested(unsigned long sessionId, unsigned long auctionNumber)
{
	// Pr�fen ob User eingeloggt ist
	if (BOOL isLoggedIn = loggedInCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);

	// Pr�ft, ob Auktionsnummer vorhanden
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

	// aktualisiert die MyBayAuctions.txt
	writeAuctionsToFile();

	return RPC_S_OK;
}

/// <summary>
/// Liste der Auktionen, die den Eingaben entsprechen
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="flags"> Option, ob -a oder -A eingegeben wurden </param>
/// <param name="articleName"> Artikelname </param>
/// <param name="countAuctions"> Auktionsnummer </param>
/// <param name="auctions"> Liste der Auktionen serialisiert </param>
/// <returns> Error Status </returns>
error_status_t getAuctions(unsigned long sessionId, unsigned long flags, unsigned char *articleName, unsigned long* countAuctions, String_t* auctions)
{
	// Pr�fen ob User eingeloggt ist
	if (BOOL isLoggedIn = loggedInCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);
	wstring sarticleName = char_to_wstring((char*)articleName);

	// Gebote serialisieren
	vector<auction> interestingAuctions = getInterestingAuctions(user, flags, sarticleName);
	if (interestingAuctions.size() == 0)
	{
		return ERROR_NO_AUCTIONS_AVAILABLE;
	}
	wstring serStr = serializeAuctions(interestingAuctions);

	// Speicher f�r die �bertragung des String_t allokieren
	int serStrSize = sizeof(char) * (serStr.size() + 1);
	auctions->str = (unsigned char*)midl_user_allocate(serStrSize);			
	auctions->size = serStrSize;
	auctions->len = serStrSize;

	strcpy((char*)auctions->str, (char*)wstring_to_char(serStr.c_str()));


	*countAuctions = interestingAuctions.size();

	return RPC_S_OK;
}

/// <summary>
/// Gebot abgeben
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <param name="bidVal"> Gebot </param>
/// <returns> Error Status </returns>
error_status_t bid(unsigned long sessionId, unsigned long auctionNumber, double bidVal)
{
	// Pr�fen ob User eingeloggt ist
	if (BOOL isLoggedIn = loggedInCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);
	wstring articleName = getArticleName(auctionNumber);

	if (articleName == L"")		// Auktionsnummer nicht vorhanden
	{
		return ERROR_AUCTIONNUMBER_DOES_NOT_ESXIST;
	}
	if (bidVal < 0)
	{
		return ERROR_BID_NEGATIVE;
	}
	int auctionState = getAuctionState(auctionNumber);
	if (auctionState == 2)		// Auktion geschlossen
	{
		return ERROR_AUCTION_CLOSED;
	}
	BOOL bidSuccessfull = addBidToAuction(auctionNumber, bidVal, user);
	if (bidSuccessfull == FALSE)	// Gebot muss h�her als Startgebot und H�chstgebot sein
	{
		return ERROR_BID_TOO_LOW;
	}
	addNewBidToMessages(auctionNumber, bidVal, user);

	// aktualisiert die MyBayAuctions.txt
	writeAuctionsToFile();

	return RPC_S_OK;
}

/// <summary>
/// Alle Gebote einer Auktion
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <param name="allBids"> Alle Gebote der Auktion serialisiert </param>
/// <param name="countBids"> Anzahl der Gebote </param>
/// <returns> Error Status </returns>
error_status_t details(unsigned long sessionId, unsigned long auctionNumber, String_t *allBids, unsigned long* countBids)
{
	// Pr�fen ob User eingeloggt ist
	if (BOOL isLoggedIn = loggedInCheck(sessionId) == FALSE)
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

	// Speicher f�r die �bertragung des String_t allokieren
	int serStrSize = sizeof(char) * (serStr.size() + 1);
	allBids->str = (unsigned char*)midl_user_allocate(serStrSize);			
	allBids->size = serStrSize;
	allBids->len = serStrSize;
	
	strcpy((char*)allBids->str, (char*)wstring_to_char(serStr.c_str()));

	// Anzahl der Auktionen bestimmen
	*countBids = countBidsOfAuction(auctionNumber);

	return RPC_S_OK;
}

/// <summary>
/// Auktion beenden
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Error Status </returns>
error_status_t endauction(unsigned long sessionId, unsigned long auctionNumber)
{
	// Pr�fen ob User eingeloggt ist
	if (BOOL isLoggedIn = loggedInCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);
	BOOL auctionStatusOpen = auctionStatusCheck(auctionNumber);
	if (auctionStatusOpen == FALSE)
		return ERROR_AUCTION_CLOSED;
	BOOL userIsAuctioneer = userCreatedAuctioncheck(user, auctionNumber);
	if (userIsAuctioneer == FALSE)
		return ERROR_USER_IS_NOT_AUCTIONEER;
	BOOL endAuctionSuccessfull = endAuction(user, auctionNumber);

	// aktualisiert die MyBayAuctions.txt
	writeAuctionsToFile();

	return RPC_S_OK;
}

/// <summary>
/// Nachrichten f�r den Client
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="messageAvailable"> Weitere Nachrichten sind f�r den Client verf�gbar </param>
/// <param name="messageType"> Nachrichtentyp </param>
/// <param name="message"> Nachricht serialisiert </param>
/// <returns> Error Status </returns>
error_status_t getMessage(unsigned long sessionId, boolean* messageAvailable, unsigned long* messageType, String_t* message)
{
	// Pr�fen ob User eingeloggt ist
	if (BOOL isLoggedIn = loggedInCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);
	int msgCnt = countMessages(user);
	// eine neue Nachricht f�r den User wurde gefunden
	if (msgCnt > 0)
	{
		vector<wstring> newMessage = searchForMessage(user);				// Neue Nachticht f�r den user suchen
		*messageType = stoul(newMessage.at(1));								// MessageType festlegen
		// Nachricht serialisieren
		wstring serStr = serializeMessage(newMessage);

		// Speicher f�r die �bertragung des String_t allokieren
		int serStrSize = sizeof(char) * (serStr.size() + 1);
		message->str = (unsigned char*)midl_user_allocate(serStrSize);			
		message->size = serStrSize;
		message->len = serStrSize;

		strcpy((char*)message->str, (char*)wstring_to_char(serStr.c_str()));
	}

	// Falls noch mehr Nachrichten f�r den Nutzer bereit liegen, wird der User mit messageAvailable dar�ber informiert.
	// Damit wei� er, dass er nochmal Pullen muss um sich die restlichen Nachrichten abzuholen
	// und nicht wartet bis der Timer bis zum n�chsten Pull abgelaufen ist.
	if (msgCnt > 1)
	{
		*messageAvailable = TRUE;
	}
	else
		*messageAvailable = FALSE;
	

	return RPC_S_OK;
}