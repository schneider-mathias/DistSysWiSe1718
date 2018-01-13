#pragma once

#include "Resource.h"
#include <Windows.h>
#include <map>
#include <vector>
#include <math.h>
#include "MyBayDefines.h"
#include "CritSectionWrapper.h"
#include "CharStringConverter.h"
#include "BstrStringConverter.h"
#include <fstream>
#include <thread>
#include <string>

using namespace std;

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
/*							Hilfsfunktionen							*/
/********************************************************************/

BOOL loginCheck(unsigned long sessionId);
BOOL loginCheck(unsigned char username);
wstring getUserName(unsigned long sessionId);
wstring getUserDataPath();
void readAuctionsFromFile();
void writeAuctionsToFile();
ULONG addNewAuction(wstring sarticleName, double startBid);
BOOL checkAuctionNumber(unsigned long auctionNumber);
BOOL addUserToInterestedUserList(wstring user, unsigned long auctionNumber);
int getAuctionState(unsigned long auctionNumber);
BOOL addBidToAuction(unsigned long auctionNumber, double bidVal, wstring user);
BOOL auctionStatusCheck(unsigned long auctionNumber);
BOOL userCreatedAuctioncheck(wstring user, unsigned long auctionNumber);
int getSerializedStrSize(unsigned long auctionNumber);
unsigned long countBidsOfAuction(unsigned long auctionNumber);
vector<wstring> getAllBids(ULONG auctionNumber);
wstring serializeAuctionDetails(unsigned long auctionNumber);
BOOL endAuction(wstring user, unsigned long auctionNumber);
vector<wstring> searchForMessage(wstring user);
int countMessages(wstring user);
wstring serializeMessage(vector<wstring> newMessage);
void addNewBidToMessages(unsigned long auctionNumber, double bidVal, wstring user);
wstring getArticleName(unsigned long auctionNumber);
vector<auction> filterArtName(wstring user, wstring sarticleName);
vector<auction> filterAuctionsByFlags(wstring user, unsigned long flags, vector<auction> filteredArtNameAuctions);
vector<auction> getInterestingAuctions(wstring user, unsigned long flags, wstring sarticleName);
vector<wstring> filterInterestingInfos(vector<auction> interestingAuctions);
wstring serializeAuctions(vector<auction> interestingAuctions);
int numberOfElements(wstring serStr);
void auctionEndProcess(wstring user, unsigned long auctionNumber);
void addEndAuctionMessage(wstring user, unsigned long auctionNumber, int warningNr);
void endAuction(unsigned long auctionNumber);


// Pr�fe ob User bereits eingeloggt ist
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

// Pr�fe ob User bereits eingeloggt ist
BOOL loginCheck(unsigned char username)
{
	if (users.find(to_wstring(username)) == users.end())
	{
		return FALSE;
	}
	else
		return TRUE;
}

// Usernamen zur zugeh�rigen SessionId ermitteln
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

// Liefert den Pfad an dem die Userdaten liegen zur�ck
wstring getUserDataPath()
{
	wstring userDataPath;
	wchar_t* buf;
	size_t size = 0;
	_wdupenv_s(&buf, &size, L"SystemDrive");
	userDataPath = wstring(buf) + L"/_MyBayData";
	return userDataPath;
}

// Alle Auktionen aus der Datei auslesen und in AuctionList speichern
void readAuctionsFromFile()
{
	std::wifstream auctionsFile;
	auctionsFile.open(L"..\\..\\_data\\MyBayAuctions.csv", std::ios::in);
	// �ffnen der Datei
	if (auctionsFile.is_open())
	{
		auction rauction;
		wstring fline;
		int line = 0;
		// einzelne Zeilen einlesen
		while (getline(auctionsFile, fline))
		{
			wstring delimiter = L" ";
			// Einzelne Auktionsdetails 
			if (line == 0)
			{
				size_t delimPos = fline.find(delimiter);
				rauction.articleName = fline.substr(0, delimPos);									// Artikelname
				fline.erase(0, delimPos + delimiter.length());
				// TODO: wstring_to_char ausbauen -> wstring.c_str() verwenden
				delimPos = fline.find(delimiter);
				char * tmp = wstring_to_char(fline.substr(0, delimPos));
				rauction.auctionNumber = strtoul(tmp, NULL, 0);										// Auktionsnummer
				fline.erase(0, delimPos + delimiter.length());

				delimPos = fline.find(delimiter);
				tmp = wstring_to_char(fline.substr(0, delimPos));
				rauction.auctionStatus = strtoul(tmp, NULL, 0);										// Auktionsstatus
				fline.erase(0, delimPos + delimiter.length());

				delimPos = fline.find(delimiter);
				rauction.highestBid = stod(wstring_to_char(fline.substr(0, delimPos)));				// H�chstgebot
				fline.erase(0, delimPos + delimiter.length());

				delimPos = fline.find(delimiter);
				rauction.highestBidder = fline.substr(0, delimPos);									// H�chstbietender
				fline.erase(0, delimPos + delimiter.length());

				delimPos = fline.find(delimiter);
				rauction.startBid = stod(wstring_to_char(fline.substr(0, delimPos)));				// Startgebot
				fline.erase(0, delimPos + delimiter.length());

				line++;
				delete[] tmp;
			}
			// alle an der Auktion Interessierten
			else if (line == 1)
			{
				size_t delimPos;
				wstring rintUser;
				vector<wstring> rintUserList;
				// bis keine Wert mehr in der Zeile
				while (fline.find(delimiter) != std::string::npos)
				{
					delimPos = fline.find(delimiter);
					rintUser = fline.substr(0, delimPos);											// interessierter User
					fline.erase(0, delimPos + delimiter.length());
					rintUserList.push_back(rintUser);
				}
				line++;
				rauction.interestedUserList = rintUserList;
			}
			// alle Bieter der Auktion
			else if (line == 2)
			{
				size_t delimPos;
				bidder rbidder;
				vector<bidder> rbidderList;
				int bidderInfo = 0;
				// Suche nach Bieterinformationen solange ein Trennungszeichen vorhanden
				while (fline.find(delimiter) != std::string::npos)
				{
					delimPos = fline.find(delimiter);
					// Gebot des Bieters 
					if (bidderInfo == 0)
					{
						delimPos = fline.find(delimiter);
						rbidder.bid = stod(wstring_to_char(fline.substr(0, delimPos)));				// H�chstgebot
						fline.erase(0, delimPos + delimiter.length());
						bidderInfo++;
					}
					// Bietername
					else if (bidderInfo == 1)
					{
						delimPos = fline.find(delimiter);
						rbidder.userName = fline.substr(0, delimPos);								// Bietername
						fline.erase(0, delimPos + delimiter.length());
						bidderInfo = 0;
						rbidderList.push_back(rbidder);
					}
				}
				line = 0;
				rauction.BidderList = rbidderList;
				AuctionList.push_back(rauction);													// nach 3. Zeile ist die Auktion fertig eingelesen und kann der Auktionsliste hinzugef�gt werden
			}
		}
	}
}

// Um alle Auktionen persistent zu halten werden alle Auktionen in eine Datei geschrieben
void writeAuctionsToFile()
{
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

	auctionsFile.open(L"..\\..\\_data\\MyBayAuctions.csv", std::ios::out);
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

// Erzeugt neue Auktion und f�gt diese der Liste aller Auktionen hinzu
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
	AuctionList.push_back(newAuct);	// neue Auktion der Liste der Auktionen hinzuf�gen
	LeaveCriticalSection(critSecWrapper.getInstance());
	return newAuct.auctionNumber;
}

// Pr�ft, ob die Auktionsnummer vorhanden ist
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

// User in die Liste der User hinzuf�gen, die sich f�r diese Auktion interessieren
BOOL addUserToInterestedUserList(wstring user, unsigned long auctionNumber)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
			{
				if (user == (*it2))						// User interessiert sich bereits f�r die Auktion
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
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if ((*it).auctionNumber == auctionNumber)
		{
			LeaveCriticalSection(critSecWrapper.getInstance());
			return (*it).auctionStatus;
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return 0;
}

// F�gt ein Gebot zur Auktion hinzu
BOOL addBidToAuction(unsigned long auctionNumber, double bidVal, wstring user)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			// Gebot muss gr��er als Startgebot und aktuelles H�chstgebot sein
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
				(*it).BidderList.push_back(newBid);	// Gebot der Liste der Gebote hinzuf�gen
				(*it).highestBid = bidVal;
				(*it).highestBidder = user;
				BOOL addToIntUsList = addUserToInterestedUserList(user, auctionNumber);
			}
			break;
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return TRUE;
}

// Pr�ft ob Auktion bereits geschlossen oder kurz vor Schluss
// return TRUE, wenn Auktion noch offen
BOOL auctionStatusCheck(unsigned long auctionNumber)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if ((*it).auctionStatus == 0)
		{
			LeaveCriticalSection(critSecWrapper.getInstance());
			return TRUE;
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return FALSE;
}

// Pr�ft ob User die Auktion selbst erstellt hat und ob die Auktion mit der angegebenen Auktion �berhaupt vorhanden ist
BOOL userCreatedAuctioncheck(wstring user, unsigned long auctionNumber)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		// Pr�fen ob Auktion vorhanden
		if (auctionNumber == (*it).auctionNumber)
		{
			// Pr�fen ob User diese Auktion auch selbst erstellt hat. Ersteller ist immer der erste in der Liste der interessierten User.
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

// gibt die L�nge des Strings zur�ck, der f�r die �bertragung aller Gebote einer bestimmten Auktion ben�tigt wird
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
				strSize += 2;			// nach jedem Gebot und Bieter wird ein Leerzeichen eingef�gt
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return strSize;
}

// Anzahl wie viel Gebote f�r die Auktion abgegeben wurden
unsigned long countBidsOfAuction(unsigned long auctionNumber)
{
	unsigned long numBids = 0;
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		// Pr�fen ob Auktion vorhanden
		if (auctionNumber == (*it).auctionNumber)
		{
			numBids = (unsigned long)(*it).BidderList.size();
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return numBids;
}

// Alle Gebote aller Auktionen
vector<wstring> getAllBids(ULONG auctionNumber)
{
	vector<wstring> bidsStr;
	int bidNmbr = 0;
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if ((*it).auctionNumber == auctionNumber)
		{
			for (std::vector<bidder>::iterator it2 = (*it).BidderList.begin(); it2 != (*it).BidderList.end(); ++it2)
			{
				bidsStr.push_back(to_wstring(bidNmbr++));				// Gebotsnummer	
				bidsStr.push_back((*it2).userName);						// Username
				bidsStr.push_back(to_wstring((*it2).bid));				// Gebot
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return bidsStr;
}


// TODO: wird nicht mehr gebraucht
// serialisiert die Gebote einer Auktion in chronologischer Reihenfolge
wstring serializeAuctionDetails(unsigned long auctionNumber)
{
	int newStrLen = 0;;
	BOOL firstCpy = TRUE;
	wstring serStr = L"";

	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		// Pr�fen ob Auktion vorhanden
		if (auctionNumber == (*it).auctionNumber)
		{
			for (std::vector<bidder>::iterator it2 = (*it).BidderList.begin(); it2 != (*it).BidderList.end(); ++it2)
			{
				// Gebot zum String hinzuf�gen
				// Gebotsnummer
				wstring tempBidStr1 = to_wstring(distance((*it).BidderList.begin(), it2));
				serStr += tempBidStr1;
				serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;

				// Bietername
				wstring tempBidStr2 = (*it2).userName;
				tempBidStr2 = tempBidStr2 + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;			// Platzhalter um Parsen beim Client zu erm�glichen
				serStr += tempBidStr2;

				// Gebot
				wstring tempBidStr3 = to_wstring((*it2).bid);
				//wstring tempBidStrCut = to_wstring((*it2).bid);				
				tempBidStr3 = tempBidStr3 + PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;		// Platzhalter um Parsen beim Client zu erm�glichen												
				serStr += tempBidStr3;
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
		// Pr�fen ob Auktion vorhanden
		if (auctionNumber == (*it).auctionNumber)
		{
			// Pr�fen ob User diese Auktion auch selbst erstellt hat. Ersteller ist immer der erste in der Liste der interessierten User.
			if ((*it).interestedUserList.at(0) == user)
			{
				LeaveCriticalSection(critSecWrapper.getInstance());
				// Thread um Nachrichten f�r die interessierten User zu erzeugen
				myThreads.push_back(thread(auctionEndProcess, user, auctionNumber));
				//std::thread EndAuktionThread(auctionEndProcess, user, auctionNumber);	// Threadpointer und dann hier starten
				return TRUE;
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return FALSE;
}

// sucht ob f�r den User eine Nachricht verf�gbar ist
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

// Anzahl der Messages die f�r den User in der Messagebox bereit liegen
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
	newMessage.erase(newMessage.begin());	// erster Wert wird entfernt, da nicht mehr interessant f�r wen die Nachricht ist
	for (std::vector<wstring>::iterator it = newMessage.begin(); it != newMessage.end(); it++)
	{
		serStr += (*it);
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
	}
	return serStr;
}

// F�gt die Nachricht f�r alle Interessierten der Messagebox hinzu, dass ein neues Gebot abgegeben wurde
void addNewBidToMessages(unsigned long auctionNumber, double bidVal, wstring user)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
			{
				// neue Nachricht erstellen
				vector<wstring> newMessage;
				int messageType = 0;
				//wstring tmpBidVal = to_wstring(bidVal);
				//wstring bidValCut = to_wstring(bidVal);

				newMessage.push_back((*it2));												// (0) Name des interessierten Users
				newMessage.push_back(to_wstring(messageType));								// (1) MessageType
				if ((*it).interestedUserList.at(0) == (*it2))
					newMessage.push_back(user);												// (2) Name des Bieters
				else
					newMessage.push_back(L"");												// (2) Name des Bieters wird nur dem Auktionator angezeigt
				newMessage.push_back((*it).articleName);									// (3) Artikelname
				newMessage.push_back(to_wstring(bidVal));											// (4) Gebot
				newMessage.push_back(L"0");													// (5) Auktionsstatus

				Messages.push_back(newMessage);												// Nachricht der Messagebox hinzuf�gen
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
}

// Liefert zu einer Auktionsnummer den entsprechenden Artikelnamen.
// gibt leeren string zur�ck, wenn Auktionsnummer nicht vorhanden.
wstring getArticleName(unsigned long auctionNumber)
{
	wstring articleName = L"";
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		// Pr�fen ob Auktion vorhanden
		if (auctionNumber == (*it).auctionNumber)
		{
			articleName = (*it).articleName;
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return articleName;
}

// Filtert alle Auktionen, die mit dem Artikelnameteil �bereinstimmen
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
		// Artikelnameteil wurde angegeben, damit werden alle Auktionen zur�ckgeliefert
		else
		{
			filtedArtNameAuctions.push_back(*it);
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return filtedArtNameAuctions;
}

// Filtert alle Auktionen, die die Bedingungen der Flags erf�llen
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
	// weder [-a] noch [-A] wurden angegeben -> alle Auktionen f�r die sich er User interessiert
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

// Filtert die Informationen aus den Auktionen, die f�r getAuctions interessant sind
vector<wstring> filterInterestingInfos(vector<auction> interestingAuctions)
{
	vector<wstring> retStr;
	for (std::vector<auction>::iterator it = interestingAuctions.begin(); it != interestingAuctions.end(); it++)
	{

		retStr.push_back(to_wstring((*it).auctionNumber));			// Auktionsnummer
		retStr.push_back((*it).articleName);						// Artikelname
		retStr.push_back(to_wstring((*it).highestBid));				// H�chstgebot
		retStr.push_back(to_wstring((*it).auctionStatus));			// Auktionsstatus
		retStr.push_back(to_wstring((*it).BidderList.size()));		// Anzahl der Gebote
	}
	return retStr;
}

// TODO: kann raus, wird nicht verwendet
// Serialisierung der Auktionen f�r getAuctions 
wstring serializeAuctions(vector<auction> interestingAuctions)
{
	wstring serStr = L"";
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = interestingAuctions.begin(); it != interestingAuctions.end(); it++)
	{
		wstring highestBid = to_wstring((*it).highestBid);
		wstring highestBidCut = highestBid.substr(0, highestBid.size() - 4);
		// Gebot zum String hinzuf�gen
		serStr += to_wstring((*it).auctionNumber);					// (0) Auktionsnummer
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
		serStr += (*it).articleName;								// (1) Artikelname
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
		serStr += to_wstring((*it).BidderList.size());				// (2) Zahl der aktuellen Gebote
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
		serStr += highestBidCut;									// (3) H�chstgebot
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return serStr;
}

// Anzahl der Elemente in einem serialisierten String
int numberOfElements(wstring serStr)
{
	size_t delimPos;
	int numbOfElem = 0;
	wstring tmpSerStr = serStr;
	wstring delimiter = L" ";
	while (tmpSerStr.find(delimiter) != std::string::npos)
	{
		delimPos = tmpSerStr.find(delimiter);
		tmpSerStr.erase(0, delimPos + delimiter.length());
		numbOfElem++;
	}
	return numbOfElem;
}


/********************************************************************/
/*							EndAuktionThread						*/
/********************************************************************/

// F�gt f�r jeden an der Auktion interessierten Nutzer eine neue Nachricht in die Messagebox hinzu, dass die Auktion bald beendet wird.
// Enth�lt einen Timer, um die Nutzer noch (TODO:) x mal zu benachrichtigen, wie lange sie noch Zeit haben bevor die Auktion
// entg�ltig beendet wird.
void auctionEndProcess(wstring user, unsigned long auctionNumber)
{
	// Message hinzuf�gen, die sagt, dass in 15 Sekunden die Auktion endet
	addEndAuctionMessage(user, auctionNumber, 1);
	Sleep(5000);

	// Message hinzuf�gen, die sagt, dass in 10 Sekunden die Auktion endet
	addEndAuctionMessage(user, auctionNumber, 2);
	Sleep(5000);

	// Message hinzuf�gen, die sagt, dass in 5 Sekunden die Auktion endet
	addEndAuctionMessage(user, auctionNumber, 3);
	Sleep(5000);

	// Auktion beenden
	endAuction(auctionNumber);
}

// Erzeugt neue Nachricht, die interessierte Nutzer daraufhinweist, dass Auktion bald endet und f�gt diese der Messagebox hinzu.
void addEndAuctionMessage(wstring user, unsigned long auctionNumber, int warningNr)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
			{
				//wstring highestBid = to_wstring((*it).highestBid);
				wstring highestBidCut = to_wstring((*it).highestBid);
				vector<wstring> newMessage;
				unsigned long messageType = 1;						// MessageType = 1 => kurz vor Auktionsende-Message
				newMessage.push_back((*it2));						// (0) Name des interessierten Users, der die Nachricht erhalten soll	
				newMessage.push_back(to_wstring(messageType));		// (1) MessageType
				newMessage.push_back(to_wstring(warningNr));		// (2) Warnungkategorie
				newMessage.push_back((*it).articleName);			// (3) Artikelname
				newMessage.push_back(to_wstring(auctionNumber));	// (4) Auktionsnummer
				newMessage.push_back(highestBidCut);				// (5) H�chstgebot
				Messages.push_back(newMessage);						// neue Nachricht der Messagebox hinzuf�gen
			}
			(*it).auctionStatus = 1;								// Auktionsstatus wird auf "kurz vor Schluss" ge�ndert
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
			(*it).auctionStatus = 2;								// Auktionsstatus wird auf "Beendet" ge�ndert
			for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
			{
				//wstring highestBid = to_wstring((*it).highestBid);
				wstring highestBidCut = to_wstring((*it).highestBid);
				// Message, dass Auktion beendet wurde	
				vector<wstring> newMessage;
				unsigned long messageType = 2;							// MessageType = 2 => Auktion beendet
				newMessage.push_back((*it2));							// (0) User, der die Nachricht erhalten soll
				newMessage.push_back(to_wstring(messageType));			// (1) MessageType
				newMessage.push_back((*it).highestBidder);				// (2) H�chstbietender
				newMessage.push_back((*it).articleName);				// (3) Artikelname
				newMessage.push_back(highestBidCut);					// (4) H�chstgebot
				newMessage.push_back(to_wstring((*it).auctionStatus));	// (5) Auktionsstatus
				Messages.push_back(newMessage);							// neue Nachricht der Messagebox hinzuf�gen
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
}
