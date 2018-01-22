/************************************************************/
/*                                                          */
/* Inhalt:    RPCMyBayCln						            */
/*                                                          */
/* Autor:	  Johannes Sauer		                        */
/* Stand:     23. Jan 2018                                  */
/*															*/
/************************************************************/

#include "MyBay_i_h.h"    
#include "Config.h"
#include "RpcException.h"
#include <iostream>
#include <vector>
#include "CharStringConverter.h"
#include "RPCMyBayCln.h"
#include <thread>
#include <windows.h>


using namespace std;

/// <summary>
/// Eintrittspunkt für diese Anwendung
/// </summary>
/// <param name="argc"> Die Anzahl der Kommandozeilen Argumente </param>
/// <param name="argv"> Ein Array mit den Kommandozeilen Argumenten als Strings </param>
void main(int argc, char**argv)
{
	char* srvAdress = NULL;

	// legt bei einer zusätzlichen Eingabe die IP-Adresse des Servers fest
	if (argc > 1)
	{
		srvAdress = (char*)malloc((strlen(argv[1]) + 1) * sizeof(char));
		srvAdress = _strdup(argv[1]);
	}
	try
	{
		Bind(srvAdress);
		try
		{
			rpcCalls();
		}
		catch (...)
		{
			UnBind();
			throw;
		}
		/* RPC-Bindung freigeben */
		UnBind();
	}
	catch (RpcException& e)
	{
		if (e.GetStatus() == RPC_S_SERVER_UNAVAILABLE)
		{
			cerr <<  "Fehler: Der Server antwortet nicht!\nClient beenden."  << endl;
		}
		else
		{
			printf("Unbekannter Fehler\nstat=0x%x, text=%s, type=%s\n",
				(int)e.GetStatus(), e.GetErrorText(), e.GetErrorType());
		}
	}

	if (srvAdress != NULL)
		free(srvAdress);

	printf("Enter druecken!\n");
	getchar();
}

/// <summary>
/// RPC Aufrufe
/// </summary>
void rpcCalls(void)
{
	RpcTryExcept
	{
		readConsole();
	}
		RpcExcept(1)
	{
		RpcException::Raise(RpcExceptionCode(), "RPC call failed", "Remote Procedure Call");
	}
	RpcEndExcept
}

/// <summary>
/// Binding
/// </summary>
/// <param name="argc"> Adresse zum Remoteserver </param>
void Bind(char* remoteNetwAddr)
{
	RPC_STATUS status;
	unsigned char *protocolSequence = (UCHAR*)MYBAY_RPC_PROT_SEQ;
	unsigned char *endpoint = (UCHAR*)MYBAY_RPC_ENDPOINT;
	unsigned char *netwAddr = (UCHAR*)MYBAY_RPC_DEF_NETWADDR;

	if (NULL != remoteNetwAddr)
		netwAddr = (UCHAR*)remoteNetwAddr;

	unsigned char *stringBinding = NULL;


	// Erzeugung der Stringdarstellung des Binding-Handles 
	status = RpcStringBindingCompose(NULL,             // Objekt UUID
		protocolSequence, // Server-/Verbindungsdaten
		netwAddr,
		endpoint,
		NULL,             // Keine Optionen fuer TCP/IP
		&stringBinding);
	if (status)
	{
		throw(RpcException(status, (string)"RpcStringBindingCompose failed: Network Address = " +
			(char*)netwAddr,
			"RPC Runtime Error"));
	}

	// Erzeugung des Binding-Handles
	status = RpcBindingFromStringBinding(stringBinding,
		&hMyBay_i);
	if (status)
	{
		throw(RpcException(status, (string)"RpcBindingFromStringBinding failed: String = " +
			(char*)stringBinding,
			"RPC Runtime Error"));
	}

	// Freigabe der Stringdarstellung des Binding-Handles 
	status = RpcStringFree(&stringBinding);
	if (status)
	{
		throw(RpcException(status, "RpcStringFree failed", "RPC Error"));
	}
}

/// <summary>
/// Unbinding
/// </summary>
void UnBind(void)
{
	RPC_STATUS status;

	// Freigabe des Binding-Handles 
	status = RpcBindingFree(&hMyBay_i);
	if (status)
	{
		throw(RpcException(status, "RpcBindingFree failed", "RPC Error"));
	}
}

/// <summary>
/// Einlesen der Kommandos von der Kommandozeile und starten des Threads
/// </summary>
void readConsole()
{
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "-		Herzlich Willkommen bei MyBay - Ihrem Auktionshaus		 -" << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	cout << endl;
	cout << "Bitte loggen Sie sich zuerst ein." << endl;
	cout << endl;

	unsigned long sessionID = 0;
	boolean threadAllow = TRUE;
	
	std::thread MessageThread(pullMessages, &sessionID, &threadAllow);

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
		interpretCommand(&sessionID, args, &threadAllow);
		if (args.at(0) == L"bye")
		{
			break;
		}
	}
	// Main-Thread waretet auf MessageThread bis dieser fertig ist
	MessageThread.join();

}

/// <summary>
/// Die Kommandozeilen-Eingaben werden entsprechend interpretiert, die zugehörige Funktion ausgeführt
/// und die Ausgabe erzeugt.
/// </summary>
/// <param name="sessionID"> Die SessionId des Clients </param>
/// <param name="args"> Ein Vector aus Strings mit den Kommandozeilen Argumenten </param>
/// <param name="threadAllow"> Die Erlaubnis, ob der Thread zum Pullen der Nachrichten ausgeführt wird oder nicht </param>
void interpretCommand(unsigned long *sessionID, std::vector<std::wstring> args, boolean *threadAllow)
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
		// login auf Server ausführen
		hr = login((unsigned char*)wstring_to_char(args.at(1).c_str()), (unsigned char*)wstring_to_char(args.at(2).c_str()), sessionID);
		// HRESULt interpretieren 
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
		// logout auf Server ausführen
		hr = logout((*sessionID));
		// HRESULt interpretieren 
		if (hr == RPC_S_OK)
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
			// Auktion wird gestartet
			hr = offer(*sessionID, (unsigned char*)wstring_to_char(args.at(1).c_str()), startBid, &auctionNumber);
			//MyOwnAuctions.push_back(auctionNumber);						// füge die neue Auktion der Liste meiner eigenen Auktionen hinzu
			// HRESULt interpretieren 
			if (hr == S_OK)
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Die Auktion wurde gestartet." << endl;
				cout << "Deine Auktion hat die Auktionsnummer: " << auctionNumber << endl;
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
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "Falscher Parameter - das Startgebot muss eine Zahl sein!" << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
				return;
			}
			// User wird als interessierter User für die angegebene Auktion registriert
			hr = interested(*sessionID, auctionNumber);
			// HRESULT interpretieren 
			if (hr == RPC_S_OK)
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

	// Gibt alle Auktionen aus, die den Eingaben entsprechen
	else if (args.at(0) == L"listauctions")
	{
		String_t auctions = { 0,0,NULL };
		if (args.size() > 3)
		{
			cerr << "Zu viele Parameter." << endl;
			cerr << "listauctions [-a][-A] [<Artikelnameteil]" << endl;
			return;
		}

		ULONG flag = 0;
		unsigned long countAuctions;
		char* artNamePart = "";

		// Falls Optionen angegeben wurden
		if (args.size() > 1)
		{
			if (args.at(1) == L"\-a")
				flag = 1;
			else if (args.at(1) == L"\-A")
				flag = 2;
		}

		// Falls nur ein Artikelnameteil angegeben wurde, ohne die Optionen -a oder -A
		if (args.size() == 2 && flag == 0)
		{
			artNamePart = wstring_to_char(args.at(1));
		}

		// Falls ein Artikelnameteil angegben wurde
		if (args.size() > 2)
		{
			artNamePart = wstring_to_char(args.at(2));
		}

		// Auktionen von Server anfragen 
		hr = getAuctions(*sessionID, flag, (unsigned char*) artNamePart, &countAuctions, &auctions);
		
		// HRESULT interpretieren 
		if (hr == RPC_S_OK)
		{
			vector<wstring> allAuctVec = deserialize(auctions.str, auctions.len);
			
			int cnt = 0;
			wcout << endl;
			int i = 10;
			// Ausgabe der Auktionen
			std::cout.width(15); std::cout << left << "Auktionsnummer";
			std::cout.width(20); std::cout << left << "Artikelname";
			std::cout.width(15); std::cout << left << "Hoechstgebot";
			std::cout.width(15); std::cout << left << "Status";
			std::cout.width(15); std::cout << left << "Anzahl Gebote" << endl;
			std::cout << "----------------------------------------------------------------------------------------" << endl;

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
					std::wcout.width(15);
					if ((*it) == L"0")
						wcout << left << "offen" ;
					if ((*it) == L"1")
						wcout << left << "kurz vor Ende";
					if ((*it) == L"2")
						wcout << left << "Beendet";
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

		// Speicher wieder freigeben
		MIDL_user_free(auctions.str);					
	}

	// Gebot abgeben
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

			// Gebot wird abgegeben
			hr = bid(*sessionID, auctionNumber, bidVal);
			
			// HRESULT interpretieren 
			if (hr == RPC_S_OK)
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

	// Alle Gebote zu einer Auktion anzeigen
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
			String_t allBids = {0,0,NULL};
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

			// Alle Gebote zu einer Auktion abholen
			hr = details(*sessionID, auctionNumber, &allBids, &countBids);

			// HRESULT interpretieren 
			if (hr == RPC_S_OK)
			{
				if (countBids == 0)
				{
					cout << "----------------------------------------------------------------------------------------" << endl;
					cout << "Bisher wurden für diese Auktion keine Gebote abgegeben." << endl;
					cout << "----------------------------------------------------------------------------------------" << endl;
					
				}
				else
				{
					vector<wstring> allBidsVec = deserialize(allBids.str, allBids.len);

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
						wcout << "  ";
						wcout << allBidsVec[i + 1];					// Bietername
						for (int k = 0; k < columnLengthBidder - allBidsVec[i + 1].size(); k++)
						{
							wcout << " ";
						}
						wcout << "|" << allBidsVec[i + 2].substr(0, allBidsVec[i + 2].size() - 4) << endl;	// Gebot
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
			
			MIDL_user_free(allBids.str);
		}
		
	}

	// Auktion beenden
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

			// Auktion wird am Server beendet
			hr = endauction(*sessionID, auctionNumber);
			
			// HRESULT interpretieren 
			if (hr == RPC_S_OK)
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

	// Zeigt eine Übersicht aller möglichen Befehle an
	else if (args.at(0) == L"help")
	{
		cout << "******************** MyBAY ********************" << endl;
		cout << "user <Username> <Passwort>" << endl;
		cout << "bye" << endl;
		cout << "offer <Artikelname> <Mindestgebot>" << endl;
		cout << "interested <Auktionsnummer>" << endl;
		cout << "listauctions [-a][-A] [<Artikelnameteil]" << endl;
		cout << "bid <Auktionsnummer> <Geldbetrag>" << endl;
		cout << "details <Auktionsnummer>" << endl;
		cout << "endauction <Auktionsnummer" << endl;
		cout << "******************** MyBAY ********************" << endl;
		cout << endl;
	}

	// Die Eingaben konnten nicht interpretiert werden
	else
	{
		cout << "Error: keine zulässige Eingabe" << endl;
	}

}
