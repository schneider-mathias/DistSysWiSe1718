/************************************************************/
/*                                                          */
/* Inhalt:    RpcMem - Speicherverwaltung		            */
/*                                                          */
/* Autor:	  Johannes Sauer		                        */
/* Stand:     23. Jan 2018                                  */
/*															*/
/************************************************************/

#include "rpc.h"   

/// <summary>
/// Speicherallocation
/// </summary>
/// <param name="len"> Länge die allokiert werden soll </param>
void __RPC_FAR * __RPC_API midl_user_allocate(size_t len)
{
	return(malloc(len));
}

/// <summary>
/// Speicherfreigabe
/// </summary>
/// <param name="ptr"> Pointer der freigegeben werden soll </param>
void __RPC_API midl_user_free(void __RPC_FAR * ptr)
{
	free(ptr);
}