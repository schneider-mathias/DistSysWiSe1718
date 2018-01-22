/************************************************************/
/*                                                          */
/* Inhalt:    COMMyBay - Schnittstellenimplementierung      */
/*                                                          */
/* Autor:	  Johannes Sauer		                        */
/* Stand:     23. Jan 2018                                  */
/*															*/
/************************************************************/

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <atlsafe.h>
#include "COMMyBayLogic.h"
#include "BstrStringConverter.h"
#include "CharStringConverter.h"
#include "COMMyBay.h"


/// <summary>
/// Login
/// </summary>
/// <param name="username"> Username </param>
/// <param name="password"> Passwort </param>
/// <param name="sessionId"> SessionId des Users </param>
/// <returns> Error Status </returns>
STDMETHODIMP CCOMMyBay::login(BSTR username, BSTR password, ULONG* sessionId)
{
	std::wifstream csvread;
	srand((unsigned)time(NULL)); // Zufallsgenerator initialisieren.

	// Wenn sich der erste Nutzer einloggt, werden die persistent gespeicherten Auktionen aus der Datei ausgelesen
	if (users.size() == 0)
	{
		AuctionList.clear();
		readAuctionsFromFile();
	}

	// Pr�fen ob User bereits eingeloggt ist. Return sessionId falls User bereits eingeloggt ist, sonst 0
	unsigned long existSessionId = loginCheck((unsigned char *)bstr_to_str(username).c_str());
	if (existSessionId != 0)
	{
		*sessionId = existSessionId;
		return S_OK;
	}

	wstring userDataPath = getUserDataPath();
	csvread.open("C:/_MyBayData/user.csv", ios::in);
	if (csvread) {
		wstring fline, fname, fpassword;
		wstring suser = bstr_to_wstr(username);
		wstring spassword = bstr_to_wstr(password);

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
				return S_OK;
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
STDMETHODIMP CCOMMyBay::logout(ULONG sessionId)
{
	//�berpr�fen ob user angemeldet und dann aus Liste entfernen
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

/// <summary>
/// Auktion starten
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="articleName"> Artikelname </param>
/// <param name="startBid"> Startgebot </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Error Status </returns>
STDMETHODIMP CCOMMyBay::offer(ULONG sessionId, BSTR articleName, DOUBLE startBid, ULONG* auctionNumber)
{
	// Pr�fen ob User eingeloggt ist
	if (BOOL isLoggedIn = loggedInCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);

	wstring sarticleName = bstr_to_wstr(articleName);
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

	return S_OK;
}

/// <summary>
/// Auktion starten
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Error Status </returns>
STDMETHODIMP CCOMMyBay::interested(ULONG sessionId, ULONG auctionNumber)
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
	if (userAdded == FALSE)								// User bereits interessiert
	{
		return ERROR_USER_ALREADY_INTERESTED;
	}

	// aktualisiert die MyBayAuctions.txt
	writeAuctionsToFile();

	return S_OK;
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
STDMETHODIMP CCOMMyBay::getAuctions(ULONG sessionId, ULONG flags, BSTR articleName, ULONG* countAuctions, SAFEARRAY_VAR* auctions)
{
	// Pr�fen ob User eingeloggt ist
	if (BOOL isLoggedIn = loggedInCheck(sessionId) == FALSE)
	{
		return ERROR_USER_NOT_LOGGED_IN;
	}
	wstring user = getUserName(sessionId);
	wstring sarticleName = bstr_to_wstr(articleName);

	// Gebote serialisieren
	vector<auction> interestingAuctions = getInterestingAuctions(user, flags, sarticleName);
	if (interestingAuctions.size() == 0)
	{
		return ERROR_NO_AUCTIONS_AVAILABLE;
	}
	vector<wstring> retStr = filterInterestingInfos(interestingAuctions);			// ben�tigte Informationen aus Auktionen suchen 

	ATL::CComSafeArray<VARIANT> auctionsSafeArray(retStr.size());						// SafeArray mit entsprechender Gr��er initialiseren

	int safeArrIt = 0;
	// Alle Informationen im SafeArray speichern
	for (int i = 0; i < retStr.size(); i++)
	{
		auctionsSafeArray[safeArrIt++] = wstr_to_bstr(retStr.at(i));
	}
	auctionsSafeArray.CopyTo(auctions);
	*countAuctions = interestingAuctions.size();

	return S_OK;
}

/// <summary>
/// Gebot abgeben
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <param name="bidValue"> Gebot </param>
/// <returns> Error Status </returns>
STDMETHODIMP CCOMMyBay::bid(ULONG sessionId, ULONG auctionNumber, DOUBLE bidValue)
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
	if (bidValue < 0)
	{
		return ERROR_BID_NEGATIVE;
	}
	int auctionState = getAuctionState(auctionNumber);
	if (auctionState == 2)		// Auktion geschlossen
	{
		return ERROR_AUCTION_CLOSED;
	}
	BOOL bidSuccessfull = addBidToAuction(auctionNumber, bidValue, user);
	if (bidSuccessfull == FALSE)	// Gebot muss h�her als Startgebot und H�chstgebot sein
	{
		return ERROR_BID_TOO_LOW;
	}
	addNewBidToMessages(auctionNumber, bidValue, user);

	// aktualisiert die MyBayAuctions.txt
	writeAuctionsToFile();

	return S_OK;
}

/// <summary>
/// Alle Gebote einer Auktion
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <param name="allBids"> Alle Gebote der Auktion  </param>
/// <param name="countBids"> Anzahl der Gebote </param>
/// <returns> Error Status </returns>
STDMETHODIMP CCOMMyBay::details(ULONG sessionId, ULONG auctionNumber, SAFEARRAY_VAR * allBids, ULONG * countBids)
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
	vector<wstring> bidsStr = getAllBids(auctionNumber);
	// Speicher f�r die �bertragung des String_t allokieren
	ATL::CComSafeArray<VARIANT> auctionsSafeArray(bidsStr.size());						// SafeArray mit entsprechender Gr��er initialiseren

	int safeArrIt = 0;
	// Alle Informationen im SafeArray speichern
	for (int i = 0; i < bidsStr.size(); i++)
	{
		auctionsSafeArray[safeArrIt++] = wstr_to_bstr(bidsStr.at(i));
	}
	auctionsSafeArray.CopyTo(allBids);

	// Anzahl der Auktionen bestimmen
	*countBids = countBidsOfAuction(auctionNumber);

	return S_OK;
}

/// <summary>
/// Auktion beenden
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Error Status </returns>
STDMETHODIMP CCOMMyBay::endauction(ULONG sessionId, ULONG auctionNumber)
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

	return S_OK;
}

/// <summary>
/// Nachrichten f�r den Client
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <param name="messageAvailable"> Weitere Nachrichten sind f�r den Client verf�gbar </param>
/// <param name="messageType"> Nachrichtentyp </param>
/// <param name="message"> Nachricht </param>
/// <returns> Error Status </returns>
STDMETHODIMP CCOMMyBay::getMessage(ULONG sessionId, BOOL * messageAvailable, ULONG * messageType, SAFEARRAY_VAR * message)
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
		ATL::CComSafeArray<VARIANT> auctionsSafeArray(newMessage.size()-2);						// SafeArray mit entsprechender Gr��er initialiseren

		int safeArrIt = 0;
		// Alle Informationen im SafeArray speichern
		for (int i = 2; i < newMessage.size(); i++)
		{
			auctionsSafeArray[safeArrIt++] = wstr_to_bstr(newMessage.at(i));
		}
		auctionsSafeArray.CopyTo(message);
	}

	// Falls noch mehr Nachrichten f�r den Nutzer bereit liegen, wird der User mit messageAvailable dar�ber informiert.
	// Damit wei� er, dass er nochmal Pullen muss, um sich die restlichen Nachrichten abzuholen
	// und nicht wartet bis der Timer f�r den n�chsten Pull abgelaufen ist.
	if (msgCnt > 1)
	{
		*messageAvailable = TRUE;
	}
	else
		*messageAvailable = FALSE;

	return S_OK;
}