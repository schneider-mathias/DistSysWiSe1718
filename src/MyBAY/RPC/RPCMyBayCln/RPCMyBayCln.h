/************************************************************/
/*                                                          */
/* Inhalt:    RPCMyBayCln - Header				            */
/*                                                          */
/* Autor:	  Johannes Sauer		                        */
/* Stand:     23. Jan 2018                                  */
/*															*/
/************************************************************/

#pragma once
#include <vector>
#include <chrono>
#include <windows.h>
#include "MyBayDefines.h"
#include "RpcException.h"


using namespace std;

static void Bind(char* remoteNetwAddr);
static void UnBind(void);
static void rpcCalls(void);
void readConsole();
void interpretCommand(unsigned long *sessionID, std::vector<std::wstring> args, boolean *threadAllow);
HRESULT executeGetMessage(unsigned long *sessionID, boolean* messageAvailable, unsigned long* messageType, String_t *message);

std::vector<std::wstring> args;					// Eingabeargument
std::vector<unsigned long> MyOwnAuctions;		// Liste meiner eigenen Auktionen (Nummer)

/// <summary>
/// Deserialisiert einen char*, dessen Wörter mit Leerzeichen getrennt sind
/// </summary>
/// <param name="inStr"> Input String der serialisert werden soll </param>
/// <param name="len"> Länge des Input String der serialisert werden soll </param>
/// <returns> Vektor mit allen Nachrichtenteilen </returns>
std::vector<wstring> deserialize(unsigned char *inStr, unsigned long len)
{
	vector<wstring> desStr;
	wstring tempStr;
	for (int i = 0; i < len; i++)
	{
		// solange kein Platzhalter kommt, wird das Wort zusammengesetzt
		if (inStr[i] != L' ')
		{
			tempStr.push_back(inStr[i]);
		}
		// Leerzeichen wurde erkannt und somit wird neues Wort der Liste hinzugefügt
		else
		{
			desStr.push_back(tempStr);
			tempStr.clear();			
		}
	}
	return desStr;
}

/// <summary>
/// Ausgabe der empfangenen Nachrichten
/// </summary>
/// <param name="messageVec"> Vector aus allen Nachrichtenteilen </param>
/// <param name="messageType"> Type der Nachricht </param>
void printMessage(vector<wstring> messageVec, unsigned long messageType)
{
	wcout << endl;
	if (messageType == 0)
	{
		wcout << "------------------------------------- Neues Gebot --------------------------------------" << endl;
		wcout << "Artikelname: " << messageVec.at(1) << endl;
		if (messageVec.at(1) != L"")
			wcout << "Bieter: " << messageVec.at(0) << endl;
		wcout << "Gebot: " << messageVec.at(2).substr(0, messageVec.at(2).size() - 4) << endl;
		wcout << "Auktionsstatus: ";
		if (messageVec.at(3) == L"0")
			wcout << "offen" << endl;
		if (messageVec.at(3) == L"1")
			wcout << "kurz vor Ende" << endl;
		if (messageVec.at(3) == L"2")
			wcout << "Beendet" << endl;
		wcout << "----------------------------------------------------------------------------------------" << endl;
	}
	else if (messageType == 1)
	{
		wcout << "------------------------------------- Auktion endet bald -------------------------------" << endl;
		wcout << "Die Auktion " << messageVec.at(2) << " endet bald." << endl;
		wcout << "Artikelname: " << messageVec.at(1) << endl;
		wcout << "Hoechstgebot: " << messageVec.at(3).substr(0, messageVec.at(3).size() - 4) << endl;
		wcout << "Nr. Warnung: " << messageVec.at(0) << endl;
		wcout << "----------------------------------------------------------------------------------------" << endl;
	}
	else if (messageType == 2)
	{
		wcout << "------------------------------------- Auktion beendet ----------------------------------" << endl;
		wcout << "Artikel: " << messageVec.at(1) << endl;
		wcout << "Kaeufer: " << messageVec.at(0) << endl;
		wcout << "Preis: " << messageVec.at(2).substr(0, messageVec.at(2).size() - 4) << endl;
		wcout << "Auktionsstatus: Beendet" << endl;
		wcout << "----------------------------------------------------------------------------------------" << endl;
	}
	else
	{

	}
	wcout << endl;
}

/// <summary>
/// MessagesThread: Pullt alle neuen Nachrichten, die auf dem Server für den Client bereitliegen und gibt diese auf der Console aus
/// </summary>
/// <param name="sessionID"> SessionId des Clients </param>
/// <param name="threadAllow"> Erlaubnis zur Ausführung für den Thread </param>
void pullMessages(unsigned long *sessionID, boolean *threadAllow)
{
	boolean messageAvailable = TRUE;
	unsigned long messageType = 0;
	String_t message = { 0,0,NULL };
	while (*threadAllow == TRUE)
	{
		// User ist eingeloggt
		if (*sessionID != 0)
		{
			HRESULT hr;
			// Pull solange Nachrichten verfügbar
			do
			{
				Sleep(1000);
				try
				{
					try
					{
						// Nachricht am Server für den Client abholen
						hr = executeGetMessage(sessionID,&messageAvailable, &messageType, &message);
					}
					catch (...)
					{
						throw;
					}
				}
				catch (RpcException& e)
				{
					if (e.GetStatus() == RPC_S_SERVER_UNAVAILABLE)
					{
						cerr << "Fehler: Der Server antwortet nicht!\nClient beenden." << endl;
						getchar();
						std::terminate();
					}
					else
					{
						
					}
				}					
				if (hr == RPC_S_OK && message.str != NULL)
				{
					// Nachricht deserialisieren
					vector<wstring> messageVec = deserialize(message.str, message.len);
					// Ausgabe der Nachricht, abhängig vom Nachrichtentyp
					printMessage(messageVec, messageType);
				}					
			} while (messageAvailable == TRUE);
		}
	}
};

/// <summary>
/// Nachrichten für den Client vom Server abholen
/// </summary>
/// <param name="sessionID"> SessionId des Clients </param>
/// <param name="messageAvailable"> Zeigt an, noch weitere Nachrichten für den Client am Server bereit stehen </param>
/// <param name="messageType"> Typ der Nachricht </param>
/// <param name="message"> Nachricht für den Client </param>
/// <returns> HRESULT </returns>
HRESULT executeGetMessage(unsigned long *sessionID, boolean* messageAvailable, unsigned long* messageType, String_t *message)
{
	HRESULT hr;
	RpcTryExcept
	{
		// neue Nachricht für Client abholen
		hr = getMessage(*sessionID, messageAvailable, messageType, message);
		return hr;
	}
		RpcExcept(1)
	{
		RpcException::Raise(RpcExceptionCode(), "RPC call failed", "Remote Procedure Call");
	}
	RpcEndExcept
}