/**---------------------------------------------------------------------
// project:	RPCMyCasino
// file:	RpcMem.c
//
// summary:	RPC memory create/free method implementation
//
//			Copyright (c) 2018 OTH-Amberg/Weiden. All rights reserved.
//
//			Date		Developer			Change
//			28.12.2017	Mathias Schneider	Created
 *-------------------------------------------------------------------**/

#include "rpc.h"   

/**---------------------------------------------------------------------
 * <summary>	Midl user allocate. </summary>
 *
 * <param name="len">	The length. </param>
 *-------------------------------------------------------------------**/

void __RPC_FAR * __RPC_API midl_user_allocate(size_t len)
{
	return(malloc(len));
}

/**---------------------------------------------------------------------
 * <summary>	Midl user free. </summary>
 *
 * <param name="ptr">	[in,out] If non-null, the pointer. </param>
 *-------------------------------------------------------------------**/

void __RPC_API midl_user_free(void __RPC_FAR * ptr)
{
	free(ptr);
}