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

// CCOMMyBay
CCOMMyBay::CCOMMyBay()
	: m_AuthService(&std::wstring(L"USERDATA"))
{
	////m_AuthService.readRegisteredUser(L"mybay_user.txt");
	//num_auct = 1;

	//std::ifstream is;
	//is.open( + "..\..\_data\MyInventArticles.csv", std::ios::in);
	//if (is)
	//{
	//	std::string sauctionNumber, sarticleName, sstartBid, shighestBid, shighestBidderName, sbidder, sbidderValue, sinterestedUsers;
	//	int auctionLine = 0;
	//	int auctionIndex = 0;
	//	auction temp;
	//	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> stringConverter;

	//	// eine Auktion wird in der CSV-Datei über 3 Zeilen gespeichert
	//	while (getline(is, sauctionNumber, ';'))
	//	{
	//		// 1. Zeile: Auktionsnummer, Artikelname, Startgebot, Höchstgebot, Höchstbietender
	//		if (auctionLine == 0)
	//		{
	//			getline(is, sarticleName, ';');
	//			getline(is, sstartBid, ';');
	//			getline(is, shighestBid, ';');
	//			getline(is, shighestBidderName, '\n');

	//			temp.auctionNumber = std::stoi(sauctionNumber);
	//			temp.articleName = stringConverter.from_bytes(sarticleName);	// konvertiert string zu wstring
	//			temp.startBid = std::stod(sstartBid);
	//			temp.highestBid = std::stod(shighestBid);
	//			temp.highestBidder = stringConverter.from_bytes(shighestBidderName);
	//			auctionLine++;
	//		}
	//		// 2. Zeile: Bieter mit jeweiligem Gebot (z.B.: Hans;100;Klaus;200;Werner;250)
	//		else if (auctionLine == 1)
	//		{
	//			int i = 0;
	//			while (getline(is, sbidder, ';'))
	//			{
	//				getline(is, sbidderValue, ';');
	//				temp.bidder.insert(std::pair<std::wstring, std::double_t>(stringConverter.from_bytes(sbidder), std::stod(sbidderValue)));
	//			}

	//			auctionLine++;
	//		}

	//		// 3.Zeile: User die sich für diese Auktion interessieren
	//		else if (auctionLine == 2)
	//		{
	//			while (getline(is, sinterestedUsers, ';'))
	//			{
	//				temp.interestedUsers.push_back(std::stoi(sinterestedUsers));
	//			}
	//			auctionList[auctionIndex] = temp;
	//			auctionLine = 0;
	//			auctionIndex++;
	//		}
	//	}
	//}
	//is.close();
}

ULONG CCOMMyBay::num_auct;

STDMETHODIMP CCOMMyBay::login(BSTR username, BSTR password, ULONG * sessionId, BSTR * errMsg)
{
	wifstream csvread;
	srand((unsigned)time(NULL)); // Zufallsgenerator initialisieren.

								 /* TODO: Abfrage ob User nicht schon eingeloggt ist.*/
	if (BOOL isLoggedIn = loginCheck(*username) == TRUE)
	{
		//cout << "Sie sind bereits angemeldet" << endl;
		return S_OK;
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


				return S_OK;
			}
		}
		csvread.close();
	}
	else
		return S_OK;

	return S_OK;
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
}

STDMETHODIMP CCOMMyBay::offer(ULONG sessionId, BSTR articleName, DOUBLE startBid, ULONG * auctionNumber, BSTR * errMsg)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return S_OK;
	}
	wstring user = getUserName(sessionId);
}

STDMETHODIMP CCOMMyBay::interested(ULONG sessionId, ULONG auctionNumber, BSTR* errMsg)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return S_OK;
	}
	wstring user = getUserName(sessionId);
}

STDMETHODIMP CCOMMyBay::getAuctions(ULONG sessionId, ULONG flags, BSTR articleName, ULONG* countAuctions, SAVEARRAY_VAR* auctions, BSTR* errMsg)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return S_OK;
	}
	wstring user = getUserName(sessionId);
}

STDMETHODIMP CCOMMyBay::bid(ULONG sessionId, ULONG auctionNumber, DOUBLE bidVal, BSTR* errMsg)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return S_OK;
	}
	wstring user = getUserName(sessionId);
}

STDMETHODIMP CCOMMyBay::details(ULONG sessionId, ULONG auctionNumber, SAVEARRAY_VAR* allBids, ULONG* countBids, BSTR* errMsg)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return S_OK;
	}
	wstring user = getUserName(sessionId);
}

STDMETHODIMP CCOMMyBay::endauction(ULONG sessionId, ULONG auctionNumber, BSTR* errMsg)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return S_OK;
	}
	wstring user = getUserName(sessionId);
}

STDMETHODIMP CCOMMyBay::getMessage(ULONG sessionId, BOOL*einBoolwasIchNichtWeiß, ULONG* messageType, SAVEARRAY_VAR* message, BSTR* errMsg)
{
	// Prüfen ob User eingeloggt ist
	if (BOOL isLoggedIn = loginCheck(sessionId) == FALSE)
	{
		return S_OK;
	}
	wstring user = getUserName(sessionId);
}
