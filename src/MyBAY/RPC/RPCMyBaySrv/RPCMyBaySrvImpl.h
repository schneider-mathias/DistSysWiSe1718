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
#include "RpcException.h"
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

/********************************************************************/
/*							Hilfsfunktionen							*/
/********************************************************************/

/// <summary>
/// Pr�fe ob User bereits eingeloggt ist
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <returns> Ob User bereits eingeloggt ist </returns>
BOOL loggedInCheck(unsigned long sessionId)
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

/// <summary>
/// Pr�fe ob User bereits eingeloggt ist
/// </summary>
/// <param name="username"> Username </param>
/// <returns> SessionId des Users </returns>
ULONG loginCheck(unsigned char* username)
{
	std::map<std::wstring, unsigned long>::iterator it;
	for (it = users.begin(); it != users.end(); ++it)
	{
		if (it->first == char_to_wstring((const char*)username) && it->second != 0)
		{
			unsigned long tmp = it->second;
			unsigned long tmp2 = (*it).second;
			return (*it).second;
		}
	}
	return 0;
}

/// <summary>
/// Usernamen zur zugeh�rigen SessionId ermitteln
/// </summary>
/// <param name="sessionId"> SessionId des Users </param>
/// <returns> Username </returns>
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

/// <summary>
/// Alle Auktionen aus der Datei auslesen und in AuctionList speichern
/// </summary>
void readAuctionsFromFile()
{
	std::wifstream auctionsFile;
	auctionsFile.open(L"C:/_MyBayData/MyBayAuctions.csv", std::ios::in);
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

/// <summary>
/// Um alle Auktionen persistent zu halten werden alle Auktionen in eine Datei geschrieben
/// </summary>
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

	auctionsFile.open(L"C:/_MyBayData/MyBayAuctions.csv", std::ios::out);
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

/// <summary>
/// Erzeugt neue Auktion und f�gt diese der Liste aller Auktionen hinzu
/// </summary>
/// <param name="sarticleName"> Artikelname </param>
/// <param name="startBid"> Startgebot </param>
/// <returns> Auktionsnummer </returns>
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

/// <summary>
/// Pr�ft, ob die Auktionsnummer vorhanden ist
/// </summary>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Auktionsnummer vorhanden </returns>
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

/// <summary>
/// User in die Liste der User hinzuf�gen, die sich f�r diese Auktion interessieren
/// </summary>
/// <param name="user"> Username </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Erfolgreich hinzugef�gt </returns>
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

/// <summary>
/// Status der Auktion 
/// </summary>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Auktionsstatus </returns>
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

/// <summary>
/// F�gt ein Gebot zur Auktion hinzu
/// </summary>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <param name="bidVal"> Gebot </param>
/// <param name="user"> Username </param>
/// <returns> Auktion erfolgreich hinzugef�gt </returns>
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

/// <summary>
/// Pr�ft ob Auktion bereits geschlossen oder kurz vor Schluss
/// </summary>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Auktionsstatus </returns>
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

/// <summary>
/// Pr�ft ob User die Auktion selbst erstellt hat und ob die Auktion mit der angegebenen Auktion �berhaupt vorhanden ist
/// </summary>
/// <param name="user"> Username </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> User ist Auktionator der Auktion </returns>
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

/// <summary>
/// Gr��e des zu serialisierenden String
/// </summary>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Gr��e des zu serialisierenden String </returns>
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

/// <summary>
/// Anzahl wie viel Gebote f�r die Auktion abgegeben wurden
/// </summary>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Anzahl Gebote </returns>
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

/// <summary>
/// Serialisiert die Gebote einer Auktion in chronologischer Reihenfolge
/// </summary>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> serialisierte Gebote </returns>
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
				//wstring tempBidStrCut = tempBidStr3.substr(0, tempBidStr3.size() - 4);				// da double 6 Nachkommastellen hat, werden die letzten 4 Nachkommastellen abgeschnitten
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

/// <summary>
/// Beendet die Auktion
/// </summary>
/// <param name="user"> Username </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Auktion beendet </returns>
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
				myThreads.push_back(std::thread(auctionEndProcess, user, auctionNumber));
				//std::thread EndAuktionThread(auctionEndProcess, user, auctionNumber);	// Threadpointer und dann hier starten
				return TRUE;
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return FALSE;
}

/// <summary>
/// Pr�ft ob f�r den User eine Nachricht verf�gbar ist
/// </summary>
/// <param name="user"> Username </param>
/// <returns> Nachricht f�r den User </returns>
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

/// <summary>
/// Anzahl der Messages die f�r den User in der Messagebox bereit liegen
/// </summary>
/// <param name="user"> Username </param>
/// <returns> Anzahl Nachrichten </returns>
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

/// <summary>
/// Serialisiert eine Nachricht
/// </summary>
/// <param name="newMessage"> Nachricht </param>
/// <returns> serialisierte Nachricht </returns>
wstring serializeMessage(vector<wstring> newMessage)
{
	wstring serStr = L"";
	newMessage.erase(newMessage.begin());	// erster und zweiter Wert wird entfernt, da nicht mehr interessant f�r wen die Nachricht ist
	newMessage.erase(newMessage.begin());
	for (std::vector<wstring>::iterator it = newMessage.begin(); it != newMessage.end(); it++)
	{
		serStr += (*it);
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;
	}
	return serStr;
}

/// <summary>
/// F�gt die Nachricht f�r alle Interessierten der Messagebox hinzu, dass ein neues Gebot abgegeben wurde
/// </summary>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <param name="bidVal"> Gebot </param>
/// <param name="user"> Username </param>
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
				//wstring bidValCut = tmpBidVal.substr(0, tmpBidVal.size() - 4);

				newMessage.push_back((*it2));												// (0) Name des interessierten Users
				newMessage.push_back(to_wstring(messageType));								// (1) MessageType
				if ((*it).interestedUserList.at(0) == (*it2))
					newMessage.push_back(user);												// (2) Name des Bieters
				else
					newMessage.push_back(L"");												// (2) Name des Bieters wird nur dem Auktionator angezeigt
				newMessage.push_back((*it).articleName);									// (3) Artikelname
				newMessage.push_back(to_wstring(bidVal));									// (4) Gebot
				newMessage.push_back(to_wstring((*it).auctionStatus));						// (5) Auktionsstatus

				Messages.push_back(newMessage);												// Nachricht der Messagebox hinzuf�gen
			}
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
}

/// <summary>
/// Liefert zu einer Auktionsnummer den entsprechenden Artikelnamen. Gibt leeren string zur�ck, wenn Auktionsnummer nicht vorhanden.
/// </summary>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <returns> Artikelname </returns>
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

/// <summary>
/// Filtert alle Auktionen, die mit dem Artikelnameteil �bereinstimmen
/// </summary>
/// <param name="user"> Username </param>
/// <param name="sarticleName"> Artikelname </param>
/// <returns> gefilterete Auktionen </returns>
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

/// <summary>
/// Filtert alle Auktionen, die die Bedingungen der Flags erf�llen
/// </summary>
/// <param name="user"> Username </param>
/// <param name="flags"> Optionen </param>
/// <param name="filteredArtNameAuctions"> Auktionen </param>
/// <returns> gefilterete Auktionen </returns>
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

/// <summary>
/// Suche na allen Auctionen, die unter der Angabe der Flags ([-a][-A]) und eines Artikelnamensteils 
/// </summary>
/// <param name="user"> Username </param>
/// <param name="flags"> Optionen </param>
/// <param name="sarticleName"> Artikelname </param>
/// <returns> alle Auktionen, die die Filterkriterien erf�llen </returns>
vector<auction> getInterestingAuctions(wstring user, unsigned long flags, wstring sarticleName)
{
	vector<auction> filteredArtNameAuctions = filterArtName(user, sarticleName);								// Filterstage 1
	vector<auction> filteredAuctionsByFlags = filterAuctionsByFlags(user, flags, filteredArtNameAuctions);		// Filterstage 2
	return filteredAuctionsByFlags;
}

/// <summary>
/// Serialisierung der Auktionen f�r getAuctions 
/// </summary>
/// <param name="interestingAuctions"> Auktionen </param>
/// <returns> serialisierte Auktionen </returns>
wstring serializeAuctions(vector<auction> interestingAuctions)
{
	wstring serStr = L"";
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = interestingAuctions.begin(); it != interestingAuctions.end(); it++)
	{
		wstring highestBidCut = to_wstring((*it).highestBid);
		// Gebot zum String hinzuf�gen
		serStr += to_wstring((*it).auctionNumber);					// (0) Auktionsnummer
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
		serStr += (*it).articleName;								// (1) Artikelname
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
		serStr += highestBidCut;									// (2) H�chstgebot
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
		serStr += to_wstring((*it).auctionStatus);					// (3) Auktionsstatus
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
		serStr += to_wstring((*it).BidderList.size());				// (4) Zahl der aktuellen Gebote
		serStr += PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION;	// Platzhalter 
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	return serStr;
}


/********************************************************************/
/*							EndAuktionThread						*/
/********************************************************************/

/// <summary>
/// F�gt f�r jeden an der Auktion interessierten Nutzer eine neue Nachricht in die Messagebox hinzu, dass die Auktion bald beendet wird.
/// </summary>
/// <param name="user"> Username </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
void auctionEndProcess(wstring user, unsigned long auctionNumber)
{
	try {
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
	catch (RpcException e)
	{
		if (e.GetStatus() == RPC_S_SERVER_UNAVAILABLE)
		{
			printf("\t[Error]: No server connection. Stop client.\n");
		}
		else
		{
			printf("Unknown error\nstat=0x%x, text=%s, type=%s\n",
				(int)e.GetStatus(), e.GetErrorText(), e.GetErrorType());
		}
	}
}

/// <summary>
/// Erzeugt neue Nachricht, die interessierte Nutzer daraufhinweist, dass Auktion bald endet und f�gt diese der Messagebox hinzu.
/// </summary>
/// <param name="user"> Username </param>
/// <param name="auctionNumber"> Auktionsnummer </param>
/// <param name="warningNr"> Warnungsnummer </param>
void addEndAuctionMessage(wstring user, unsigned long auctionNumber, int warningNr)
{
	EnterCriticalSection(critSecWrapper.getInstance());
	for (std::vector<auction>::iterator it = AuctionList.begin(); it != AuctionList.end(); ++it)
	{
		if (auctionNumber == (*it).auctionNumber)
		{
			for (std::vector<wstring>::iterator it2 = (*it).interestedUserList.begin(); it2 != (*it).interestedUserList.end(); ++it2)
			{
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
	writeAuctionsToFile();
}

/// <summary>
/// Beendet die Auktion
/// </summary>
/// <param name="auctionNumber"> Auktionsnummer </param>
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
				wstring highestBidCut = to_wstring((*it).highestBid);
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
			// Liste aller Interessierten User l�schen bis auf 1., da dieser der Auktionator ist
			wstring auctioneer = (*it).interestedUserList.at(0);
			(*it).interestedUserList.clear();
			(*it).interestedUserList.push_back(auctioneer);
		}
	}
	LeaveCriticalSection(critSecWrapper.getInstance());
	writeAuctionsToFile();
}
