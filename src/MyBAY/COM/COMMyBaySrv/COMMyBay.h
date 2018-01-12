// COMMyBay.h: Deklaration von CCOMMyBay

#pragma once
#include "resource.h"       // Hauptsymbole



#include "COMMyBaySrv_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Singlethread-COM-Objekte werden auf der Windows CE-Plattform nicht vollständig unterstützt. Windows Mobile-Plattformen bieten beispielsweise keine vollständige DCOM-Unterstützung. Definieren Sie _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA, um ATL zu zwingen, die Erstellung von Singlethread-COM-Objekten zu unterstützen und die Verwendung eigener Singlethread-COM-Objektimplementierungen zu erlauben. Das Threadmodell in der RGS-Datei wurde auf 'Free' festgelegt, da dies das einzige Threadmodell ist, das auf Windows CE-Plattformen ohne DCOM unterstützt wird."
#endif

using namespace ATL;


// CCOMMyBay

class ATL_NO_VTABLE CCOMMyBay :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCOMMyBay, &CLSID_COMMyBay>,
	public IDispatchImpl<ICOMMyBay, &IID_ICOMMyBay, &LIBID_COMMyBaySrvLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CCOMMyBay()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_COMMYBAY)


BEGIN_COM_MAP(CCOMMyBay)
	COM_INTERFACE_ENTRY(ICOMMyBay)
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

public:
	STDMETHOD(login)(BSTR username, BSTR password, ULONG* sessionId);
	STDMETHOD(logout)(ULONG sessionId);
	STDMETHOD(offer)(ULONG sessionId, BSTR articleName, DOUBLE startBid, ULONG* auctionNumber);
	STDMETHOD(interested)(ULONG sessionId, ULONG auctionNumber);
	STDMETHOD(getAuctions)(ULONG sessionId, ULONG flags, BSTR articleName, ULONG * countAuctions, SAFEARRAY_VAR * auctions);
	STDMETHOD(bid)(ULONG sessionId, ULONG auctionNumber, DOUBLE bidVal);
	STDMETHOD(details)(ULONG sessionId, ULONG auctionNumber, SAFEARRAY_VAR * allBids, ULONG * countBids);
	STDMETHOD(endauction)(ULONG sessionId, ULONG auctionNumber);
	STDMETHOD(getMessage)(ULONG sessionId, BOOL * messageAvailable, ULONG * messageType, SAFEARRAY_VAR * message);
};

OBJECT_ENTRY_AUTO(__uuidof(COMMyBay), CCOMMyBay)


