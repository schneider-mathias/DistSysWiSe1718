// COMMyBaySrv.cpp : Implementierung von DLL-Exporten.


#include "stdafx.h"
#include "resource.h"
#include "COMMyBaySrv_i.h"
#include "dllmain.h"


using namespace ATL;

// Wird verwendet, um festzustellen, ob die DLL von OLE entladen werden kann.
STDAPI DllCanUnloadNow(void)
{
			return _AtlModule.DllCanUnloadNow();
	}

// Gibt eine Klassenfactory zur�ck, um ein Objekt vom angeforderten Typ zu erstellen.
_Check_return_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
		return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - F�gt der Systemregistrierung Eintr�ge hinzu.
STDAPI DllRegisterServer(void)
{
	// Registriert Objekt, Typelib und alle Schnittstellen in Typelib.
	HRESULT hr = _AtlModule.DllRegisterServer();
		return hr;
}

// DllUnregisterServer - Entfernt Eintr�ge aus der Systemregistrierung.
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
		return hr;
}

// DllInstall - F�gt der Systemregistrierung pro Benutzer pro Computer Eintr�ge hinzu oder entfernt sie.
STDAPI DllInstall(BOOL bInstall, _In_opt_  LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;
	static const wchar_t szUserSwitch[] = L"user";

	if (pszCmdLine != NULL)
	{
		if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
		{
			ATL::AtlSetPerUserRegistration(true);
		}
	}

	if (bInstall)
	{	
		hr = DllRegisterServer();
		if (FAILED(hr))
		{
			DllUnregisterServer();
		}
	}
	else
	{
		hr = DllUnregisterServer();
	}

	return hr;
}


