#include "stdafx.h"
#include "COMMyBay.h"
#include "BstrStringConverter.h"
#include "CharStringConverter.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <locale>
#include <codecvt>
#include <string>

using namespace std;

ULONG CCOMMyBay::num_auct;

STDMETHODIMP CCOMMyBay::login(BSTR username, BSTR password, ULONG * sessionId, BSTR * errMsg)
{
	wifstream csvread;
	srand((unsigned)time(NULL)); // Zufallsgenerator initialisieren.

	// Wenn sich der erste Nutzer einloggt, werden die persistent gespeicherten Auktionen aus der Datei ausgelesen
	if (users.size() == 0)
	{
		AuctionList.clear();
		readAuctionsFromFile();
	}

	/* Abfrage ob User nicht schon eingeloggt ist.*/
	wstring tmp = bstr_to_wstr(username);
	BOOL isLoggedIn = loginCheck(tmp.c_str);
	if (isLoggedIn == TRUE)
	{
		//cout << "Sie sind bereits angemeldet" << endl;
		return ERROR_ALREADY_LOGGED_IN;
	}

	csvread.open("user.csv", ios::in);
	if (csvread) {
		wstring fline, fname, fpassword;
		wstring suser = char_to_wstring((char*)username);
		wstring spassword = char_to_wstring((char*)password);

		// in jeder Zeile prüfen ob der Username und Passwort mit den Eingegebenen übereinstimmen
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
				return S_OK;
			}
		}
		csvread.close();
	}
	else
		return ERROR_FILE_COULD_NOT_BE_OPENED;

	return ERROR_USERNAME_OR_PASSWORD_WRONG;
}

STDMETHODIMP CCOMMyBay::logout(ULONG sessionId, BSTR *errMsg)
{
	//überprüfen ob user angemeldet und dann aus Liste entfernen
	std::map<std::wstring, unsigned long>::iterator it;
	for (it = users.begin(); it != users.end(); ++it)
	{
		if (it->second == sessionId)
		{
			users.erase(it);
			return S_OK;
		}
	}
	return ERROR_USER_NOT_LOGGED_IN; //user nicht angemeldet
}

STDMETHODIMP CCOMMyBay::offer(ULONG sessionId, BSTR articleName, DOUBLE startBid, ULONG * auctionNumber, BSTR * errMsg)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);

	wstring sarticleName = bstr_to_wstr(articleName);
	if (sarticleName == L"")		// Artikelname darf nicht leer sein
	{
		return ERROR_ARTICLENAME_IS_EMPTY;
	}

	// neue Auktion erzeugen
	unsigned long newAuctNumb = addNewAuction(sarticleName, startBid);
	// User der Liste der interessierten User für diese Auktion hinzufügen
	BOOL userAdded = addUserToInterestedUserList(user, newAuctNumb);

	// aktualisiert die MyBayAuctions.txt
	writeAuctionsToFile();

	*auctionNumber = newAuctNumb;		// Auktionsnummer zurückliefern

	return S_OK;
}

STDMETHODIMP CCOMMyBay::interested(ULONG sessionId, ULONG auctionNumber, BSTR* errMsg)
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

	// aktualisiert die MyBayAuctions.txt
	writeAuctionsToFile();

	return S_OK;
}

STDMETHODIMP CCOMMyBay::getAuctions(ULONG sessionId, ULONG flags, BSTR articleName, ULONG* countAuctions, SAVEARRAY_VAR* auctions, BSTR* errMsg)
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
		return ERROR_NO_AUCTIONS_AVAILABLE;
	}
	wstring serStr = serializeAuctions(interestingAuctions);

	// Speicher für die Übertragung des String_t allokieren
	// TODO: in SAVEARRAY
	int serStrSize = sizeof(char) * (serStr.size() + 1);
	auctions->str = (unsigned char*)midl_user_allocate(serStrSize);
	auctions->size = serStrSize;
	auctions->len = serStrSize;

	strcpy((char*)auctions->str, (char*)wstring_to_char(serStr.c_str()));


	*countAuctions = interestingAuctions.size();

	return S_OK;
}

STDMETHODIMP CCOMMyBay::bid(ULONG sessionId, ULONG auctionNumber, DOUBLE bidVal, BSTR* errMsg)
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
	if (bidSuccessfull == FALSE)	// Gebot muss höher als Startgebot und Höchstgebot sein
	{
		return ERROR_BID_TOO_LOW;
	}
	addNewBidToMessages(auctionNumber, bidVal, user);

	// aktualisiert die MyBayAuctions.txt
	writeAuctionsToFile();

	return S_OK;
}

STDMETHODIMP CCOMMyBay::details(ULONG sessionId, ULONG auctionNumber, SAVEARRAY_VAR* allBids, ULONG* countBids, BSTR* errMsg)
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

	// Speicher für die Übertragung des String_t allokieren
	// TODO: SAVEARRAY
	int serStrSize = sizeof(char) * (serStr.size() + 1);
	allBids->str = (unsigned char*)midl_user_allocate(serStrSize);
	allBids->size = serStrSize;
	allBids->len = serStrSize;

	strcpy((char*)allBids->str, (char*)wstring_to_char(serStr.c_str()));

	// Anzahl der Auktionen bestimmen
	*countBids = countBidsOfAuction(auctionNumber);

	return S_OK;
}

STDMETHODIMP CCOMMyBay::endauction(ULONG sessionId, ULONG auctionNumber, BSTR* errMsg)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
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

	return S_OK;
}

STDMETHODIMP CCOMMyBay::getMessage(ULONG sessionId, BOOL*messageAvailable, ULONG* messageType, SAVEARRAY_VAR* message, BSTR* errMsg)
{
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

		// Speicher für die Übertragung des String_t allokieren
		// TODO: SAVEARRAY
		int serStrSize = sizeof(char) * (serStr.size() + 1);
		message->str = (unsigned char*)midl_user_allocate(serStrSize);
		message->size = serStrSize;
		message->len = serStrSize;

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


	return S_OK;
}
