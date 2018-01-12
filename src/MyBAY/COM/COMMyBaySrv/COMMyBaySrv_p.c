

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Jan 12 22:40:54 2018
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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "COMMyBaySrv_i.h"

#define TYPE_FORMAT_STRING_SIZE   1069                              
#define PROC_FORMAT_STRING_SIZE   439                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _COMMyBaySrv_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } COMMyBaySrv_MIDL_TYPE_FORMAT_STRING;

typedef struct _COMMyBaySrv_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } COMMyBaySrv_MIDL_PROC_FORMAT_STRING;

typedef struct _COMMyBaySrv_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } COMMyBaySrv_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const COMMyBaySrv_MIDL_TYPE_FORMAT_STRING COMMyBaySrv__MIDL_TypeFormatString;
extern const COMMyBaySrv_MIDL_PROC_FORMAT_STRING COMMyBaySrv__MIDL_ProcFormatString;
extern const COMMyBaySrv_MIDL_EXPR_FORMAT_STRING COMMyBaySrv__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ICOMMyBay_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ICOMMyBay_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const COMMyBaySrv_MIDL_PROC_FORMAT_STRING COMMyBaySrv__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure login */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 16 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x1 ),	/* 1 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter username */

/* 24 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter password */

/* 30 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter sessionId */

/* 36 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 42 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 44 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 46 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure logout */

/* 48 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 50 */	NdrFcLong( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x8 ),	/* 8 */
/* 56 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 58 */	NdrFcShort( 0x8 ),	/* 8 */
/* 60 */	NdrFcShort( 0x8 ),	/* 8 */
/* 62 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 64 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 66 */	NdrFcShort( 0x0 ),	/* 0 */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */
/* 70 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 72 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 74 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 76 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 80 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure offer */

/* 84 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x9 ),	/* 9 */
/* 92 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 94 */	NdrFcShort( 0x18 ),	/* 24 */
/* 96 */	NdrFcShort( 0x24 ),	/* 36 */
/* 98 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 100 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 102 */	NdrFcShort( 0x0 ),	/* 0 */
/* 104 */	NdrFcShort( 0x1 ),	/* 1 */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 108 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 110 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter articleName */

/* 114 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 116 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 118 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter startBid */

/* 120 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 122 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 124 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 126 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 128 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 132 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 134 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 136 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure interested */

/* 138 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 140 */	NdrFcLong( 0x0 ),	/* 0 */
/* 144 */	NdrFcShort( 0xa ),	/* 10 */
/* 146 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 148 */	NdrFcShort( 0x10 ),	/* 16 */
/* 150 */	NdrFcShort( 0x8 ),	/* 8 */
/* 152 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 154 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0x0 ),	/* 0 */
/* 160 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 162 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 164 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 168 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 170 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getAuctions */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0xb ),	/* 11 */
/* 188 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 190 */	NdrFcShort( 0x10 ),	/* 16 */
/* 192 */	NdrFcShort( 0x24 ),	/* 36 */
/* 194 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 196 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 198 */	NdrFcShort( 0x1 ),	/* 1 */
/* 200 */	NdrFcShort( 0x1 ),	/* 1 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 206 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter flags */

/* 210 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 212 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter articleName */

/* 216 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 218 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 220 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter countAuctions */

/* 222 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 224 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctions */

/* 228 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 230 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 232 */	NdrFcShort( 0x422 ),	/* Type Offset=1058 */

	/* Return value */

/* 234 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 236 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 238 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure bid */

/* 240 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 242 */	NdrFcLong( 0x0 ),	/* 0 */
/* 246 */	NdrFcShort( 0xc ),	/* 12 */
/* 248 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 250 */	NdrFcShort( 0x20 ),	/* 32 */
/* 252 */	NdrFcShort( 0x8 ),	/* 8 */
/* 254 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 256 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 262 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 264 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 266 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 270 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 272 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 274 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bidValue */

/* 276 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 278 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 280 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 282 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 284 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure details */

/* 288 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0xd ),	/* 13 */
/* 296 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 298 */	NdrFcShort( 0x10 ),	/* 16 */
/* 300 */	NdrFcShort( 0x24 ),	/* 36 */
/* 302 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x5,		/* 5 */
/* 304 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 306 */	NdrFcShort( 0x1 ),	/* 1 */
/* 308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 310 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 312 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 314 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 316 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 318 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 320 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter allBids */

/* 324 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 326 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 328 */	NdrFcShort( 0x422 ),	/* Type Offset=1058 */

	/* Parameter countBids */

/* 330 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 332 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 336 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 338 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 340 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure endauction */

/* 342 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 344 */	NdrFcLong( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0xe ),	/* 14 */
/* 350 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 352 */	NdrFcShort( 0x10 ),	/* 16 */
/* 354 */	NdrFcShort( 0x8 ),	/* 8 */
/* 356 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 358 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 362 */	NdrFcShort( 0x0 ),	/* 0 */
/* 364 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 366 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 368 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 372 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 374 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 376 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 378 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 380 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 382 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getMessage */

/* 384 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 386 */	NdrFcLong( 0x0 ),	/* 0 */
/* 390 */	NdrFcShort( 0xf ),	/* 15 */
/* 392 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 394 */	NdrFcShort( 0x8 ),	/* 8 */
/* 396 */	NdrFcShort( 0x40 ),	/* 64 */
/* 398 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x5,		/* 5 */
/* 400 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 402 */	NdrFcShort( 0x1 ),	/* 1 */
/* 404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 408 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 410 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 412 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter messageAvailable */

/* 414 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 416 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 418 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter messageType */

/* 420 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 422 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 424 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter message */

/* 426 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 428 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 430 */	NdrFcShort( 0x422 ),	/* Type Offset=1058 */

	/* Return value */

/* 432 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 434 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 436 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const COMMyBaySrv_MIDL_TYPE_FORMAT_STRING COMMyBaySrv__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0xe ),	/* Offset= 14 (18) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 16 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 18 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 20 */	NdrFcShort( 0x8 ),	/* 8 */
/* 22 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (6) */
/* 24 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 26 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 28 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 30 */	NdrFcShort( 0x0 ),	/* 0 */
/* 32 */	NdrFcShort( 0x4 ),	/* 4 */
/* 34 */	NdrFcShort( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0xffde ),	/* Offset= -34 (2) */
/* 38 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 40 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 42 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 44 */	NdrFcShort( 0x3f6 ),	/* Offset= 1014 (1058) */
/* 46 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 48 */	NdrFcShort( 0x2 ),	/* Offset= 2 (50) */
/* 50 */	
			0x13, 0x0,	/* FC_OP */
/* 52 */	NdrFcShort( 0x3dc ),	/* Offset= 988 (1040) */
/* 54 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 56 */	NdrFcShort( 0x18 ),	/* 24 */
/* 58 */	NdrFcShort( 0xa ),	/* 10 */
/* 60 */	NdrFcLong( 0x8 ),	/* 8 */
/* 64 */	NdrFcShort( 0x5a ),	/* Offset= 90 (154) */
/* 66 */	NdrFcLong( 0xd ),	/* 13 */
/* 70 */	NdrFcShort( 0x90 ),	/* Offset= 144 (214) */
/* 72 */	NdrFcLong( 0x9 ),	/* 9 */
/* 76 */	NdrFcShort( 0xc2 ),	/* Offset= 194 (270) */
/* 78 */	NdrFcLong( 0xc ),	/* 12 */
/* 82 */	NdrFcShort( 0x2c0 ),	/* Offset= 704 (786) */
/* 84 */	NdrFcLong( 0x24 ),	/* 36 */
/* 88 */	NdrFcShort( 0x2ea ),	/* Offset= 746 (834) */
/* 90 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 94 */	NdrFcShort( 0x306 ),	/* Offset= 774 (868) */
/* 96 */	NdrFcLong( 0x10 ),	/* 16 */
/* 100 */	NdrFcShort( 0x320 ),	/* Offset= 800 (900) */
/* 102 */	NdrFcLong( 0x2 ),	/* 2 */
/* 106 */	NdrFcShort( 0x33a ),	/* Offset= 826 (932) */
/* 108 */	NdrFcLong( 0x3 ),	/* 3 */
/* 112 */	NdrFcShort( 0x354 ),	/* Offset= 852 (964) */
/* 114 */	NdrFcLong( 0x14 ),	/* 20 */
/* 118 */	NdrFcShort( 0x36e ),	/* Offset= 878 (996) */
/* 120 */	NdrFcShort( 0xffff ),	/* Offset= -1 (119) */
/* 122 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 124 */	NdrFcShort( 0x4 ),	/* 4 */
/* 126 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 132 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 134 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 136 */	NdrFcShort( 0x4 ),	/* 4 */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */
/* 140 */	NdrFcShort( 0x1 ),	/* 1 */
/* 142 */	NdrFcShort( 0x0 ),	/* 0 */
/* 144 */	NdrFcShort( 0x0 ),	/* 0 */
/* 146 */	0x13, 0x0,	/* FC_OP */
/* 148 */	NdrFcShort( 0xff7e ),	/* Offset= -130 (18) */
/* 150 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 152 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 154 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 158 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 160 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 162 */	NdrFcShort( 0x4 ),	/* 4 */
/* 164 */	NdrFcShort( 0x4 ),	/* 4 */
/* 166 */	0x11, 0x0,	/* FC_RP */
/* 168 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (122) */
/* 170 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 172 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 174 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 176 */	NdrFcLong( 0x0 ),	/* 0 */
/* 180 */	NdrFcShort( 0x0 ),	/* 0 */
/* 182 */	NdrFcShort( 0x0 ),	/* 0 */
/* 184 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 186 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 188 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 190 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 192 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 196 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 202 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 206 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 208 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 210 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (174) */
/* 212 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 214 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 216 */	NdrFcShort( 0x8 ),	/* 8 */
/* 218 */	NdrFcShort( 0x0 ),	/* 0 */
/* 220 */	NdrFcShort( 0x6 ),	/* Offset= 6 (226) */
/* 222 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 224 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 226 */	
			0x11, 0x0,	/* FC_RP */
/* 228 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (192) */
/* 230 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 232 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 240 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 242 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 244 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 246 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 248 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */
/* 252 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 254 */	NdrFcShort( 0x0 ),	/* 0 */
/* 256 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 258 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 262 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 264 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 266 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (230) */
/* 268 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 270 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 272 */	NdrFcShort( 0x8 ),	/* 8 */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */
/* 276 */	NdrFcShort( 0x6 ),	/* Offset= 6 (282) */
/* 278 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 280 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 282 */	
			0x11, 0x0,	/* FC_RP */
/* 284 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (248) */
/* 286 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 288 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 290 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 292 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 294 */	NdrFcShort( 0x2 ),	/* Offset= 2 (296) */
/* 296 */	NdrFcShort( 0x10 ),	/* 16 */
/* 298 */	NdrFcShort( 0x2f ),	/* 47 */
/* 300 */	NdrFcLong( 0x14 ),	/* 20 */
/* 304 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 306 */	NdrFcLong( 0x3 ),	/* 3 */
/* 310 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 312 */	NdrFcLong( 0x11 ),	/* 17 */
/* 316 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 318 */	NdrFcLong( 0x2 ),	/* 2 */
/* 322 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 324 */	NdrFcLong( 0x4 ),	/* 4 */
/* 328 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 330 */	NdrFcLong( 0x5 ),	/* 5 */
/* 334 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 336 */	NdrFcLong( 0xb ),	/* 11 */
/* 340 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 342 */	NdrFcLong( 0xa ),	/* 10 */
/* 346 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 348 */	NdrFcLong( 0x6 ),	/* 6 */
/* 352 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (584) */
/* 354 */	NdrFcLong( 0x7 ),	/* 7 */
/* 358 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 360 */	NdrFcLong( 0x8 ),	/* 8 */
/* 364 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (590) */
/* 366 */	NdrFcLong( 0xd ),	/* 13 */
/* 370 */	NdrFcShort( 0xff3c ),	/* Offset= -196 (174) */
/* 372 */	NdrFcLong( 0x9 ),	/* 9 */
/* 376 */	NdrFcShort( 0xff6e ),	/* Offset= -146 (230) */
/* 378 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 382 */	NdrFcShort( 0xd4 ),	/* Offset= 212 (594) */
/* 384 */	NdrFcLong( 0x24 ),	/* 36 */
/* 388 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (602) */
/* 390 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 394 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (602) */
/* 396 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 400 */	NdrFcShort( 0x100 ),	/* Offset= 256 (656) */
/* 402 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 406 */	NdrFcShort( 0xfe ),	/* Offset= 254 (660) */
/* 408 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 412 */	NdrFcShort( 0xfc ),	/* Offset= 252 (664) */
/* 414 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 418 */	NdrFcShort( 0xfa ),	/* Offset= 250 (668) */
/* 420 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 424 */	NdrFcShort( 0xf8 ),	/* Offset= 248 (672) */
/* 426 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 430 */	NdrFcShort( 0xf6 ),	/* Offset= 246 (676) */
/* 432 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 436 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (660) */
/* 438 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 442 */	NdrFcShort( 0xde ),	/* Offset= 222 (664) */
/* 444 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 448 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (680) */
/* 450 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 454 */	NdrFcShort( 0xde ),	/* Offset= 222 (676) */
/* 456 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 460 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (684) */
/* 462 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 466 */	NdrFcShort( 0xde ),	/* Offset= 222 (688) */
/* 468 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 472 */	NdrFcShort( 0xdc ),	/* Offset= 220 (692) */
/* 474 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 478 */	NdrFcShort( 0xda ),	/* Offset= 218 (696) */
/* 480 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 484 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (708) */
/* 486 */	NdrFcLong( 0x10 ),	/* 16 */
/* 490 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 492 */	NdrFcLong( 0x12 ),	/* 18 */
/* 496 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 498 */	NdrFcLong( 0x13 ),	/* 19 */
/* 502 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 504 */	NdrFcLong( 0x15 ),	/* 21 */
/* 508 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 510 */	NdrFcLong( 0x16 ),	/* 22 */
/* 514 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 516 */	NdrFcLong( 0x17 ),	/* 23 */
/* 520 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 522 */	NdrFcLong( 0xe ),	/* 14 */
/* 526 */	NdrFcShort( 0xbe ),	/* Offset= 190 (716) */
/* 528 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 532 */	NdrFcShort( 0xc2 ),	/* Offset= 194 (726) */
/* 534 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 538 */	NdrFcShort( 0xc0 ),	/* Offset= 192 (730) */
/* 540 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 544 */	NdrFcShort( 0x74 ),	/* Offset= 116 (660) */
/* 546 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 550 */	NdrFcShort( 0x72 ),	/* Offset= 114 (664) */
/* 552 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 556 */	NdrFcShort( 0x70 ),	/* Offset= 112 (668) */
/* 558 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 562 */	NdrFcShort( 0x66 ),	/* Offset= 102 (664) */
/* 564 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 568 */	NdrFcShort( 0x60 ),	/* Offset= 96 (664) */
/* 570 */	NdrFcLong( 0x0 ),	/* 0 */
/* 574 */	NdrFcShort( 0x0 ),	/* Offset= 0 (574) */
/* 576 */	NdrFcLong( 0x1 ),	/* 1 */
/* 580 */	NdrFcShort( 0x0 ),	/* Offset= 0 (580) */
/* 582 */	NdrFcShort( 0xffff ),	/* Offset= -1 (581) */
/* 584 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 586 */	NdrFcShort( 0x8 ),	/* 8 */
/* 588 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 590 */	
			0x13, 0x0,	/* FC_OP */
/* 592 */	NdrFcShort( 0xfdc2 ),	/* Offset= -574 (18) */
/* 594 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 596 */	NdrFcShort( 0x2 ),	/* Offset= 2 (598) */
/* 598 */	
			0x13, 0x0,	/* FC_OP */
/* 600 */	NdrFcShort( 0x1b8 ),	/* Offset= 440 (1040) */
/* 602 */	
			0x13, 0x0,	/* FC_OP */
/* 604 */	NdrFcShort( 0x20 ),	/* Offset= 32 (636) */
/* 606 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 608 */	NdrFcLong( 0x2f ),	/* 47 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0x0 ),	/* 0 */
/* 616 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 618 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 620 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 622 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 624 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 626 */	NdrFcShort( 0x1 ),	/* 1 */
/* 628 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 630 */	NdrFcShort( 0x4 ),	/* 4 */
/* 632 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 634 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 636 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 638 */	NdrFcShort( 0x10 ),	/* 16 */
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 642 */	NdrFcShort( 0xa ),	/* Offset= 10 (652) */
/* 644 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 646 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 648 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (606) */
/* 650 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 652 */	
			0x13, 0x0,	/* FC_OP */
/* 654 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (624) */
/* 656 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 658 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 660 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 662 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 664 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 666 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 668 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 670 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 672 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 674 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 676 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 678 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 680 */	
			0x13, 0x0,	/* FC_OP */
/* 682 */	NdrFcShort( 0xff9e ),	/* Offset= -98 (584) */
/* 684 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 686 */	NdrFcShort( 0xffa0 ),	/* Offset= -96 (590) */
/* 688 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 690 */	NdrFcShort( 0xfdfc ),	/* Offset= -516 (174) */
/* 692 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 694 */	NdrFcShort( 0xfe30 ),	/* Offset= -464 (230) */
/* 696 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 698 */	NdrFcShort( 0x2 ),	/* Offset= 2 (700) */
/* 700 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 702 */	NdrFcShort( 0x2 ),	/* Offset= 2 (704) */
/* 704 */	
			0x13, 0x0,	/* FC_OP */
/* 706 */	NdrFcShort( 0x14e ),	/* Offset= 334 (1040) */
/* 708 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 710 */	NdrFcShort( 0x2 ),	/* Offset= 2 (712) */
/* 712 */	
			0x13, 0x0,	/* FC_OP */
/* 714 */	NdrFcShort( 0x14 ),	/* Offset= 20 (734) */
/* 716 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 718 */	NdrFcShort( 0x10 ),	/* 16 */
/* 720 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 722 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 724 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 726 */	
			0x13, 0x0,	/* FC_OP */
/* 728 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (716) */
/* 730 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 732 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 734 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 736 */	NdrFcShort( 0x20 ),	/* 32 */
/* 738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 740 */	NdrFcShort( 0x0 ),	/* Offset= 0 (740) */
/* 742 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 744 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 746 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 748 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 750 */	NdrFcShort( 0xfe30 ),	/* Offset= -464 (286) */
/* 752 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 754 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 756 */	NdrFcShort( 0x4 ),	/* 4 */
/* 758 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 760 */	NdrFcShort( 0x0 ),	/* 0 */
/* 762 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 764 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 766 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 768 */	NdrFcShort( 0x4 ),	/* 4 */
/* 770 */	NdrFcShort( 0x0 ),	/* 0 */
/* 772 */	NdrFcShort( 0x1 ),	/* 1 */
/* 774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 778 */	0x13, 0x0,	/* FC_OP */
/* 780 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (734) */
/* 782 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 784 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 786 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 788 */	NdrFcShort( 0x8 ),	/* 8 */
/* 790 */	NdrFcShort( 0x0 ),	/* 0 */
/* 792 */	NdrFcShort( 0x6 ),	/* Offset= 6 (798) */
/* 794 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 796 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 798 */	
			0x11, 0x0,	/* FC_RP */
/* 800 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (754) */
/* 802 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 804 */	NdrFcShort( 0x4 ),	/* 4 */
/* 806 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 810 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 812 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 814 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 816 */	NdrFcShort( 0x4 ),	/* 4 */
/* 818 */	NdrFcShort( 0x0 ),	/* 0 */
/* 820 */	NdrFcShort( 0x1 ),	/* 1 */
/* 822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 826 */	0x13, 0x0,	/* FC_OP */
/* 828 */	NdrFcShort( 0xff40 ),	/* Offset= -192 (636) */
/* 830 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 832 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 834 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 836 */	NdrFcShort( 0x8 ),	/* 8 */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 840 */	NdrFcShort( 0x6 ),	/* Offset= 6 (846) */
/* 842 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 844 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 846 */	
			0x11, 0x0,	/* FC_RP */
/* 848 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (802) */
/* 850 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 852 */	NdrFcShort( 0x8 ),	/* 8 */
/* 854 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 856 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 858 */	NdrFcShort( 0x10 ),	/* 16 */
/* 860 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 862 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 864 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (850) */
			0x5b,		/* FC_END */
/* 868 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 870 */	NdrFcShort( 0x18 ),	/* 24 */
/* 872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 874 */	NdrFcShort( 0xa ),	/* Offset= 10 (884) */
/* 876 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 878 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 880 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (856) */
/* 882 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 884 */	
			0x11, 0x0,	/* FC_RP */
/* 886 */	NdrFcShort( 0xfd4a ),	/* Offset= -694 (192) */
/* 888 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 890 */	NdrFcShort( 0x1 ),	/* 1 */
/* 892 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 894 */	NdrFcShort( 0x0 ),	/* 0 */
/* 896 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 898 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 900 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 902 */	NdrFcShort( 0x8 ),	/* 8 */
/* 904 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 906 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 908 */	NdrFcShort( 0x4 ),	/* 4 */
/* 910 */	NdrFcShort( 0x4 ),	/* 4 */
/* 912 */	0x13, 0x0,	/* FC_OP */
/* 914 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (888) */
/* 916 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 918 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 920 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 922 */	NdrFcShort( 0x2 ),	/* 2 */
/* 924 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 926 */	NdrFcShort( 0x0 ),	/* 0 */
/* 928 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 930 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 932 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 934 */	NdrFcShort( 0x8 ),	/* 8 */
/* 936 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 938 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 940 */	NdrFcShort( 0x4 ),	/* 4 */
/* 942 */	NdrFcShort( 0x4 ),	/* 4 */
/* 944 */	0x13, 0x0,	/* FC_OP */
/* 946 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (920) */
/* 948 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 950 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 952 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 954 */	NdrFcShort( 0x4 ),	/* 4 */
/* 956 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 958 */	NdrFcShort( 0x0 ),	/* 0 */
/* 960 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 962 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 964 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 966 */	NdrFcShort( 0x8 ),	/* 8 */
/* 968 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 970 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 972 */	NdrFcShort( 0x4 ),	/* 4 */
/* 974 */	NdrFcShort( 0x4 ),	/* 4 */
/* 976 */	0x13, 0x0,	/* FC_OP */
/* 978 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (952) */
/* 980 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 982 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 984 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 986 */	NdrFcShort( 0x8 ),	/* 8 */
/* 988 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 990 */	NdrFcShort( 0x0 ),	/* 0 */
/* 992 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 994 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 996 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 998 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1000 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1002 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1004 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1006 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1008 */	0x13, 0x0,	/* FC_OP */
/* 1010 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (984) */
/* 1012 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1014 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1016 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1018 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1020 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1022 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1024 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1026 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1028 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1030 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 1032 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1034 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1036 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1016) */
/* 1038 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1040 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1042 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1044 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1024) */
/* 1046 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1046) */
/* 1048 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1050 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1052 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1054 */	NdrFcShort( 0xfc18 ),	/* Offset= -1000 (54) */
/* 1056 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1058 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1060 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1062 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1064 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1066 */	NdrFcShort( 0xfc04 ),	/* Offset= -1020 (46) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            LPSAFEARRAY_UserSize
            ,LPSAFEARRAY_UserMarshal
            ,LPSAFEARRAY_UserUnmarshal
            ,LPSAFEARRAY_UserFree
            }

        };



/* Standard interface: __MIDL_itf_COMMyBaySrv_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ICOMMyBay, ver. 0.0,
   GUID={0xC3E6334F,0x3574,0x4001,{0x8D,0xBE,0x1B,0x54,0x9B,0x38,0x77,0xD0}} */

#pragma code_seg(".orpc")
static const unsigned short ICOMMyBay_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    48,
    84,
    138,
    180,
    240,
    288,
    342,
    384
    };

static const MIDL_STUBLESS_PROXY_INFO ICOMMyBay_ProxyInfo =
    {
    &Object_StubDesc,
    COMMyBaySrv__MIDL_ProcFormatString.Format,
    &ICOMMyBay_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ICOMMyBay_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    COMMyBaySrv__MIDL_ProcFormatString.Format,
    &ICOMMyBay_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(16) _ICOMMyBayProxyVtbl = 
{
    &ICOMMyBay_ProxyInfo,
    &IID_ICOMMyBay,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* ICOMMyBay::login */ ,
    (void *) (INT_PTR) -1 /* ICOMMyBay::logout */ ,
    (void *) (INT_PTR) -1 /* ICOMMyBay::offer */ ,
    (void *) (INT_PTR) -1 /* ICOMMyBay::interested */ ,
    (void *) (INT_PTR) -1 /* ICOMMyBay::getAuctions */ ,
    (void *) (INT_PTR) -1 /* ICOMMyBay::bid */ ,
    (void *) (INT_PTR) -1 /* ICOMMyBay::details */ ,
    (void *) (INT_PTR) -1 /* ICOMMyBay::endauction */ ,
    (void *) (INT_PTR) -1 /* ICOMMyBay::getMessage */
};


static const PRPC_STUB_FUNCTION ICOMMyBay_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _ICOMMyBayStubVtbl =
{
    &IID_ICOMMyBay,
    &ICOMMyBay_ServerInfo,
    16,
    &ICOMMyBay_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    COMMyBaySrv__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x800025b, /* MIDL Version 8.0.603 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _COMMyBaySrv_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ICOMMyBayProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _COMMyBaySrv_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ICOMMyBayStubVtbl,
    0
};

PCInterfaceName const _COMMyBaySrv_InterfaceNamesList[] = 
{
    "ICOMMyBay",
    0
};

const IID *  const _COMMyBaySrv_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _COMMyBaySrv_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _COMMyBaySrv, pIID, n)

int __stdcall _COMMyBaySrv_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_COMMyBaySrv_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo COMMyBaySrv_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _COMMyBaySrv_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _COMMyBaySrv_StubVtblList,
    (const PCInterfaceName * ) & _COMMyBaySrv_InterfaceNamesList,
    (const IID ** ) & _COMMyBaySrv_BaseIIDList,
    & _COMMyBaySrv_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

