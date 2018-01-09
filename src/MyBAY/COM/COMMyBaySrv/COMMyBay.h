// COMMyCasino.h : Declaration of the CCOMMyCasino

#pragma once
#include "resource.h"       // main symbols
#include "AuthServiceTemplate.h"
#include "MyBayUser.h"
#include <vector>

#include "COMMyBaySrv_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// Struktur für Auktion
struct bidder
{
	std::wstring userName;
	double bid;
};

struct auction
{
	unsigned long auctionNumber;
	std::wstring articleName;
	double startBid;
	double highestBid;
	std::wstring highestBidder;
	std::vector<bidder> BidderList;
	std::vector<std::wstring> interestedUserList;
	int auctionStatus;							// 0 = offen, 1 = kurz vor Abschluss, 2 = Beendet
};

std::map<std::wstring, unsigned long> users;
std::vector<auction> AuctionList;
std::vector<std::vector<std::wstring>> Messages;


using namespace ATL;


// COMMyBay

class ATL_NO_VTABLE CCOMMyBay :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCOMMyBay, &CLSID_COMMyBay>,
	public IDispatchImpl<ICOMMyBay, &IID_ICOMMyBay, &LIBID_COMMyBaySrvLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CCOMMyBay();

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

private:
	CAuthServiceTemplate<MyBayUser> m_AuthService;
	auction* auctionList;
	static ULONG num_auct; //Anzahl Auktionen

public:
	STDMETHOD(login)(BSTR username, BSTR password, ULONG* sessionId, BSTR* errMsg);
	STDMETHOD(logout)(ULONG sessionId, BSTR* errMsg);
	STDMETHOD(offer)(ULONG sessionId, BSTR articleName, DOUBLE startBid, ULONG* auctionNumber, BSTR* errMsg);
	STDMETHOD(interested)(ULONG sessionId, ULONG auctionNumber, BSTR * errMsg);
	STDMETHOD(getAuctions)(ULONG sessionId, ULONG flags, BSTR articleName, ULONG * countAuctions, SAVEARRAY_VAR * auctions, BSTR * errMsg);
	STDMETHOD(bid)(ULONG sessionId, ULONG auctionNumber, DOUBLE bidVal, BSTR * errMsg);
	STDMETHOD(details)(ULONG sessionId, ULONG auctionNumber, SAVEARRAY_VAR * allBids, ULONG * countBids, BSTR * errMsg);
	STDMETHOD(endauction)(ULONG sessionId, ULONG auctionNumber, BSTR * errMsg);
	STDMETHOD(getMessage)(ULONG sessionId, BOOL * einBoolwasIchNichtWeiß, ULONG * messageType, SAVEARRAY_VAR * message, BSTR * errMsg);
};

OBJECT_ENTRY_AUTO(__uuidof(COMMyBay), CCOMMyBay)


using namespace std;
// Prüfe ob User bereits eingeloggt ist
BOOL loginCheck(unsigned char username)
{
	if (users.find(to_wstring(username)) == users.end())
	{
		return FALSE;
	}
	else
		return TRUE;
}

// Usernamen zur zugehörigen SessionId ermitteln
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