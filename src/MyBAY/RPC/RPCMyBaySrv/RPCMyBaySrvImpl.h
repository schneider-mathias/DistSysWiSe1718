/************************************************************/
/*                                                          */
/* Inhalt:    Serverimplementierung	- Header	            */
/*                                                          */
/* Autor:	  Johannes Sauer		                        */
/* Stand:     23. Jan 2018                                  */
/*															*/	
/************************************************************/
#pragma once

#include <Windows.h>
#include <map>
#include <vector>
#include "MyBayDefines.h"
#include "CritSectionWrapper.h"
#include <thread>

using namespace std;

/********************************************************************/
/*						Strukturdefinitionen						*/
/********************************************************************/

struct bidder
{
	wstring userName;
	double bid;
};

struct auction
{
	unsigned long auctionNumber;
	wstring articleName;
	double startBid;
	double highestBid;
	wstring highestBidder;
	std::vector<bidder> BidderList;
	std::vector<wstring> interestedUserList;	// erster in der Liste, ist Auktionator
	int auctionStatus;							// 0 = offen, 1 = kurz vor Abschluss, 2 = Beendet
};

CritSectionWrapper critSecWrapper;
std::map<std::wstring, unsigned long> users;
std::vector<auction> AuctionList;
std::vector<vector<wstring>> Messages;
std::vector<thread> myThreads;

/********************************************************************/
/*						Funktionsdefinitionen						*/
/********************************************************************/
error_status_t login(unsigned char *username, unsigned char *password, unsigned long* sessionId);
error_status_t logout(unsigned long sessionId);
error_status_t offer(unsigned long sessionId, unsigned char *articleName, double startBid, unsigned long* auctionNumber);
error_status_t interested(unsigned long sessionId, unsigned long auctionNumber);
error_status_t getAuctions(unsigned long sessionId, unsigned long flags, unsigned char *articleName, unsigned long* countAuctions, String_t* auctions);
error_status_t bid(unsigned long sessionId, unsigned long auctionNumber, double bidVal);
error_status_t details(unsigned long sessionId, unsigned long auctionNumber, String_t *allBids, unsigned long* countBids);
error_status_t endauction(unsigned long sessionId, unsigned long auctionNumber);
error_status_t getMessage(unsigned long sessionId, boolean* messageAvailable, unsigned long* messageType, String_t* message);
void auctionEndProcess(wstring user, unsigned long auctionNumber);
void addEndAuctionMessage(wstring user, unsigned long auctionNumber, int warningNr);
void endAuction(unsigned long auctionNumber);


//// Auktionsnummernprüfung für auction
//struct find_auctionNumber : std::unary_function<auction, bool> {
//	DWORD auctionNumber;
//	find_auctionNumber(DWORD auctionNumber) :auctionNumber(auctionNumber) { }
//	bool operator()(auction const& m) const {
//		return m.auctionNumber == auctionNumber;
//	}
//};

/********************************************************************/
/*							Hilfsfunktionen							*/
/********************************************************************/

// Prüfe ob User bereits eingeloggt ist
BOOL loginCheck(unsigned long sessionId)
{
	std::map<std::wstring, unsigned long>::iterator it;
	for (it = users.begin(); it != users.end(); ++it)
	{
		if (it->second == sessionId)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// Prüfe ob User bereits eingeloggt ist
BOOL loginCheck(unsigned char username)
{
	if (users.find(to_wstring(username)) == users.end())
	{
		return FALSE;
	}
	else
		return TRUE;
}

// Usernamen zur zugehörigen SessionId ermitteln
wstring getUserName(unsigned long sessionId)
{
	wstring ret;
	std::map<std::wstring, unsigned long>::iterator it;
	for (it = users.begin(); it != users.end(); ++it)
	{
		if (it->second == sessionId)
		{
			ret = it->first;
		}
	}
	return ret;
}

// Um alle Auktionen persistent zu halten werden alle Auktionen in eine Datei geschrieben
void writeAuctionsToFile()
{
	//TODO:: writeAuctionsToFile()
	// 1. String erstellen für 1. Zeile
	// 2. Serialisiern
	// 3. in Datei schreiben
	// 4. String erstellen für 2. Zeile
	// ...

	std::wofstream auctionsFile;
	vector<wstring> listOfAuctions;
	// Auktionen zerlegen und in Liste speichern
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		// Einzelinfos der Auktion, die in erste Zeile gespeichert werden
		wstring auctionInfos;
		auctionInfos += (*it).articleName + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
		auctionInfos += to_wstring((*it).auctionNumber) + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
		auctionInfos += to_wstring((*it).auctionStatus) + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
		auctionInfos += to_wstring((*it).highestBid) + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
		auctionInfos += (*it).highestBidder + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
		auctionInfos += to_wstring((*it).startBid) + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;

		// Alle interessierten User der Auktion, die in zweiter Zeile gespeichert werden
		wstring auctionInterestedUsers;
		for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
		{
			auctionInterestedUsers += (*it2) + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
		}

		// Alle Bieter der Auktion, die in dritter Spalte gespeichert werden
		wstring auctionBidder;
		for (std::vector<bidder>::iterator it2 = (*it).BidderList.begin(); it2 != (*it).BidderList.end(); ++it2)
		{
			auctionBidder += to_wstring((*it2).bid) + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
			auctionBidder += (*it2).userName + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
		}
		listOfAuctions.push_back(auctionInfos);
		listOfAuctions.push_back(auctionInterestedUsers);
		listOfAuctions.push_back(auctionBidder);
	}

	auctionsFile.open(L"MyBayAuctions.csv", std::ios::out);
	if (auctionsFile.is_open())
	{
		// Schreibe alle Auktionen in die Datei
		for (std::vector<wstring>::iterator it = listOfAuctions.begin(); it != listOfAuctions.end(); ++it)
		{
			auctionsFile << (*it) << "\n";				
		}
	}

	auctionsFile.close();
}

// Erzeugt neue Auktion und fügt diese der Liste aller Auktionen hinzu
ULONG addNewAuction(wstring sarticleName, double startBid)
{
	// neue Auktion erstellen 
	auction newAuct;
	newAuct.articleName = sarticleName;
	newAuct.auctionNumber = AuctionList.size();
	newAuct.highestBid = startBid;
	newAuct.highestBidder = L"";
	newAuct.startBid = startBid;
	newAuct.auctionStatus = 0;
	EnterCriticalSection(critSecWrapper.getInstance());
	AuctionList.push_back(newAuct);	// neue Auktion der Liste der Auktionen hinzufügen
	LeaveCriticalSection(critSecWrapper.getInstance());
	return newAuct.auctionNumber;
}

// Prüft, ob die Auktionsnummer vorhanden ist
BOOL checkAuctionNumber(unsigned long auctionNumber)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			LeaveCriticalSection(critSecWrapper.getInstance());
			return TRUE;
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return FALSE;
}

// User in die Liste der User hinzufügen, die sich für diese Auktion interessieren
BOOL addUserToInterestedUserList(wstring user, unsigned long auctionNumber)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
			{
				if (user == (*it2))
				{
					LeaveCriticalSection(critSecWrapper.getInstance());
					return FALSE;
				}
			}
			(*it).interestedUserList.push_back(user);
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return TRUE;
}

// Status der Auktion 
int getAuctionState(unsigned long auctionNumber)
{
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if ((*it).auctionNumber == auctionNumber)
			return (*it).auctionStatus;
	}
}

// Fügt ein Gebot zur Auktion hinzu
BOOL addBidToAuction(unsigned long auctionNumber, double bidVal, wstring user)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			// Gebot muss größer als Startgebot und aktuelles Höchstgebot sein
			if ((*it).highestBid >= bidVal || (*it).startBid >= bidVal)
			{
				LeaveCriticalSection(critSecWrapper.getInstance());
				return FALSE;
			}
			else
			{
				// Neues Gebot anlegen
				bidder newBid;
				newBid.userName = user;
				newBid.bid = bidVal;
				(*it).BidderList.push_back(newBid);	// Gebot der Liste der Gebote hinzufügen
				(*it).highestBid = bidVal;
				(*it).highestBidder = user;
			}
			break;
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return TRUE;
}

// Prüft ob User die Auktion selbst erstellt hat und ob die Auktion mit der angegebenen Auktion überhaupt vorhanden ist
BOOL userCreatedAuctioncheck(wstring user, unsigned long auctionNumber)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		// Prüfen ob Auktion vorhanden
		if (auctionNumber == (*it).auctionNumber)
		{
			// Prüfen ob User diese Auktion auch selbst erstellt hat. Ersteller ist immer der erste in der Liste der interessierten User.
			if ((*it).interestedUserList.at(0) == user)
			{
				LeaveCriticalSection(critSecWrapper.getInstance());
				return TRUE;
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return FALSE;
}

// gibt die Länge des Strings zurück, der für die Übertragung aller Gebote einer bestimmten Auktion benötigt wird
int getSerializedStrSize(unsigned long auctionNumber)
{
	int strSize = 0;
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			for (std::vector<bidder>::iterator it2 = (*it).BidderList.begin(); it2 != (*it).BidderList.end(); ++it2)
			{
				strSize += to_wstring((*it2).bid).size();
				strSize += (*it2).userName.size();
				strSize += 2;			// nach jedem Gebot und Bieter wird ein Leerzeichen eingefügt
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return strSize;
}

// Anzahl wie viel Gebote für die Auktion abgegeben wurden
unsigned long countBidsOfAuction(unsigned long auctionNumber)
{
	unsigned long numBids = 0;
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		// Prüfen ob Auktion vorhanden
		if (auctionNumber == (*it).auctionNumber)
		{
			numBids = (unsigned long)(*it).BidderList.size();
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return numBids;
}

// serialisiert die Gebote einer Auktion in chronologischer Reihenfolge
wstring serializeAuctionDetails(unsigned long auctionNumber)
{
	int newStrLen = 0;;
	BOOL firstCpy = TRUE;
	wstring serStr = L"";
	 
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		// Prüfen ob Auktion vorhanden
		if (auctionNumber == (*it).auctionNumber)
		{
			for (std::vector<bidder>::iterator it2 = (*it).BidderList.begin(); it2 != (*it).BidderList.end(); ++it2)
			{
				// Gebot zum String hinzufügen
				// Gebotsnummer
				wstring tempBidStr1 = to_wstring(distance((*it).BidderList.begin(), it2));
				serStr += tempBidStr1;
				serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;

				// Bietername
				wstring tempBidStr2 = (*it2).userName;
				tempBidStr2 = tempBidStr2 + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;			// Platzhalter um Parsen beim Client zu ermöglichen
				serStr += tempBidStr2;

				// Gebot
				wstring tempBidStr3 = to_wstring((*it2).bid);
				wstring tempBidStrCut = tempBidStr3.substr(0, tempBidStr3.size() - 4);				// da double 6 Nachkommastellen hat, werden die letzten 4 Nachkommastellen abgeschnitten
				tempBidStrCut = tempBidStrCut + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;		// Platzhalter um Parsen beim Client zu ermöglichen												
				serStr += tempBidStrCut;		
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	if (serStr == L"")
		serStr = L"NO_DETAILS";
	return serStr;
}

// Beendet die Auktion
BOOL endAuction(wstring user, unsigned long auctionNumber)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		// Prüfen ob Auktion vorhanden
		if (auctionNumber == (*it).auctionNumber)
		{
			// Prüfen ob User diese Auktion auch selbst erstellt hat. Ersteller ist immer der erste in der Liste der interessierten User.
			if ((*it).interestedUserList.at(0) == user)
			{
				LeaveCriticalSection(critSecWrapper.getInstance());
				// Thread um Nachrichten für die interessierten User zu erzeugen
				myThreads.push_back(std::thread(auctionEndProcess, user, auctionNumber));
				//std::thread EndAuktionThread(auctionEndProcess, user, auctionNumber);	// Threadpointer und dann hier starten
				return TRUE;
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return FALSE;
}



// sucht ob für den User eine Nachricht verfügbar ist
vector<wstring> searchForMessage(wstring user)
{
	vector<wstring> message;
	int i = -1;
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<vector<wstring>>::iterator it = Messages.begin(); it != Messages.end(); ++it)
	{
		i++;
		if ((*it).at(0) == user)
		{
			message = (*it);
			Messages.erase(it);
			LeaveCriticalSection(critSecWrapper.getInstance());
			return message;
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return message;
}

// Anzahl der Messages die für den User in der Messagebox bereit liegen
int countMessages(wstring user)
{
	int cnt = 0;
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<vector<wstring>>::iterator it = Messages.begin(); it != Messages.end(); ++it)
	{
		if ((*it).at(0) == user)
		{
			cnt++;
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return cnt;
}

// serialisiert eine Nachricht
wstring serializeMessage(vector<wstring> newMessage)
{
	wstring serStr = L"";
	newMessage.erase(newMessage.begin());	// erster Wert wird entfernt, da nicht mehr interessant für wen die Nachricht ist
	for (std::vector<wstring>::iterator it = newMessage.begin(); it != newMessage.end(); it++)
	{
		serStr += (*it);
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
	}
	return serStr;
}

// Fügt die Nachricht für alle Interessierten der Messagebox hinzu, dass ein neues Gebot abgegeben wurde
void addNewBidToMessages(wstring articleName, double bidVal, wstring user)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (articleName == (*it).articleName)
		{
			for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
			{
				// neue Nachricht erstellen
				vector<wstring> newMessage;
				int messageType = 0;
				wstring tmpBidVal = to_wstring(bidVal);
				wstring bidValCut = tmpBidVal.substr(0, tmpBidVal.size() - 4);

				newMessage.push_back((*it2));												// (0) Name des interessierten Users
				newMessage.push_back(to_wstring(messageType));								// (1) MessageType
				if ((*it).interestedUserList.at(0) == (*it2))
					newMessage.push_back(user);												// (2) Name des Bieters
				else
					newMessage.push_back(L"");												// (2) Name des Bieters wird nur dem Auktionator angezeigt
				newMessage.push_back(articleName);											// (3) Artikelname
				newMessage.push_back(bidValCut);											// (4) Gebot
				newMessage.push_back(L"0");													// (5) Auktionsstatus

				Messages.push_back(newMessage);												// Nachricht der Messagebox hinzufügen
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
}

// Liefert zu einer Auktionsnummer den entsprechenden Artikelnamen.
// gibt leeren string zurück, wenn Auktionsnummer nicht vorhanden.
wstring getArticleName(unsigned long auctionNumber)
{
	wstring articleName = L"";
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		// Prüfen ob Auktion vorhanden
		if (auctionNumber == (*it).auctionNumber)
		{
			articleName = (*it).articleName;
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return articleName;
}

// Filtert alle Auktionen, die mit dem Artikelnameteil übereinstimmen
vector<auction> filterArtName(wstring user, wstring sarticleName)
{
	vector<auction> filtedArtNameAuctions;
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); it++)
	{
		// Artikelnameteil wurde angegeben 
		if (sarticleName != L"")
		{
			// sucht ob Artikelnameteil in dem aktuellen Artikelnamen
			if ((*it).articleName.find(sarticleName) != std::string::npos)
			{
				filtedArtNameAuctions.push_back(*it);
			}
		}
		// Artikelnameteil wurde angegeben, damit werden alle Auktionen zurückgeliefert
		else
		{
			filtedArtNameAuctions.push_back(*it);
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return filtedArtNameAuctions;
}

// Filtert alle Auktionen, die die Bedingungen der Flags erfüllen
vector<auction> filterAuctionsByFlags(wstring user, unsigned long flags, vector<auction> filteredArtNameAuctions)
{
	vector<auction> filteredAuctionsByFlags;
	// [-A] -> alle Auktionen
	if (flags == 2)
	{
		filteredAuctionsByFlags = filteredArtNameAuctions;
	}
	// [-a] -> offene Auktionen
	else if (flags == 1)
	{
		EnterCriticalSection(critSecWrapper.getInstance());
		for (std::vector<auction>::iterator it = filteredArtNameAuctions.begin(); it != filteredArtNameAuctions.end(); it++)
		{
			if ((*it).auctionStatus == 0)
			{
				filteredAuctionsByFlags.push_back(*it);
			}
		}
		LeaveCriticalSection(critSecWrapper.getInstance());
	}
	// weder [-a] noch [-A] wurden angegeben -> alle Auktionen für die sich er User interessiert
	else if (flags == 0)
	{
		EnterCriticalSection(critSecWrapper.getInstance());
		for (std::vector<auction>::iterator it = filteredArtNameAuctions.begin(); it != filteredArtNameAuctions.end(); it++)
		{
			for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
			{
				if ((*it2) == user)
				{
					filteredAuctionsByFlags.push_back(*it);
				}
			}
		}
		LeaveCriticalSection(critSecWrapper.getInstance());
	}
	return filteredAuctionsByFlags;
}

// Suche na allen Auctionen, die unter der Angabe der Flags ([-a][-A]) und eines Artikelnamensteils 
vector<auction> getInterestingAuctions(wstring user, unsigned long flags, wstring sarticleName)
{
	vector<auction> filteredArtNameAuctions = filterArtName(user, sarticleName);								// Filterstage 1
	vector<auction> filteredAuctionsByFlags = filterAuctionsByFlags(user, flags, filteredArtNameAuctions);		// Filterstage 2
	return filteredAuctionsByFlags;
}

// Serialisierung der Auktionen für getAuctions 
wstring serializeAuctions(vector<auction> interestingAuctions)
{
	wstring serStr = L"";
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = interestingAuctions.begin(); it != interestingAuctions.end(); it++)
	{
		wstring highestBid = to_wstring((*it).highestBid);
		wstring highestBidCut = highestBid.substr(0, highestBid.size() - 4);
		// Gebot zum String hinzufügen
		serStr += to_wstring((*it).auctionNumber);					// (0) Auktionsnummer
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
		serStr += (*it).articleName;								// (1) Artikelname
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
		serStr += to_wstring((*it).BidderList.size());				// (2) Zahl der aktuellen Gebote
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
		serStr += highestBidCut;									// (3) Höchstgebot
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return serStr;
}


/********************************************************************/
/*							EndAuktionThread						*/
/********************************************************************/

// Fügt für jeden an der Auktion interessierten Nutzer eine neue Nachricht in die Messagebox hinzu, dass die Auktion bald beendet wird.
// Enthält einen Timer, um die Nutzer noch (TODO:) x mal zu benachrichtigen, wie lange sie noch Zeit haben bevor die Auktion
// entgültig beendet wird.
void auctionEndProcess(wstring user, unsigned long auctionNumber)
{
	// Message hinzufügen, die sagt, dass in 15 Sekunden die Auktion endet
	addEndAuctionMessage(user, auctionNumber, 0);
	Sleep(5000);

	// Message hinzufügen, die sagt, dass in 10 Sekunden die Auktion endet
	addEndAuctionMessage(user, auctionNumber, 1);
	Sleep(5000);

	// Message hinzufügen, die sagt, dass in 5 Sekunden die Auktion endet
	addEndAuctionMessage(user, auctionNumber, 2);
	Sleep(5000);

	// Message hinzufügen, die sagt, dass die Auktion beendet
	addEndAuctionMessage(user, auctionNumber, 3);

	// Auktion beenden
	endAuction(auctionNumber);
}

// Erzeugt neue Nachricht, die interessierte Nutzer daraufhinweist, dass Auktion bald endet und fügt diese der Messagebox hinzu.
void addEndAuctionMessage(wstring user, unsigned long auctionNumber, int warningNr)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
			{
				wstring highestBid = to_wstring((*it).highestBid);
				wstring highestBidCut = highestBid.substr(0, highestBid.size() - 4);
				vector<wstring> newMessage;
				unsigned long messageType = 1;						// MessageType = 1 => kurz vor Auktionsende-Message
				newMessage.push_back((*it2));						// (0) Name des interessierten Users, der die Nachricht erhalten soll	
				newMessage.push_back(to_wstring(messageType));		// (1) MessageType
				newMessage.push_back(to_wstring(warningNr));		// (2) Warnungkategorie
				newMessage.push_back((*it).articleName);			// (3) Artikelname
				newMessage.push_back(to_wstring(auctionNumber));	// (4) Auktionsnummer
				newMessage.push_back(highestBidCut);				// (5) Höchstgebot
				Messages.push_back(newMessage);						// neue Nachricht der Messagebox hinzufügen
			}
			(*it).auctionStatus = 1;								// Auktionsstatus wird auf "kurz vor Schluss" geändert
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
}

// beendet die Auktion TODO: Auktionsende an alle Interessierten
void endAuction(unsigned long auctionNumber)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			(*it).auctionStatus = 2;								// Auktionsstatus wird auf "Beendet" geändert
			for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
			{
				wstring highestBid = to_wstring((*it).highestBid);
				wstring highestBidCut = highestBid.substr(0, highestBid.size() - 4);
				// Message, dass Auktion beendet wurde	
				vector<wstring> newMessage;
				unsigned long messageType = 2;							// MessageType = 2 => Auktion beendet
				newMessage.push_back((*it).interestedUserList.at(0));	// (0) Auktionator, der die Nachricht erhalten soll
				newMessage.push_back(to_wstring(messageType));			// (1) MessageType
				newMessage.push_back((*it).highestBidder);				// (2) Höchstbietender
				newMessage.push_back((*it).articleName);				// (3) Artikelname
				newMessage.push_back(highestBidCut);					// (4) Höchstgebot
				newMessage.push_back(to_wstring((*it).auctionStatus));	// (5) Auktionsstatus
				Messages.push_back(newMessage);							// neue Nachricht der Messagebox hinzufügen
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
}
