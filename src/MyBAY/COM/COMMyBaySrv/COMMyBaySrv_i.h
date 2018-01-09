

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Jan 08 10:32:48 2018
 */
/* Compiler settings for COMMyBaySrv.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __COMMyBaySrv_i_h__
#define __COMMyBaySrv_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICOMMyBay_FWD_DEFINED__
#define __ICOMMyBay_FWD_DEFINED__
typedef interface ICOMMyBay ICOMMyBay;

#endif 	/* __ICOMMyBay_FWD_DEFINED__ */


#ifndef __COMMyBay_FWD_DEFINED__
#define __COMMyBay_FWD_DEFINED__

#ifdef __cplusplus
typedef class COMMyBay COMMyBay;
#else
typedef struct COMMyBay COMMyBay;
#endif /* __cplusplus */

#endif 	/* __COMMyBay_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_COMMyBaySrv_0000_0000 */
/* [local] */ 

typedef SAFEARRAY * SAVEARRAY_VAR;



extern RPC_IF_HANDLE __MIDL_itf_COMMyBaySrv_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_COMMyBaySrv_0000_0000_v0_0_s_ifspec;

#ifndef __ICOMMyBay_INTERFACE_DEFINED__
#define __ICOMMyBay_INTERFACE_DEFINED__

/* interface ICOMMyBay */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICOMMyBay;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("148D7498-389E-476E-8A40-311800A25CE4")
    ICOMMyBay : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE login( 
            /* [in] */ BSTR username,
            /* [in] */ BSTR password,
            /* [out] */ ULONG *sessionId,
            /* [out] */ BSTR *errMsg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE logout( 
            /* [in] */ ULONG sessionId,
            /* [out] */ BSTR *errMsg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE offer( 
            /* [in] */ ULONG sessionId,
            /* [in] */ BSTR articleName,
            /* [in] */ DOUBLE startBid,
            /* [out] */ ULONG *auctionNumber,
            /* [out] */ BSTR *errMsg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE interested( 
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [out] */ BSTR *errMsg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getAuctions( 
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG flags,
            /* [in] */ BSTR articleName,
            /* [out] */ ULONG *countAuctions,
            /* [out] */ SAVEARRAY_VAR *auctions,
            /* [out] */ BSTR *errMsg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE bid( 
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [in] */ DOUBLE bidVal,
            /* [out] */ BSTR *errMsg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE details( 
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [out] */ SAVEARRAY_VAR *allBids,
            /* [out] */ ULONG *countBids,
            /* [out] */ BSTR *errMsg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE endauction( 
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [out] */ BSTR *errMsg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getMessage( 
            /* [in] */ ULONG sessionId,
            /* [out] */ BOOL *einBoolwasIchNichtWei�,
            /* [out] */ ULONG *messageType,
            /* [out] */ SAVEARRAY_VAR *message,
            /* [out] */ BSTR *errMsg) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICOMMyBayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICOMMyBay * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICOMMyBay * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICOMMyBay * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICOMMyBay * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICOMMyBay * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICOMMyBay * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICOMMyBay * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *login )( 
            ICOMMyBay * This,
            /* [in] */ BSTR username,
            /* [in] */ BSTR password,
            /* [out] */ ULONG *sessionId,
            /* [out] */ BSTR *errMsg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *logout )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [out] */ BSTR *errMsg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *offer )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ BSTR articleName,
            /* [in] */ DOUBLE startBid,
            /* [out] */ ULONG *auctionNumber,
            /* [out] */ BSTR *errMsg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *interested )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [out] */ BSTR *errMsg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getAuctions )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG flags,
            /* [in] */ BSTR articleName,
            /* [out] */ ULONG *countAuctions,
            /* [out] */ SAVEARRAY_VAR *auctions,
            /* [out] */ BSTR *errMsg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *bid )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [in] */ DOUBLE bidVal,
            /* [out] */ BSTR *errMsg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *details )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [out] */ SAVEARRAY_VAR *allBids,
            /* [out] */ ULONG *countBids,
            /* [out] */ BSTR *errMsg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *endauction )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [out] */ BSTR *errMsg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getMessage )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [out] */ BOOL *einBoolwasIchNichtWei�,
            /* [out] */ ULONG *messageType,
            /* [out] */ SAVEARRAY_VAR *message,
            /* [out] */ BSTR *errMsg);
        
        END_INTERFACE
    } ICOMMyBayVtbl;

    interface ICOMMyBay
    {
        CONST_VTBL struct ICOMMyBayVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICOMMyBay_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICOMMyBay_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICOMMyBay_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICOMMyBay_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICOMMyBay_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICOMMyBay_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICOMMyBay_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICOMMyBay_login(This,username,password,sessionId,errMsg)	\
    ( (This)->lpVtbl -> login(This,username,password,sessionId,errMsg) ) 

#define ICOMMyBay_logout(This,sessionId,errMsg)	\
    ( (This)->lpVtbl -> logout(This,sessionId,errMsg) ) 

#define ICOMMyBay_offer(This,sessionId,articleName,startBid,auctionNumber,errMsg)	\
    ( (This)->lpVtbl -> offer(This,sessionId,articleName,startBid,auctionNumber,errMsg) ) 

#define ICOMMyBay_interested(This,sessionId,auctionNumber,errMsg)	\
    ( (This)->lpVtbl -> interested(This,sessionId,auctionNumber,errMsg) ) 

#define ICOMMyBay_getAuctions(This,sessionId,flags,articleName,countAuctions,auctions,errMsg)	\
    ( (This)->lpVtbl -> getAuctions(This,sessionId,flags,articleName,countAuctions,auctions,errMsg) ) 

#define ICOMMyBay_bid(This,sessionId,auctionNumber,bidVal,errMsg)	\
    ( (This)->lpVtbl -> bid(This,sessionId,auctionNumber,bidVal,errMsg) ) 

#define ICOMMyBay_details(This,sessionId,auctionNumber,allBids,countBids,errMsg)	\
    ( (This)->lpVtbl -> details(This,sessionId,auctionNumber,allBids,countBids,errMsg) ) 

#define ICOMMyBay_endauction(This,sessionId,auctionNumber,errMsg)	\
    ( (This)->lpVtbl -> endauction(This,sessionId,auctionNumber,errMsg) ) 

#define ICOMMyBay_getMessage(This,sessionId,einBoolwasIchNichtWei�,messageType,message,errMsg)	\
    ( (This)->lpVtbl -> getMessage(This,sessionId,einBoolwasIchNichtWei�,messageType,message,errMsg) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICOMMyBay_INTERFACE_DEFINED__ */



#ifndef __COMMyBaySrvLib_LIBRARY_DEFINED__
#define __COMMyBaySrvLib_LIBRARY_DEFINED__

/* library COMMyBaySrvLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_COMMyBaySrvLib;

EXTERN_C const CLSID CLSID_COMMyBay;

#ifdef __cplusplus

class DECLSPEC_UUID("BCD466A4-0DB3-468E-9C14-AF36AD44C119")
COMMyBay;
#endif
#endif /* __COMMyBaySrvLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

