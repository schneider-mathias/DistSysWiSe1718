

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Jan 09 10:04:27 2018
 */
/* Compiler settings for MyBay_i.idl:
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


#ifndef __MyBay_i_h_h__
#define __MyBay_i_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __MyBay_i_INTERFACE_DEFINED__
#define __MyBay_i_INTERFACE_DEFINED__

/* interface MyBay_i */
/* [implicit_handle][unique][version][uuid] */ 

typedef struct String_t
    {
    unsigned long size;
    unsigned long len;
    /* [length_is][size_is] */ unsigned char *str;
    } 	String_t;

typedef struct MyBayAuctionBidder_t
    {
    struct String_t userName;
    double bid;
    } 	MyBayAuctionBidder_t;

typedef struct MyBayAuction_t
    {
    unsigned long auctionNumber;
    struct String_t articleName;
    double startBid;
    double highestBid;
    struct String_t highestBidder;
    struct MyBayAuctionBidder_t bidder;
    struct String_t interestedUsers;
    int numInterestedUsers;
    int numBidder;
    } 	MyBayAuction_t;

error_status_t login( 
    /* [string][in] */ unsigned char *username,
    /* [string][in] */ unsigned char *password,
    /* [out] */ unsigned long *sessionId);

error_status_t logout( 
    /* [in] */ unsigned long sessionId);

error_status_t offer( 
    /* [in] */ unsigned long sessionId,
    /* [string][in] */ unsigned char *articleName,
    /* [in] */ double startBid,
    /* [out] */ unsigned long *auctionNumber);

error_status_t interested( 
    /* [in] */ unsigned long sessionId,
    /* [in] */ unsigned long auctionNumber);

error_status_t getAuctions( 
    /* [in] */ unsigned long sessionId,
    /* [in] */ unsigned long flags,
    /* [string][in] */ unsigned char *articleName,
    /* [out] */ unsigned long *countAuctions,
    /* [out] */ String_t *auctions);

error_status_t bid( 
    /* [in] */ unsigned long sessionId,
    /* [in] */ unsigned long auctionNumber,
    /* [in] */ double bidVal);

error_status_t details( 
    /* [in] */ unsigned long sessionId,
    /* [in] */ unsigned long auctionNumber,
    /* [out] */ String_t *allBids,
    /* [out] */ unsigned long *countBids);

error_status_t endauction( 
    /* [in] */ unsigned long sessionId,
    /* [in] */ unsigned long auctionNumber);

error_status_t getMessage( 
    /* [in] */ unsigned long sessionId,
    /* [out] */ boolean *messageAvailable,
    /* [out] */ unsigned long *messageType,
    /* [out] */ String_t *message);


extern handle_t hMyBay_i;


extern RPC_IF_HANDLE MyBay_i_v1_0_c_ifspec;
extern RPC_IF_HANDLE MyBay_i_v1_0_s_ifspec;
#endif /* __MyBay_i_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


