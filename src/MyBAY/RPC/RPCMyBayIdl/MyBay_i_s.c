

/* this ALWAYS GENERATED file contains the RPC server stubs */


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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#include <string.h>
#include "MyBay_i_h.h"

#define TYPE_FORMAT_STRING_SIZE   59                                
#define PROC_FORMAT_STRING_SIZE   439                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _MyBay_i_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MyBay_i_MIDL_TYPE_FORMAT_STRING;

typedef struct _MyBay_i_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MyBay_i_MIDL_PROC_FORMAT_STRING;

typedef struct _MyBay_i_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } MyBay_i_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

extern const MyBay_i_MIDL_TYPE_FORMAT_STRING MyBay_i__MIDL_TypeFormatString;
extern const MyBay_i_MIDL_PROC_FORMAT_STRING MyBay_i__MIDL_ProcFormatString;
extern const MyBay_i_MIDL_EXPR_FORMAT_STRING MyBay_i__MIDL_ExprFormatString;

/* Standard interface: MyBay_i, ver. 1.0,
   GUID={0xBEE48EE5,0x830F,0x453C,{0xAA,0xE4,0x9B,0x6B,0x3F,0x4F,0xC2,0x70}} */


extern const MIDL_SERVER_INFO MyBay_i_ServerInfo;

extern const RPC_DISPATCH_TABLE MyBay_i_v1_0_DispatchTable;

static const RPC_SERVER_INTERFACE MyBay_i___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0xBEE48EE5,0x830F,0x453C,{0xAA,0xE4,0x9B,0x6B,0x3F,0x4F,0xC2,0x70}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    (RPC_DISPATCH_TABLE*)&MyBay_i_v1_0_DispatchTable,
    0,
    0,
    0,
    &MyBay_i_ServerInfo,
    0x04000000
    };
RPC_IF_HANDLE MyBay_i_v1_0_s_ifspec = (RPC_IF_HANDLE)& MyBay_i___RpcServerInterface;

extern const MIDL_STUB_DESC MyBay_i_StubDesc;


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MyBay_i_MIDL_PROC_FORMAT_STRING MyBay_i__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure login */

			0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter username */

/* 24 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 26 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 28 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter password */

/* 30 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 32 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 34 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter sessionId */

/* 36 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 38 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 42 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

	/* Procedure logout */

/* 48 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 50 */	NdrFcLong( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x1 ),	/* 1 */
/* 56 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
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
/* 74 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 76 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 80 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 82 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

	/* Procedure offer */

/* 84 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x2 ),	/* 2 */
/* 92 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 94 */	NdrFcShort( 0x18 ),	/* 24 */
/* 96 */	NdrFcShort( 0x24 ),	/* 36 */
/* 98 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 100 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 102 */	NdrFcShort( 0x0 ),	/* 0 */
/* 104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 108 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 110 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter articleName */

/* 114 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 116 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 118 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter startBid */

/* 120 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 122 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 124 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 126 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 128 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 132 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 134 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 136 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

	/* Procedure interested */

/* 138 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 140 */	NdrFcLong( 0x0 ),	/* 0 */
/* 144 */	NdrFcShort( 0x3 ),	/* 3 */
/* 146 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
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
/* 164 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 168 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

	/* Procedure getAuctions */

/* 180 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0x4 ),	/* 4 */
/* 188 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 190 */	NdrFcShort( 0x10 ),	/* 16 */
/* 192 */	NdrFcShort( 0x24 ),	/* 36 */
/* 194 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 196 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 198 */	NdrFcShort( 0x1 ),	/* 1 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 206 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter flags */

/* 210 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 212 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter articleName */

/* 216 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 218 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 220 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter countAuctions */

/* 222 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 224 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctions */

/* 228 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 230 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 232 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */

/* 234 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 236 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 238 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

	/* Procedure bid */

/* 240 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 242 */	NdrFcLong( 0x0 ),	/* 0 */
/* 246 */	NdrFcShort( 0x5 ),	/* 5 */
/* 248 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
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
/* 266 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 270 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 272 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 274 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bidVal */

/* 276 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 278 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 280 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 282 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 284 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 286 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

	/* Procedure details */

/* 288 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0x6 ),	/* 6 */
/* 296 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
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
/* 314 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 316 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 318 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 320 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter allBids */

/* 324 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 326 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 328 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Parameter countBids */

/* 330 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 332 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 336 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 338 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 340 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

	/* Procedure endauction */

/* 342 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 344 */	NdrFcLong( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0x7 ),	/* 7 */
/* 350 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
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
/* 368 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter auctionNumber */

/* 372 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 374 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 376 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 378 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 380 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 382 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

	/* Procedure getMessage */

/* 384 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 386 */	NdrFcLong( 0x0 ),	/* 0 */
/* 390 */	NdrFcShort( 0x8 ),	/* 8 */
/* 392 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 394 */	NdrFcShort( 0x8 ),	/* 8 */
/* 396 */	NdrFcShort( 0x3d ),	/* 61 */
/* 398 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x5,		/* 5 */
/* 400 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 402 */	NdrFcShort( 0x1 ),	/* 1 */
/* 404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sessionId */

/* 408 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 410 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 412 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter messageAvailable */

/* 414 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 416 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 418 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Parameter messageType */

/* 420 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 422 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 424 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter message */

/* 426 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 428 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 430 */	NdrFcShort( 0x20 ),	/* Type Offset=32 */

	/* Return value */

/* 432 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 434 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 436 */	0x10,		/* FC_ERROR_STATUS_T */
			0x0,		/* 0 */

			0x0
        }
    };

static const MyBay_i_MIDL_TYPE_FORMAT_STRING MyBay_i__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  8 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 10 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 12 */	NdrFcShort( 0x14 ),	/* Offset= 20 (32) */
/* 14 */	
			0x1c,		/* FC_CVARRAY */
			0x0,		/* 0 */
/* 16 */	NdrFcShort( 0x1 ),	/* 1 */
/* 18 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 24 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 26 */	NdrFcShort( 0x4 ),	/* 4 */
/* 28 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 30 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 32 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 34 */	NdrFcShort( 0xc ),	/* 12 */
/* 36 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 38 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 40 */	NdrFcShort( 0x8 ),	/* 8 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	0x12, 0x0,	/* FC_UP */
/* 46 */	NdrFcShort( 0xffe0 ),	/* Offset= -32 (14) */
/* 48 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 50 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 52 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 54 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 56 */	0x3,		/* FC_SMALL */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const unsigned short MyBay_i_FormatStringOffsetTable[] =
    {
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


static const MIDL_STUB_DESC MyBay_i_StubDesc = 
    {
    (void *)& MyBay_i___RpcServerInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    0,
    0,
    0,
    0,
    0,
    MyBay_i__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x800025b, /* MIDL Version 8.0.603 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

static const RPC_DISPATCH_FUNCTION MyBay_i_table[] =
    {
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    0
    };
static const RPC_DISPATCH_TABLE MyBay_i_v1_0_DispatchTable = 
    {
    9,
    (RPC_DISPATCH_FUNCTION*)MyBay_i_table
    };

static const SERVER_ROUTINE MyBay_i_ServerRoutineTable[] = 
    {
    (SERVER_ROUTINE)login,
    (SERVER_ROUTINE)logout,
    (SERVER_ROUTINE)offer,
    (SERVER_ROUTINE)interested,
    (SERVER_ROUTINE)getAuctions,
    (SERVER_ROUTINE)bid,
    (SERVER_ROUTINE)details,
    (SERVER_ROUTINE)endauction,
    (SERVER_ROUTINE)getMessage
    };

static const MIDL_SERVER_INFO MyBay_i_ServerInfo = 
    {
    &MyBay_i_StubDesc,
    MyBay_i_ServerRoutineTable,
    MyBay_i__MIDL_ProcFormatString.Format,
    MyBay_i_FormatStringOffsetTable,
    0,
    0,
    0,
    0};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

