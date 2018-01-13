

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sat Jan 13 18:12:04 2018
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

typedef SAFEARRAY * SAFEARRAY_VAR;



extern RPC_IF_HANDLE __MIDL_itf_COMMyBaySrv_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_COMMyBaySrv_0000_0000_v0_0_s_ifspec;

#ifndef __ICOMMyBay_INTERFACE_DEFINED__
#define __ICOMMyBay_INTERFACE_DEFINED__

/* interface ICOMMyBay */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICOMMyBay;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C3E6334F-3574-4001-8DBE-1B549B3877D0")
    ICOMMyBay : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE login( 
            /* [in] */ BSTR username,
            /* [in] */ BSTR password,
            /* [out] */ ULONG *sessionId) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE logout( 
            /* [in] */ ULONG sessionId) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE offer( 
            /* [in] */ ULONG sessionId,
            /* [in] */ BSTR articleName,
            /* [in] */ DOUBLE startBid,
            /* [out] */ ULONG *auctionNumber) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE interested( 
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getAuctions( 
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG flags,
            /* [in] */ BSTR articleName,
            /* [out] */ ULONG *countAuctions,
            /* [out] */ SAFEARRAY_VAR *auctions) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE bid( 
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [in] */ DOUBLE bidValue) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE details( 
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [out] */ SAFEARRAY_VAR *allBids,
            /* [out] */ ULONG *countBids) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE endauction( 
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getMessage( 
            /* [in] */ ULONG sessionId,
            /* [out] */ BOOL *messageAvailable,
            /* [out] */ ULONG *messageType,
            /* [out] */ SAFEARRAY_VAR *message) = 0;
        
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
            /* [out] */ ULONG *sessionId);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *logout )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *offer )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ BSTR articleName,
            /* [in] */ DOUBLE startBid,
            /* [out] */ ULONG *auctionNumber);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *interested )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getAuctions )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG flags,
            /* [in] */ BSTR articleName,
            /* [out] */ ULONG *countAuctions,
            /* [out] */ SAFEARRAY_VAR *auctions);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *bid )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [in] */ DOUBLE bidValue);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *details )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber,
            /* [out] */ SAFEARRAY_VAR *allBids,
            /* [out] */ ULONG *countBids);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *endauction )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [in] */ ULONG auctionNumber);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getMessage )( 
            ICOMMyBay * This,
            /* [in] */ ULONG sessionId,
            /* [out] */ BOOL *messageAvailable,
            /* [out] */ ULONG *messageType,
            /* [out] */ SAFEARRAY_VAR *message);
        
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


#define ICOMMyBay_login(This,username,password,sessionId)	\
    ( (This)->lpVtbl -> login(This,username,password,sessionId) ) 

#define ICOMMyBay_logout(This,sessionId)	\
    ( (This)->lpVtbl -> logout(This,sessionId) ) 

#define ICOMMyBay_offer(This,sessionId,articleName,startBid,auctionNumber)	\
    ( (This)->lpVtbl -> offer(This,sessionId,articleName,startBid,auctionNumber) ) 

#define ICOMMyBay_interested(This,sessionId,auctionNumber)	\
    ( (This)->lpVtbl -> interested(This,sessionId,auctionNumber) ) 

#define ICOMMyBay_getAuctions(This,sessionId,flags,articleName,countAuctions,auctions)	\
    ( (This)->lpVtbl -> getAuctions(This,sessionId,flags,articleName,countAuctions,auctions) ) 

#define ICOMMyBay_bid(This,sessionId,auctionNumber,bidValue)	\
    ( (This)->lpVtbl -> bid(This,sessionId,auctionNumber,bidValue) ) 

#define ICOMMyBay_details(This,sessionId,auctionNumber,allBids,countBids)	\
    ( (This)->lpVtbl -> details(This,sessionId,auctionNumber,allBids,countBids) ) 

#define ICOMMyBay_endauction(This,sessionId,auctionNumber)	\
    ( (This)->lpVtbl -> endauction(This,sessionId,auctionNumber) ) 

#define ICOMMyBay_getMessage(This,sessionId,messageAvailable,messageType,message)	\
    ( (This)->lpVtbl -> getMessage(This,sessionId,messageAvailable,messageType,message) ) 

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

class DECLSPEC_UUID("E591CA8E-1693-4BD0-9A65-919D333389AA")
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


