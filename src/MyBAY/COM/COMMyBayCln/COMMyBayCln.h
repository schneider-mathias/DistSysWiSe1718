/************************************************************/
/*                                                          */
/* Inhalt:    COMMyBayCln - Header				            */
/*                                                          */
/* Autor:	  Johannes Sauer		                        */
/* Stand:     23. Jan 2018                                  */
/*															*/
/************************************************************/

#pragma once
#include <vector>
#include <chrono>
#include <windows.h>
#include <iostream>
#include "MyBayDefines.h"
#import "COMMyBaySrv.dll"
#include "../COMMyBaySrv/COMMyBaySrv_i.c"
#include "../COMMyBaySrv/COMMyBaySrv_i.h"

using namespace std;

//static void Bind(void);
//static void UnBind(void);
//static void rpcCalls(void);
void readConsole(ICOMMyBay *p_ICOMMyBaySrv);
void interpretCommand(ICOMMyBay *p_ICOMMyBaySrv, unsigned long *sessionID, std::vector<std::wstring> args, boolean *threadAllow);

std::vector<std::wstring> args;					// Eingabeargumente

// Deserialisiert einen char*, dessen Wörter mit Leerzeichen getrennt sind
std::vector<wstring> deserialize(unsigned char *inStr, unsigned long len)
{
	vector<wstring> desStr;
	wstring tempStr;
	for (int i = 0; i < len; i++)
	{
		// solange kein Platzhalter kommt, wird das Wort zusammengesetzt
		if (inStr[i] != L' ')
		{
			//wstring s((char)inStr[i]);
			tempStr.push_back(inStr[i]);
			//tempStr += s(char_to_wstring((const char*)inStr[i]));
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

//// Deserialisiert einen String, dessen Wörter mit einem Platzhalter getrennt sind
//std::vector<wstring> deserialize(unsigned char *inStr, unsigned long len)
//{
//	vector<wstring> desStr;
//	wstring tempStr;
//	for (int i = 0; i < len; i++)
//	{
//		// solange kein Platzhalter kommt, wird das Wort zusammengesetzt
//		if (inStr[i] != (unsigned char)PLACEHOLDER_FOR_SERIALISATION_DESERIALISATION)
//		{
//			//wstring s((char)inStr[i]);
//			tempStr.push_back(inStr[i]);
//			//tempStr += s(char_to_wstring((const char*)inStr[i]));
//		}
//		// Leerzeichen wurde erkannt und somit wird neues Wort der Liste hinzugefügt
//		else
//		{
//			desStr.push_back(tempStr);
//			tempStr.clear();
//		}
//	}
//	return desStr;
//}


// Ausgabe der empfangenen Nachrichten
void printMessage(vector<wstring> messageVec, unsigned long messageType)
{
	wcout << endl;
	if (messageType == 0)
	{
		wcout << "------------------------------------- Neues Gebot --------------------------------------" << endl;
		wcout << "Artikelname: " << messageVec.at(2) << endl;
		if (messageVec.at(1) != L"")
			wcout << "Bieter: " << messageVec.at(1) << endl;
		wcout << "Gebot: " << messageVec.at(3) << endl;
		wcout << "Auktionsstatus: ";
		if (messageVec.at(4) == L"0")
			wcout << "offen" << endl;
		if (messageVec.at(4) == L"1")
			wcout << "kurz vor Ende" << endl;
		if (messageVec.at(4) == L"2")
			wcout << "Beendet" << endl;
		wcout << "----------------------------------------------------------------------------------------" << endl;
	}
	else if (messageType == 1)
	{
		wcout << "------------------------------------- Auktion endet bald --------------------------------------" << endl;
		wcout << "Die Auktion " << messageVec.at(3) << " endet bald." << endl;
		wcout << "Artikelname: " << messageVec.at(2) << endl;
		wcout << "Hoechstgebot: " << messageVec.at(4) << endl;
		wcout << "Nr. Warnung: " << messageVec.at(1) << endl;
		wcout << "----------------------------------------------------------------------------------------" << endl;
	}
	else if (messageType == 2)
	{
		wcout << "------------------------------------- Auktion beendet --------------------------------------" << endl;
		wcout << "Artikel: " << messageVec.at(2) << endl;
		wcout << "Kaeufer: " << messageVec.at(1) << endl;
		wcout << "Preis: " << messageVec.at(3) << endl;
		wcout << "Auktionsstatus: ";
		if (messageVec.at(4) == L"0")
			wcout << "offen" << endl;
		if (messageVec.at(4) == L"1")
			wcout << "kurz vor Ende" << endl;
		if (messageVec.at(4) == L"2")
			wcout << "Beendet" << endl;
		wcout << "----------------------------------------------------------------------------------------" << endl;
	}
	else if (messageType == 3)
	{
		wcout << "--------------------- Information --------------------- " << endl;
		for (std::vector<wstring>::iterator it = messageVec.begin(); it != messageVec.end(); it++)
		{
			wcout << (*it) << " ";
		}
		wcout << endl;
	}
	else
	{

	}
	wcout << endl;
}

// Funktion wird von Thread MessagesThread ausgeführt
// Pullt alle neuen Nachrichten, die auf dem Server für den Client bereitliegen und gibt diese auf der Console aus
void pullMessages(ICOMMyBay *p_ICOMMyBaySrv, unsigned long *sessionID, boolean *threadAllow)
{
	BOOL messageAvailable = TRUE;
	unsigned long messageType = 0;
	SAFEARRAY_VAR message;

	while (*threadAllow == TRUE)
	{
		Sleep(1000);	// millisekundne
						//std::this_thread::sleep_for(1s);
						// User ist eingeloggt
		if (*sessionID != 0)
		{
			HRESULT hr;
			// Pull solange Nachrichten verfügbar
			do
			{
				// neue Nachricht für Client abholen
				hr = p_ICOMMyBaySrv->getMessage(*sessionID, &messageAvailable, &messageType, &message);
				if (hr == S_OK)
				{
					CComSafeArray<VARIANT> newMessage(message);
					vector<wstring> messageVec;
					// Alle Elemente im SAFEARRAY werden in einen Vector übertragen
					for (int i = 0; i < newMessage.GetCount(); i++)
					{
						messageVec.push_back(bstr_to_wstr(*newMessage[i].pbstrVal));
					}
					if (messageVec.size() > 0)
					{
						// Ausgabe der Nachricht, abhängig vom Nachrichtentyp
						printMessage(messageVec, messageType);
					}
					
				}
			} while (messageAvailable == TRUE);
		}
	}
	//std::terminate();
}
