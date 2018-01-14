#include "COMMyBayCln.h"
#include <atlsafe.h>
#include <iostream>
#include <vector>
#import "COMMyBaySrv.dll"
#include "CharStringConverter.h"
#include "BstrStringConverter.h"
#include <thread>
#include <windows.h>

using namespace std;

int main(int argc, char**argv)
{
	CoInitialize(NULL);
	IClassFactory *pCF = NULL;

	ICOMMyBay *p_ICOMMyBaySrv = NULL;
	HRESULT hr;

	IID IID_tmpICOMMyBay = IID_ICOMMyBay;

	// Erzeugung auf anderem Rechner (auch mit Rechernamen)
	COSERVERINFO srvInfo = { 0, L"127.0.0.1", 0, 0 };
	LPWSTR lpwstr_SrvName = 0;
	// Überträgt die IP falls über die Kommandozeile eine zusätzliche IP für den Server angegeben wird
	if (argc > 1)
	{
		wstring wstr_SrvName(argv[1], argv[1] + strlen(argv[1]) + 1);
		lpwstr_SrvName = new WCHAR[wstr_SrvName.length()];
		StrCpyNW(lpwstr_SrvName, wstr_SrvName.c_str(), wstr_SrvName.length());
		copy(argv[1], argv[1] + strlen(argv[1]), lpwstr_SrvName);

		srvInfo.pwszName = lpwstr_SrvName;
	}

	MULTI_QI multiQi = { &IID_tmpICOMMyBay, 0, 0 };

	hr = CoCreateInstanceEx(CLSID_COMMyBay, NULL, CLSCTX_REMOTE_SERVER,
		&srvInfo, 1, &multiQi);
	
	if (FAILED(hr))
	{
		std::cout << "Failure: CoCreateInstanceEx - " << std::hex << hr << std::endl;
		return FALSE;
	}

	p_ICOMMyBaySrv = (ICOMMyBay*)multiQi.pItf;

	readConsole(p_ICOMMyBaySrv, srvInfo);

	p_ICOMMyBaySrv->Release();
	CoUninitialize();


	return 0;
}

// Liest die Eingaben der Console
void readConsole(ICOMMyBay *p_ICOMMyBaySrv, COSERVERINFO srvInfo)
{
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "-		Herzlich Willkommen bei MyBay - Ihrem Auktionshaus		 -" << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	cout << endl;
	cout << "Bitte loggen Sie sich zuerst ein." << endl;
	cout << endl;

	unsigned long sessionID = 0;
	boolean threadAllow = TRUE;

	std::thread MessageThread(pullMessages, &sessionID, &threadAllow, srvInfo);

	while (true)
	{
		cout << endl;
		wstring scommand;
		std::getline(std::wcin, scommand);
		int i = 0;
		args.clear();

		//Eingabe parsen und Argumente heraussuchen.Als Trennung zwischen Argumenten wird ein Leerzeichen erwartet
		while ((i = scommand.find(L" ")) != -1)
		{
			if (i != 0)
			{
				args.push_back(scommand.substr(0, i));									// Argument bis zum nächsten Leerzeichen zur Argumenten-Liste hinzufügen
				scommand = scommand.substr(i + 1, scommand.length());					// Argument vom String entfernen
			}
		}
		// letztes Argument zur Liste der Argumente hinzufügen
		args.push_back(scommand);
		interpretCommand(p_ICOMMyBaySrv, &sessionID, args, &threadAllow);
		if (args.at(0) == L"bye")
		{
			break;
		}
	}
	// Main-Thread waretet auf MessageThread bis dieser fertig ist
	MessageThread.join();

}

// Eingaben werden entsprechend interpretiert
void interpretCommand(ICOMMyBay *p_ICOMMyBaySrv, unsigned long *sessionID, std::vector<std::wstring> args, boolean *threadAllow)
{
	HRESULT hr;
	if (args.size() < 0)
	{
		return;
	}

	// login
	if (args.at(0) == L"user")
	{
		if (args.size() < 3)
		{
			cerr << "Nicht genug Parameter." << endl;
			cerr << "login <Username> <Passwort>" << endl;
			return;
		}
		else if (args.size() > 3)
		{
			cerr << "Zu viele Parameter." << endl;
			cerr << "login <Username> <Passwort>" << endl;
			return;
		}
		hr = p_ICOMMyBaySrv->login(wstr_to_bstr(args.at(1)), wstr_to_bstr(args.at(2)), sessionID);
		if (hr == S_OK)
		{
			wcout << "Einloggen erfolgreich" << endl;
		}
		else if (hr == ERROR_ALREADY_LOGGED_IN)
			cerr << "Fehler: Sie sind bereits eingeloggt." << endl;
		else if (hr == ERROR_USERNAME_OR_PASSWORD_WRONG)
			cerr << "Fehler: Benutzername oder Passwort falsch." << endl;
		else if (hr == ERROR_FILE_COULD_NOT_BE_OPENED)
			cerr << "Fehler: Userfile konnte nicht geoeffnet werden!" << endl;
		else if (hr == 0x800706BA)
		{
			cerr << "Verbindung zum Server verloren!" << endl;
			cerr << "Der Client wird beendet. Bitte druecken Sie Enter." << endl;
			cin.get();
			exit(hr);
		}
		else
			cerr << "Es ist ein unerwarteter Fehler aufgetreten. Bitte versuche es erneut." << endl;
		wcout << endl;
	}

	// logout
	else if (args.at(0) == L"bye")
	{
		hr = p_ICOMMyBaySrv->logout((*sessionID));
		if (hr == S_OK)
		{
			cout << " *** Auf Wiedersehen! *** " << endl;
			*threadAllow = FALSE;
		}
		else if (hr == ERROR_USER_NOT_LOGGED_IN)
			cout << "Fehler: Nicht eingeloggt" << endl;
		else
			wcout << "Fehler: Ein unerwarteter Fehler ist aufgetreten" << endl;
	}

	// Auktion eines Artikels starten
	else if (args.at(0) == L"offer")
	{
		if (args.size() < 3)
		{
			cerr << "Nicht genug Parameter." << endl;
			cerr << "offer <Artikelname> <Mindestgebot>" << endl;
			return;
		}
		else if (args.size() > 3)
		{
			cerr << "Zu viele Parameter." << endl;
			cerr << "offer <Artikelname> <Mindestgebot>" << endl;
			return;
		}
		else
		{
			unsigned long auctionNumber;
			double startBid;
			// Prüfe ob das Startgebot eine Zahl ist und wandle sie in double 
			try
			{
				startBid = stod(args.at(2));
			}
			catch (invalid_argument e)
			{
				cout << "Falscher Parameter - das Startgebot muss eine Zahl sein!" << endl;
				return;
			}

			hr = p_ICOMMyBaySrv->offer(*sessionID, wstr_to_bstr(args.at(1).c_str()), startBid, &auctionNumber);
			if (hr == S_OK)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Die Auktion wurde gestartet." << endl;
				cout << "Ihre Auktion hat die Auktionsnummer: " << auctionNumber << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			else if (hr == ERROR_USER_NOT_LOGGED_IN)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Bitte zuerst einloggen!" << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}

			else if (hr == ERROR_ARTICLENAME_IS_EMPTY)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Artikelname muss angegeben sein." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;

			}
			else if (hr == ERROR_VALUE_NEGATIVE)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Startgebot darf nicht negativ sein." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
		}
	}

	// Nutzer interessiert sich für eine Auktion
	else if (args.at(0) == L"interested")
	{
		if (args.size() < 2)
		{
			cerr << "Nicht genug Parameter." << endl;
			cerr << "interested <Auktionsnummer>" << endl;
			return;
		}
		else if (args.size() > 2)
		{
			cerr << "Zu viele Parameter." << endl;
			cerr << "interested <Auktionsnummer>" << endl;
			return;
		}
		else
		{
			ULONG auctionNumber;

			// konvertiert wstring zu ULONG
			try
			{
				string tempAucNmr(args.at(1).begin(), args.at(1).end());
				auctionNumber = strtoul(tempAucNmr.c_str(), NULL, 0);
			}
			catch (invalid_argument e)
			{
				cout << "Falscher Parameter - das Startgebot muss eine Zahl sein!" << endl;
				return;
			}
			hr = p_ICOMMyBaySrv->interested(*sessionID, auctionNumber);
			if (hr == S_OK)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cerr << "Sie werden ab sofort ueber Neues der Auktion benachrichtigt." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}

			else if (hr == ERROR_USER_NOT_LOGGED_IN)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Bitte zuerst einloggen." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			else if (hr == ERROR_AUCTIONNUMBER_NOT_AVAILABLE)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Auktion nicht vorhanden." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}

			else if (hr == ERROR_USER_ALREADY_INTERESTED)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Sie sind bereits unter den Interessierten der Auktion." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			else
				cerr << "Es ist ein unerwarteter Fehler aufgetreten. Bitte versuche es erneut." << endl;
		}
	}
	else if (args.at(0) == L"listauctions")
	{

		SAFEARRAY_VAR auctions;
		if (args.size() > 3)
		{
			cerr << "Zu viele Parameter." << endl;
			cerr << "listauctions [-a][-A] [<Artikelnameteil]" << endl;
			return;
		}

		ULONG flag = 0;
		unsigned long countAuctions;
		wstring artNamePart = L"";

		// Flags entsprechend setzen
		// Flag = 0 -> ohne [-a] und [-A]
		// Flag = 1 -> [-a]
		// Flag = 2 -> [-A]
		if (args.size() > 1)
		{
			if (args.at(1) == L"\-a")
				flag = 1;
			else if (args.at(1) == L"\-A")
				flag = 2;
		}
		if (args.size() == 2 && flag == 0)
		{
			artNamePart = args.at(1);
		}
		// Falls ein Artikelnameteil angegben wurde
		if (args.size() > 2)
		{
			artNamePart = args.at(2);
		}
		hr = p_ICOMMyBaySrv->getAuctions(*sessionID, flag, wstr_to_bstr(artNamePart), &countAuctions, &auctions);
		if (hr == S_OK)
		{
			CComSafeArray<VARIANT> retAuctions(auctions);
			vector<wstring> allAuctVec;
			// Alle Elemente im SAFEARRAY werden in einen Vector übertragen
			for (int i = 0; i < retAuctions.GetCount(); i++)
			{
				//BSTR vari = retAuctions[i].bstrVal;
				//wstring tmp = bstr_to_wstr(retAuctions[i].bstrVal);
				allAuctVec.push_back(bstr_to_wstr(retAuctions[i].bstrVal));
			}

			// Ausgabe der Auktionen
			int cnt = 0;
			wcout << endl;

			/* Testprint */
			int i = 10;
			std::cout.width(15); std::cout << left << "Auktionsnummer";
			std::cout.width(20); std::cout << left << "Artikelname";
			std::cout.width(15); std::cout << left << "Hoechstgebot";
			std::cout.width(15); std::cout << left << "Auktionsstatus";
			std::cout.width(15); std::cout << left << "Anzahl Gebote" << endl;
			std::cout << "----------------------------------------------------------------------------------------" << endl;
			/* End Testprint */

			for (std::vector<wstring>::iterator it = allAuctVec.begin(); it != allAuctVec.end(); it++)
			{
				if (cnt == 0)
				{
					std::wcout.width(15); std::wcout << left << (*it);
					cnt++;
				}

				else if (cnt == 1)
				{
					std::wcout.width(20); std::wcout << left << (*it);
					cnt++;
				}

				else if (cnt == 2)
				{
					std::wcout.width(15); std::wcout << left << (*it).substr(0, (*it).size() - 4);
					cnt++;
				}
				else if (cnt == 3)
				{
					std::wcout.width(15); std::wcout << left << (*it);
					cnt++;
				}
				else if (cnt == 4)
				{
					std::wcout.width(15); std::wcout << left << (*it).substr(0, (*it).size() - 4);
					cnt = 0;
					wcout << endl;
				}
			}
			std::cout << "----------------------------------------------------------------------------------------" << endl;
			wcout << endl;

		}
		else if (hr == ERROR_USER_NOT_LOGGED_IN)
		{
			cout << "----------------------------------------------------------------------------------------" << endl;
			cout << "Fehler: Bitte zuerst einloggen." << endl;
			cout << "----------------------------------------------------------------------------------------" << endl;
		}

		else if (hr == ERROR_NO_AUCTIONS_AVAILABLE)
		{
			cout << "----------------------------------------------------------------------------------------" << endl;
			cout << "Ihre Eingabe ergab keine Ergebnisse." << endl;
			cout << "----------------------------------------------------------------------------------------" << endl;
		}
		else
			cerr << "Es ist ein unerwarteter Fehler aufgetreten. Bitte versuche es erneut." << endl;
	}
	else if (args.at(0) == L"bid")
	{
		if (args.size() < 3)
		{
			cerr << "Error: Nicht genug Parameter." << endl;
			cerr << "bid <Auktionsnummer> <Geldbetrag>" << endl;
			return;
		}
		else if (args.size() > 3)
		{
			cerr << "Error: Zu viele Parameter." << endl;
			cerr << "bid <Auktionsnummer> <Geldbetrag>" << endl;
			return;
		}
		else
		{
			ULONG auctionNumber;
			double bidVal;
			try
			{
				// Konvertiert auctionNumber in ULONG
				string tempAucNmr(args.at(1).begin(), args.at(1).end());
				auctionNumber = strtoul(tempAucNmr.c_str(), NULL, 0);
				// Konvertiert bidVal in double
				string tempbidVal(args.at(2).begin(), args.at(2).end());
				bidVal = strtod(tempbidVal.c_str(), NULL);
			}
			catch (invalid_argument e)
			{
				cout << "Falscher Parameter - das Startgebot muss eine Zahl sein!" << endl;
				return;
			}
			hr = p_ICOMMyBaySrv->bid(*sessionID, auctionNumber, bidVal);
			if (hr == S_OK)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cerr << "Gebot erfolgreich abgegeben." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}

			else if (hr == ERROR_USER_NOT_LOGGED_IN)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Bitte zuerst einloggen." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}

			else if (hr == ERROR_AUCTIONNUMBER_DOES_NOT_ESXIST)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Auktion nicht vorhanden." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			else if (hr == ERROR_BID_NEGATIVE)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Das Startgebot muss eine positive Zahl sein." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			else if (hr == ERROR_AUCTION_CLOSED)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Die Auktion ist bereits geschlossen." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}

			else if (hr == ERROR_BID_TOO_LOW)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Das Gebot muss groesser als das Hoechstgebot sein." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;

			}
			else
				cerr << "Es ist ein unerwarteter Fehler aufgetreten. Bitte versuche es erneut." << endl;
		}
	}
	else if (args.at(0) == L"details")
	{
		if (args.size() < 2)
		{
			cerr << "Nicht genug Parameter." << endl;
			cerr << "details <Auktionsnummer>" << endl;
			return;
		}
		else if (args.size() > 2)
		{
			cerr << "Zu viele Parameter." << endl;
			cerr << "details <Auktionsnummer>" << endl;
			return;
		}
		else
		{
			ULONG auctionNumber;
			SAFEARRAY_VAR allBids;
			unsigned long countBids;
			// konvertiert wstring zu ULONG
			try
			{
				string tempAucNmr(args.at(1).begin(), args.at(1).end());
				auctionNumber = strtoul(tempAucNmr.c_str(), NULL, 0);
			}
			catch (invalid_argument e)
			{
				cout << "Falscher Parameter - das Startgebot muss eine Zahl sein!" << endl;
				return;
			}
			hr = p_ICOMMyBaySrv->details(*sessionID, auctionNumber, &allBids, &countBids);
			if (hr == S_OK)
			{
				if (countBids == 0)
				{
					cout << "----------------------------------------------------------------------------------------" << endl;
					cout << "Bisher wurden für diese Auktion keine Gebote abgegeben." << endl;
					cout << "----------------------------------------------------------------------------------------" << endl;

				}
				else
				{
					CComSafeArray<VARIANT> retAllBids(allBids);
					vector<wstring> allBidsVec;
					// Alle Elemente im SAFEARRAY werden in einen Vector übertragen
					for (int i = 0; i < retAllBids.GetCount(); i++)
					{
						allBidsVec.push_back(bstr_to_wstr(retAllBids[i].bstrVal));
					}

					// Ausgabe aller Gebote
					int columnLengthBidder = 20;
					cout << "----------------------------------------------------------------------------------------" << endl;
					wcout << "Folgende Gebote wurden fuer die Auktion " << auctionNumber << " bisher abgegeben:" << endl;
					cout << endl;
					wcout << "Nr	" << "Bieter           " << "Gebot" << endl;		// 17 
					wcout << "______________________________________________" << endl;
					for (int i = 0; i < allBidsVec.size() - 2; i += 3)
					{
						wcout << allBidsVec[i];						// Gebotsnummer
						wcout << "    ";
						wcout << allBidsVec[i + 1];					// Bietername
						for (int k = 0; k < columnLengthBidder - allBidsVec[i + 1].size(); k++)
						{
							wcout << " ";
						}
						wcout << "|" << allBidsVec[i + 2].substr(0, allBidsVec[i + 2].size() - 4) << endl;	// Gebot mit nur 2 Nachkommastellen
					}
					cout << "----------------------------------------------------------------------------------------" << endl;
					cout << endl;
				}
			}
			else if (hr == ERROR_USER_NOT_LOGGED_IN)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Bitte zuerst einloggen." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}

			else if (hr == ERROR_USER_IS_NOT_AUCTIONEER)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Sie koennen nur als Auktionator alle Gebote einsehen." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			else
				cerr << "Es ist ein unerwarteter Fehler aufgetreten. Bitte versuche es erneut." << endl;
		}

	}
	else if (args.at(0) == L"endauction")
	{
		if (args.size() < 2)
		{
			cerr << "Nicht genug Parameter." << endl;
			cerr << "details <Auktionsnummer>" << endl;
			return;
		}
		else if (args.size() > 2)
		{
			cerr << "Zu viele Parameter." << endl;
			cerr << "details <Auktionsnummer>" << endl;
			return;
		}
		else
		{
			ULONG auctionNumber;
			// konvertiert wstring zu ULONG
			try
			{
				string tempAucNmr(args.at(1).begin(), args.at(1).end());
				auctionNumber = strtoul(tempAucNmr.c_str(), NULL, 0);
			}
			catch (invalid_argument e)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Falscher Parameter - das Startgebot muss eine Zahl sein!" << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
				return;
			}
			hr = p_ICOMMyBaySrv->endauction(*sessionID, auctionNumber);
			if (hr == S_OK)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Auktion wird in 15 Sekunden beendet." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			else if (hr == ERROR_USER_NOT_LOGGED_IN)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Bitte zuerst einloggen." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			else if (hr == ERROR_AUCTION_CLOSED)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Die Auktion ist bereits geschlossen." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			else if (hr == ERROR_USER_IS_NOT_AUCTIONEER)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Fehler: Sie koennen nur Auktionen beenden, die Sie auch erstellt haben." << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}
			else
				cerr << "Es ist ein unerwarteter Fehler aufgetreten. Bitte versuche es erneut." << endl;
		}
	}
	else if (args.at(0) == L"help")
	{
		cout << "******************** MyBAY ********************" << endl;
		cout << "login <Username> <Passwort>" << endl;
		cout << "logout" << endl;
		cout << "offer <Artikelname> <Mindestgebot>" << endl;
		cout << "interested <Auktionsnummer>" << endl;
		cout << "listauctions [-a][-A] [<Artikelnameteil]" << endl;
		cout << "bid <Auktionsnummer> <Geldbetrag>" << endl;
		cout << "details <Auktionsnummer>" << endl;
		cout << "endauction <Auktionsnummer" << endl;
		cout << "******************** MyBAY ********************" << endl;
		cout << endl;
	}
	else
	{
		cout << "Error: keine zulässige Eingabe" << endl;
	}

}