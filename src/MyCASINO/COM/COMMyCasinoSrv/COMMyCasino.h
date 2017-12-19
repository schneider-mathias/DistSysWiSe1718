// COMMyCasino.h : Declaration of the CCOMMyCasino

#pragma once
#include "resource.h"       // main symbols
#include "AuthService.h"


#include "COMMyCasinoSrv_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CCOMMyCasino

class ATL_NO_VTABLE CCOMMyCasino :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCOMMyCasino, &CLSID_COMMyCasino>,
	public IDispatchImpl<ICOMMyCasino, &IID_ICOMMyCasino, &LIBID_COMMyCasinoSrvLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CCOMMyCasino();

DECLARE_REGISTRY_RESOURCEID(IDR_COMMYCASINO)


BEGIN_COM_MAP(CCOMMyCasino)
	COM_INTERFACE_ENTRY(ICOMMyCasino)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

private:
	CAuthService m_AuthService;

public:



	STDMETHOD(login)(BSTR username, BSTR password, ULONG* sessionId, BSTR* errMsg);
	STDMETHOD(logout)(ULONG sessionId, BSTR* errMsg);
};

OBJECT_ENTRY_AUTO(__uuidof(COMMyCasino), CCOMMyCasino)
