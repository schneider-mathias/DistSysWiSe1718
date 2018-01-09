

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#define TYPE_FORMAT_STRING_SIZE   1083                              
#define PROC_FORMAT_STRING_SIZE   493                               
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
/*  8 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 16 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 18 */	NdrFcShort( 0x1 ),	/* 1 */
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

	/* Parameter errMsg */

/* 42 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 44 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 46 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 48 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 50 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 52 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure logout */

/* 54 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 56 */	NdrFcLong( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0x8 ),	/* 8 */
/* 62 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 64 */	NdrFcShort( 0x8 ),	/* 8 */
/* 66 */	NdrFcShort( 0x8 ),	/* 8 */
/* 68 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 70 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 72 */	NdrFcShort( 0x1 ),	/* 1 */
/* 74 */	NdrFcShort( 0x0 ),	/* 0 */
/* 76 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 78 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 80 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter errMsg */

/* 84 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 86 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 88 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 90 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 92 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 94 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure offer */

/* 96 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 98 */	NdrFcLong( 0x0 ),	/* 0 */
/* 102 */	NdrFcShort( 0x9 ),	/* 9 */
/* 104 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 106 */	NdrFcShort( 0x18 ),	/* 24 */
/* 108 */	NdrFcShort( 0x24 ),	/* 36 */
/* 110 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 112 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 114 */	NdrFcShort( 0x1 ),	/* 1 */
/* 116 */	NdrFcShort( 0x1 ),	/* 1 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 120 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 122 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter articleName */

/* 126 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 128 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 130 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter startBid */

/* 132 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 134 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 136 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 138 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 140 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter errMsg */

/* 144 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 146 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 148 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 150 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 152 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 154 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure interested */

/* 156 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 158 */	NdrFcLong( 0x0 ),	/* 0 */
/* 162 */	NdrFcShort( 0xa ),	/* 10 */
/* 164 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 166 */	NdrFcShort( 0x10 ),	/* 16 */
/* 168 */	NdrFcShort( 0x8 ),	/* 8 */
/* 170 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 172 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 174 */	NdrFcShort( 0x1 ),	/* 1 */
/* 176 */	NdrFcShort( 0x0 ),	/* 0 */
/* 178 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 180 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 182 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 184 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 186 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 188 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 190 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter errMsg */

/* 192 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 194 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 196 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 198 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 200 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 202 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getAuctions */

/* 204 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 206 */	NdrFcLong( 0x0 ),	/* 0 */
/* 210 */	NdrFcShort( 0xb ),	/* 11 */
/* 212 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 214 */	NdrFcShort( 0x10 ),	/* 16 */
/* 216 */	NdrFcShort( 0x24 ),	/* 36 */
/* 218 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x7,		/* 7 */
/* 220 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 222 */	NdrFcShort( 0x1 ),	/* 1 */
/* 224 */	NdrFcShort( 0x1 ),	/* 1 */
/* 226 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 228 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 230 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 232 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter flags */

/* 234 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 236 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 238 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter articleName */

/* 240 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 242 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 244 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter countAuctions */

/* 246 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 248 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctions */

/* 252 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 254 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 256 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter errMsg */

/* 258 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 260 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 262 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 264 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 266 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure bid */

/* 270 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 272 */	NdrFcLong( 0x0 ),	/* 0 */
/* 276 */	NdrFcShort( 0xc ),	/* 12 */
/* 278 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 280 */	NdrFcShort( 0x20 ),	/* 32 */
/* 282 */	NdrFcShort( 0x8 ),	/* 8 */
/* 284 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x5,		/* 5 */
/* 286 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 288 */	NdrFcShort( 0x1 ),	/* 1 */
/* 290 */	NdrFcShort( 0x0 ),	/* 0 */
/* 292 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 294 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 296 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 298 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 300 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 302 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 304 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bidVal */

/* 306 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 308 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 310 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter errMsg */

/* 312 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 314 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 316 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 318 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 320 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure details */

/* 324 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 326 */	NdrFcLong( 0x0 ),	/* 0 */
/* 330 */	NdrFcShort( 0xd ),	/* 13 */
/* 332 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 334 */	NdrFcShort( 0x10 ),	/* 16 */
/* 336 */	NdrFcShort( 0x24 ),	/* 36 */
/* 338 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x6,		/* 6 */
/* 340 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 342 */	NdrFcShort( 0x1 ),	/* 1 */
/* 344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 348 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 350 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 352 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 354 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 356 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter allBids */

/* 360 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 362 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 364 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter countBids */

/* 366 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 368 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter errMsg */

/* 372 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 374 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 376 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 378 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 380 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 382 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure endauction */

/* 384 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 386 */	NdrFcLong( 0x0 ),	/* 0 */
/* 390 */	NdrFcShort( 0xe ),	/* 14 */
/* 392 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 394 */	NdrFcShort( 0x10 ),	/* 16 */
/* 396 */	NdrFcShort( 0x8 ),	/* 8 */
/* 398 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
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

	/* Parameter auctionNumber */

/* 414 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 416 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 418 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter errMsg */

/* 420 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 422 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 424 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 426 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 428 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getMessage */

/* 432 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 434 */	NdrFcLong( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0xf ),	/* 15 */
/* 440 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 442 */	NdrFcShort( 0x8 ),	/* 8 */
/* 444 */	NdrFcShort( 0x40 ),	/* 64 */
/* 446 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x6,		/* 6 */
/* 448 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 450 */	NdrFcShort( 0x1 ),	/* 1 */
/* 452 */	NdrFcShort( 0x0 ),	/* 0 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 456 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 458 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 460 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter einBoolwasIchNichtWeiß */

/* 462 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 464 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 466 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter messageType */

/* 468 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 470 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter message */

/* 474 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 476 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 478 */	NdrFcShort( 0x430 ),	/* Type Offset=1072 */

	/* Parameter errMsg */

/* 480 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 482 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 484 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 486 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 488 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 490 */	0x8,		/* FC_LONG */
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
/* 44 */	NdrFcShort( 0x6 ),	/* Offset= 6 (50) */
/* 46 */	
			0x13, 0x0,	/* FC_OP */
/* 48 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (18) */
/* 50 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 52 */	NdrFcShort( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x4 ),	/* 4 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (46) */
/* 60 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 62 */	NdrFcShort( 0x3f2 ),	/* Offset= 1010 (1072) */
/* 64 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 66 */	NdrFcShort( 0x2 ),	/* Offset= 2 (68) */
/* 68 */	
			0x13, 0x0,	/* FC_OP */
/* 70 */	NdrFcShort( 0x3d8 ),	/* Offset= 984 (1054) */
/* 72 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 74 */	NdrFcShort( 0x18 ),	/* 24 */
/* 76 */	NdrFcShort( 0xa ),	/* 10 */
/* 78 */	NdrFcLong( 0x8 ),	/* 8 */
/* 82 */	NdrFcShort( 0x5a ),	/* Offset= 90 (172) */
/* 84 */	NdrFcLong( 0xd ),	/* 13 */
/* 88 */	NdrFcShort( 0x90 ),	/* Offset= 144 (232) */
/* 90 */	NdrFcLong( 0x9 ),	/* 9 */
/* 94 */	NdrFcShort( 0xc2 ),	/* Offset= 194 (288) */
/* 96 */	NdrFcLong( 0xc ),	/* 12 */
/* 100 */	NdrFcShort( 0x2bc ),	/* Offset= 700 (800) */
/* 102 */	NdrFcLong( 0x24 ),	/* 36 */
/* 106 */	NdrFcShort( 0x2e6 ),	/* Offset= 742 (848) */
/* 108 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 112 */	NdrFcShort( 0x302 ),	/* Offset= 770 (882) */
/* 114 */	NdrFcLong( 0x10 ),	/* 16 */
/* 118 */	NdrFcShort( 0x31c ),	/* Offset= 796 (914) */
/* 120 */	NdrFcLong( 0x2 ),	/* 2 */
/* 124 */	NdrFcShort( 0x336 ),	/* Offset= 822 (946) */
/* 126 */	NdrFcLong( 0x3 ),	/* 3 */
/* 130 */	NdrFcShort( 0x350 ),	/* Offset= 848 (978) */
/* 132 */	NdrFcLong( 0x14 ),	/* 20 */
/* 136 */	NdrFcShort( 0x36a ),	/* Offset= 874 (1010) */
/* 138 */	NdrFcShort( 0xffff ),	/* Offset= -1 (137) */
/* 140 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 142 */	NdrFcShort( 0x4 ),	/* 4 */
/* 144 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 146 */	NdrFcShort( 0x0 ),	/* 0 */
/* 148 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 150 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 152 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 154 */	NdrFcShort( 0x4 ),	/* 4 */
/* 156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0x1 ),	/* 1 */
/* 160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	0x13, 0x0,	/* FC_OP */
/* 166 */	NdrFcShort( 0xff6c ),	/* Offset= -148 (18) */
/* 168 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 170 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 172 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 174 */	NdrFcShort( 0x8 ),	/* 8 */
/* 176 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 178 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 180 */	NdrFcShort( 0x4 ),	/* 4 */
/* 182 */	NdrFcShort( 0x4 ),	/* 4 */
/* 184 */	0x11, 0x0,	/* FC_RP */
/* 186 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (140) */
/* 188 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 190 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 192 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 194 */	NdrFcLong( 0x0 ),	/* 0 */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 204 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 206 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 208 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 210 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 214 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 216 */	NdrFcShort( 0x0 ),	/* 0 */
/* 218 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 220 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 224 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 226 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 228 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (192) */
/* 230 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 232 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 234 */	NdrFcShort( 0x8 ),	/* 8 */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x6 ),	/* Offset= 6 (244) */
/* 240 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 242 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 244 */	
			0x11, 0x0,	/* FC_RP */
/* 246 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (210) */
/* 248 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 250 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 254 */	NdrFcShort( 0x0 ),	/* 0 */
/* 256 */	NdrFcShort( 0x0 ),	/* 0 */
/* 258 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 260 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 262 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 264 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 266 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 268 */	NdrFcShort( 0x0 ),	/* 0 */
/* 270 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 274 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 276 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 280 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 282 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 284 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (248) */
/* 286 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 288 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 290 */	NdrFcShort( 0x8 ),	/* 8 */
/* 292 */	NdrFcShort( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0x6 ),	/* Offset= 6 (300) */
/* 296 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 298 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 300 */	
			0x11, 0x0,	/* FC_RP */
/* 302 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (266) */
/* 304 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 306 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 308 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 310 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 312 */	NdrFcShort( 0x2 ),	/* Offset= 2 (314) */
/* 314 */	NdrFcShort( 0x10 ),	/* 16 */
/* 316 */	NdrFcShort( 0x2f ),	/* 47 */
/* 318 */	NdrFcLong( 0x14 ),	/* 20 */
/* 322 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 324 */	NdrFcLong( 0x3 ),	/* 3 */
/* 328 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 330 */	NdrFcLong( 0x11 ),	/* 17 */
/* 334 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 336 */	NdrFcLong( 0x2 ),	/* 2 */
/* 340 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 342 */	NdrFcLong( 0x4 ),	/* 4 */
/* 346 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 348 */	NdrFcLong( 0x5 ),	/* 5 */
/* 352 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 354 */	NdrFcLong( 0xb ),	/* 11 */
/* 358 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 360 */	NdrFcLong( 0xa ),	/* 10 */
/* 364 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 366 */	NdrFcLong( 0x6 ),	/* 6 */
/* 370 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (602) */
/* 372 */	NdrFcLong( 0x7 ),	/* 7 */
/* 376 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 378 */	NdrFcLong( 0x8 ),	/* 8 */
/* 382 */	NdrFcShort( 0xfeb0 ),	/* Offset= -336 (46) */
/* 384 */	NdrFcLong( 0xd ),	/* 13 */
/* 388 */	NdrFcShort( 0xff3c ),	/* Offset= -196 (192) */
/* 390 */	NdrFcLong( 0x9 ),	/* 9 */
/* 394 */	NdrFcShort( 0xff6e ),	/* Offset= -146 (248) */
/* 396 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 400 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (608) */
/* 402 */	NdrFcLong( 0x24 ),	/* 36 */
/* 406 */	NdrFcShort( 0xd2 ),	/* Offset= 210 (616) */
/* 408 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 412 */	NdrFcShort( 0xcc ),	/* Offset= 204 (616) */
/* 414 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 418 */	NdrFcShort( 0xfc ),	/* Offset= 252 (670) */
/* 420 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 424 */	NdrFcShort( 0xfa ),	/* Offset= 250 (674) */
/* 426 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 430 */	NdrFcShort( 0xf8 ),	/* Offset= 248 (678) */
/* 432 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 436 */	NdrFcShort( 0xf6 ),	/* Offset= 246 (682) */
/* 438 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 442 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (686) */
/* 444 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 448 */	NdrFcShort( 0xf2 ),	/* Offset= 242 (690) */
/* 450 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 454 */	NdrFcShort( 0xdc ),	/* Offset= 220 (674) */
/* 456 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 460 */	NdrFcShort( 0xda ),	/* Offset= 218 (678) */
/* 462 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 466 */	NdrFcShort( 0xe4 ),	/* Offset= 228 (694) */
/* 468 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 472 */	NdrFcShort( 0xda ),	/* Offset= 218 (690) */
/* 474 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 478 */	NdrFcShort( 0xdc ),	/* Offset= 220 (698) */
/* 480 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 484 */	NdrFcShort( 0xda ),	/* Offset= 218 (702) */
/* 486 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 490 */	NdrFcShort( 0xd8 ),	/* Offset= 216 (706) */
/* 492 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 496 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (710) */
/* 498 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 502 */	NdrFcShort( 0xdc ),	/* Offset= 220 (722) */
/* 504 */	NdrFcLong( 0x10 ),	/* 16 */
/* 508 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 510 */	NdrFcLong( 0x12 ),	/* 18 */
/* 514 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 516 */	NdrFcLong( 0x13 ),	/* 19 */
/* 520 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 522 */	NdrFcLong( 0x15 ),	/* 21 */
/* 526 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 528 */	NdrFcLong( 0x16 ),	/* 22 */
/* 532 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 534 */	NdrFcLong( 0x17 ),	/* 23 */
/* 538 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 540 */	NdrFcLong( 0xe ),	/* 14 */
/* 544 */	NdrFcShort( 0xba ),	/* Offset= 186 (730) */
/* 546 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 550 */	NdrFcShort( 0xbe ),	/* Offset= 190 (740) */
/* 552 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 556 */	NdrFcShort( 0xbc ),	/* Offset= 188 (744) */
/* 558 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 562 */	NdrFcShort( 0x70 ),	/* Offset= 112 (674) */
/* 564 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 568 */	NdrFcShort( 0x6e ),	/* Offset= 110 (678) */
/* 570 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 574 */	NdrFcShort( 0x6c ),	/* Offset= 108 (682) */
/* 576 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 580 */	NdrFcShort( 0x62 ),	/* Offset= 98 (678) */
/* 582 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 586 */	NdrFcShort( 0x5c ),	/* Offset= 92 (678) */
/* 588 */	NdrFcLong( 0x0 ),	/* 0 */
/* 592 */	NdrFcShort( 0x0 ),	/* Offset= 0 (592) */
/* 594 */	NdrFcLong( 0x1 ),	/* 1 */
/* 598 */	NdrFcShort( 0x0 ),	/* Offset= 0 (598) */
/* 600 */	NdrFcShort( 0xffff ),	/* Offset= -1 (599) */
/* 602 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 604 */	NdrFcShort( 0x8 ),	/* 8 */
/* 606 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 608 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 610 */	NdrFcShort( 0x2 ),	/* Offset= 2 (612) */
/* 612 */	
			0x13, 0x0,	/* FC_OP */
/* 614 */	NdrFcShort( 0x1b8 ),	/* Offset= 440 (1054) */
/* 616 */	
			0x13, 0x0,	/* FC_OP */
/* 618 */	NdrFcShort( 0x20 ),	/* Offset= 32 (650) */
/* 620 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 622 */	NdrFcLong( 0x2f ),	/* 47 */
/* 626 */	NdrFcShort( 0x0 ),	/* 0 */
/* 628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 630 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 632 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 634 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 636 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 638 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 640 */	NdrFcShort( 0x1 ),	/* 1 */
/* 642 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 644 */	NdrFcShort( 0x4 ),	/* 4 */
/* 646 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 648 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 650 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 652 */	NdrFcShort( 0x10 ),	/* 16 */
/* 654 */	NdrFcShort( 0x0 ),	/* 0 */
/* 656 */	NdrFcShort( 0xa ),	/* Offset= 10 (666) */
/* 658 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 660 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 662 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (620) */
/* 664 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 666 */	
			0x13, 0x0,	/* FC_OP */
/* 668 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (638) */
/* 670 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 672 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 674 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 676 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 678 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 680 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 682 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 684 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 686 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 688 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 690 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 692 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 694 */	
			0x13, 0x0,	/* FC_OP */
/* 696 */	NdrFcShort( 0xffa2 ),	/* Offset= -94 (602) */
/* 698 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 700 */	NdrFcShort( 0xfd72 ),	/* Offset= -654 (46) */
/* 702 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 704 */	NdrFcShort( 0xfe00 ),	/* Offset= -512 (192) */
/* 706 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 708 */	NdrFcShort( 0xfe34 ),	/* Offset= -460 (248) */
/* 710 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 712 */	NdrFcShort( 0x2 ),	/* Offset= 2 (714) */
/* 714 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 716 */	NdrFcShort( 0x2 ),	/* Offset= 2 (718) */
/* 718 */	
			0x13, 0x0,	/* FC_OP */
/* 720 */	NdrFcShort( 0x14e ),	/* Offset= 334 (1054) */
/* 722 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 724 */	NdrFcShort( 0x2 ),	/* Offset= 2 (726) */
/* 726 */	
			0x13, 0x0,	/* FC_OP */
/* 728 */	NdrFcShort( 0x14 ),	/* Offset= 20 (748) */
/* 730 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 732 */	NdrFcShort( 0x10 ),	/* 16 */
/* 734 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 736 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 738 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 740 */	
			0x13, 0x0,	/* FC_OP */
/* 742 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (730) */
/* 744 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 746 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 748 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 750 */	NdrFcShort( 0x20 ),	/* 32 */
/* 752 */	NdrFcShort( 0x0 ),	/* 0 */
/* 754 */	NdrFcShort( 0x0 ),	/* Offset= 0 (754) */
/* 756 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 758 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 760 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 762 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 764 */	NdrFcShort( 0xfe34 ),	/* Offset= -460 (304) */
/* 766 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 768 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 770 */	NdrFcShort( 0x4 ),	/* 4 */
/* 772 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 778 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 780 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 782 */	NdrFcShort( 0x4 ),	/* 4 */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x1 ),	/* 1 */
/* 788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0x0 ),	/* 0 */
/* 792 */	0x13, 0x0,	/* FC_OP */
/* 794 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (748) */
/* 796 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 798 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 800 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 802 */	NdrFcShort( 0x8 ),	/* 8 */
/* 804 */	NdrFcShort( 0x0 ),	/* 0 */
/* 806 */	NdrFcShort( 0x6 ),	/* Offset= 6 (812) */
/* 808 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 810 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 812 */	
			0x11, 0x0,	/* FC_RP */
/* 814 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (768) */
/* 816 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 818 */	NdrFcShort( 0x4 ),	/* 4 */
/* 820 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 824 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 826 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 828 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 830 */	NdrFcShort( 0x4 ),	/* 4 */
/* 832 */	NdrFcShort( 0x0 ),	/* 0 */
/* 834 */	NdrFcShort( 0x1 ),	/* 1 */
/* 836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 840 */	0x13, 0x0,	/* FC_OP */
/* 842 */	NdrFcShort( 0xff40 ),	/* Offset= -192 (650) */
/* 844 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 846 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 848 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 850 */	NdrFcShort( 0x8 ),	/* 8 */
/* 852 */	NdrFcShort( 0x0 ),	/* 0 */
/* 854 */	NdrFcShort( 0x6 ),	/* Offset= 6 (860) */
/* 856 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 858 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 860 */	
			0x11, 0x0,	/* FC_RP */
/* 862 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (816) */
/* 864 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 866 */	NdrFcShort( 0x8 ),	/* 8 */
/* 868 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 870 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 872 */	NdrFcShort( 0x10 ),	/* 16 */
/* 874 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 876 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 878 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (864) */
			0x5b,		/* FC_END */
/* 882 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 884 */	NdrFcShort( 0x18 ),	/* 24 */
/* 886 */	NdrFcShort( 0x0 ),	/* 0 */
/* 888 */	NdrFcShort( 0xa ),	/* Offset= 10 (898) */
/* 890 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 892 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 894 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (870) */
/* 896 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 898 */	
			0x11, 0x0,	/* FC_RP */
/* 900 */	NdrFcShort( 0xfd4e ),	/* Offset= -690 (210) */
/* 902 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 904 */	NdrFcShort( 0x1 ),	/* 1 */
/* 906 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 908 */	NdrFcShort( 0x0 ),	/* 0 */
/* 910 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 912 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 914 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 916 */	NdrFcShort( 0x8 ),	/* 8 */
/* 918 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 920 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 922 */	NdrFcShort( 0x4 ),	/* 4 */
/* 924 */	NdrFcShort( 0x4 ),	/* 4 */
/* 926 */	0x13, 0x0,	/* FC_OP */
/* 928 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (902) */
/* 930 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 932 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 934 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 936 */	NdrFcShort( 0x2 ),	/* 2 */
/* 938 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 940 */	NdrFcShort( 0x0 ),	/* 0 */
/* 942 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 944 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 946 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 948 */	NdrFcShort( 0x8 ),	/* 8 */
/* 950 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 952 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 954 */	NdrFcShort( 0x4 ),	/* 4 */
/* 956 */	NdrFcShort( 0x4 ),	/* 4 */
/* 958 */	0x13, 0x0,	/* FC_OP */
/* 960 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (934) */
/* 962 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 964 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 966 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 968 */	NdrFcShort( 0x4 ),	/* 4 */
/* 970 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 974 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 976 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 978 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 980 */	NdrFcShort( 0x8 ),	/* 8 */
/* 982 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 984 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 986 */	NdrFcShort( 0x4 ),	/* 4 */
/* 988 */	NdrFcShort( 0x4 ),	/* 4 */
/* 990 */	0x13, 0x0,	/* FC_OP */
/* 992 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (966) */
/* 994 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 996 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 998 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 1000 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1002 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1004 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1006 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1008 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1010 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1012 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1014 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1016 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1018 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1020 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1022 */	0x13, 0x0,	/* FC_OP */
/* 1024 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (998) */
/* 1026 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1028 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1030 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1032 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1034 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1036 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1038 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1040 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1042 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1044 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 1046 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1048 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1050 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1030) */
/* 1052 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1054 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1056 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1058 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1038) */
/* 1060 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1060) */
/* 1062 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1064 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1066 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1068 */	NdrFcShort( 0xfc1c ),	/* Offset= -996 (72) */
/* 1070 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1072 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1074 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1076 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1078 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1080 */	NdrFcShort( 0xfc08 ),	/* Offset= -1016 (64) */

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
   GUID={0x148D7498,0x389E,0x476E,{0x8A,0x40,0x31,0x18,0x00,0xA2,0x5C,0xE4}} */

#pragma code_seg(".orpc")
static const unsigned short ICOMMyBay_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    54,
    96,
    156,
    204,
    270,
    324,
    384,
    432
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

