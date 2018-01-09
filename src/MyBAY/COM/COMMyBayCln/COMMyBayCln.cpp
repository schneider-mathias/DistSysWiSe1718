//#include "AuthService.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>


#include <atlsafe.h>

//#import "COMMyBaySrv.dll"

#include "../COMMyBaySrv/COMMyBaySrv_i.c"
#include "../COMMyBaySrv/COMMyBaySrv_i.h"
#include "COMMyBayCln.h"


#include "BstrStringConverter.h"
using namespace std;


void readConsole(ICOMMyBay *p_ICOMMyBaySrv);
void interpretCommand(ULONG *sessionID, ICOMMyBay *p_ICOMMyBaySvr, std::vector<wstring> args);

int main(int argc, char**argv)
{
	CoInitialize(NULL);
	IClassFactory *pCF = NULL;

	ICOMMyBay *p_ICOMMyBaySrv = NULL;
	HRESULT hr;

	IID IID_tmpICOMMyINVENT = IID_ICOMMyBay;

	// Erzeugung auf anderem Rechner (auch mit Rechernamen)
	COSERVERINFO srvInfo = { 0, L"127.0.0.1", 0, 0 };
	LPWSTR lpwstr_SrvName = 0;
	// Überträgt die IP
	if (argc > 1)
	{
		wstring wstr_SrvName(argv[1], argv[1] + strlen(argv[1]) + 1);
		lpwstr_SrvName = new WCHAR[wstr_SrvName.length()];
		StrCpyNW(lpwstr_SrvName, wstr_SrvName.c_str(), wstr_SrvName.length());
		copy(argv[1], argv[1] + strlen(argv[1]), lpwstr_SrvName);

		srvInfo.pwszName = lpwstr_SrvName;
	}

	MULTI_QI multiQi = { &IID_tmpICOMMyINVENT, 0, 0 };

	hr = CoCreateInstanceEx(CLSID_COMMyBay, NULL, CLSCTX_REMOTE_SERVER,
		&srvInfo, 1, &multiQi);

	if (FAILED(hr))
	{
		std::cout << "Failure: CoCreateInstanceEx - " << std::hex << hr << std::endl;
		return FALSE;
	}

	p_ICOMMyBaySrv = (ICOMMyBay*)multiQi.pItf;

	std::cout << "--------------------------------------------------------" << std::endl;
	std::cout << "--------------Eingaben können durchgeführt werden-------" << std::endl;
	std::cout << "--------------------------------------------------------" << std::endl;
	

	readConsole(p_ICOMMyBaySrv);

	p_ICOMMyBaySrv->Release();
	CoUninitialize();


	return 0;
}

void readConsole(ICOMMyBay *p_ICOMMyBaySrv)
{
	ULONG sessionID = 0;
	while (true)
	{
		wchar_t* ccommand = new wchar_t[1];
		vector<wstring> args;
		wstring wscommand;
		int count = 0;
		vector<int> blankPositions;
		int startCommandPositon = 0;

		wcin.getline(ccommand, 655455);  //Gesamte Eingabe bis Enter einlesen
		wscommand = wstring(ccommand);

		// suche Befehl und zugehörige Argumente aus dem String
		for (size_t i = 0; i < wscommand.size(); i++)
		{
			// suche nach Leerzeichen und speichere deren Position
			if (wscommand[i] == ' ')
			{
				blankPositions.push_back(i);
			}
		}

		// suche alle Argumente aus Kommando heraus
		for (int i = 0; i < (int)blankPositions.size(); i++)
		{
			int argLength = blankPositions[i] - startCommandPositon;				// Länge des Arguments
			wstring singleArg = wscommand.substr(startCommandPositon, argLength);		// erzeugt das einzelne Argument
			args.push_back(singleArg);												// fügt das Argument der Liste hinzu
			startCommandPositon = blankPositions[i] + 1;
		}
		// letztes Argument
		wstring singleArg = wscommand.substr(startCommandPositon, wscommand.size());
		args.push_back(singleArg);

		interpretCommand(&sessionID, p_ICOMMyBaySrv, args);
		}
}

void interpretCommand(ULONG *sessionID, ICOMMyBay *p_ICOMMyBaySvr, std::vector<wstring> args)
{
	if ((int)args.size() < 0)
	{
		return;
	}

	HRESULT hr;
	BSTR errMsg;

	// login-Versuch
	if (args[0] == L"login")
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
		/*BSTR arg1 = wstr_to_bstr(args[1]);*/
		/*hr = p_ICOMMyBaySvr->login(wstr_to_bstr(args[1]), wstr_to_bstr(args[2]), sessionID, &errMessage)*/
		hr = S_OK;
		if (hr == S_OK)
			cout << "Einloggen erfolgreich" << endl;
		else if (hr == E_ACCESSDENIED)
			cerr << "Benutzername oder Passwort falsch." << endl;
		else if (hr == E_FAIL)
			cerr << "Serverproblem! Userfile konnte nicht geöffnet werden!" << endl;
		else if(hr == 0x800706BA) 
		{
			cerr << "Verbindung zum Server verloren!" << endl;
			cerr << "Der Client wird beendet. Bitte druecken Sie Enter." << endl;
			cin.get();
			exit(hr);
		}
		else
		{
			cerr << "Es ist ein unerwarteter Fehler aufgetreten. Bitte versuche es erneut." << endl;
		}

	}

	// logout-Versuch
	else if (args[0] == L"logout")
	{
		if (args.size() != 1)
		{
			cerr << "Für Logout werden keine weiteren Angaben benötigt." << endl;
			cerr << "Bitte tippen Sie nur: \"logout\" um sich abzumelden." << endl;
			return;
		}
		else
		{
			hr = p_ICOMMyBaySvr->logout(*sessionID, &errMsg);
			if (hr == S_OK)
			{
				cerr << "***** Ausloggen erfolgreich *****" << endl;
			}
			else
			{
				cout << "Sie sind nicht eingeloggt" << endl;
			}

		}
	}

	// Auktion eines Artikels starten
	else if (args[0] == L"offer")
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
			ULONG auctionNumber;
			DOUBLE startBid;
			// Prüfe ob das Startgebot eine Zahl ist und wandle sie in double 
			try
			{
				startBid = stod(args[2]);
			}
			catch (invalid_argument e)
			{
				cout << "Falscher Parameter - das Startgebot muss eine Zahl sein!" << endl;
				return;
			}

			//hr = p_ICOMMyBaySvr->offer(*sessionID, wstr_to_bstr(args[1]), startBid, &auctionNumber, &errMsg);
			if (hr == S_OK)
			{
				cout << "Eine Auktion für den Artikel wurde gestartet." << endl;
				cout << "Das Startgebot lautet: " << endl;
				MyAuctions.push_back(auctionNumber);		// fügt die Auktionsnummer zur Liste der eigenen Auktionen hinzu
			}
		}
	}
	else if (args[0] == L"interested")
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

			// convertiert wstring zu ULONG
			try
			{
				string tempAucNmr(args[1].begin(), args[1].end());
				auctionNumber = strtoul(tempAucNmr.c_str(), NULL, 0);
			}
			catch (invalid_argument e)
			{
				cout << "Falscher Parameter - das Startgebot muss eine Zahl sein!" << endl;
				return;
			}
			hr = p_ICOMMyBaySvr->interested(*sessionID, auctionNumber, &errMsg);
			if (hr == E_INVALIDARG)
			{
				cerr << "Fehler: Auktionsnummer nicht vorhanden" << endl;
			}
		}
	}
	else if (args[0] == L"listauctions")
	{
		if (args.size() == 1)
		{
			
		}
		else if (args.size() > 2)
		{
			cerr << "Zu viele Parameter." << endl;
			cerr << "listauctions [-a][-A] [<Artikelnameteil]" << endl;
			return;
		}
		else
		{

		}
	}
	else if (args[0] == L"bid")
	{

	}
	else if (args[0] == L"details")
	{

	}
	else if (args[0] == L"endauction")
	{

	}
	else if (args[0] == L"help")
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
	}

}
