/************************************************************/
/*                                                          */
/* Inhalt:    RpcExcepton						            */
/*                                                          */
/* Autor:	  Johannes Sauer		                        */
/* Stand:     23. Jan 2018                                  */
/*															*/
/************************************************************/

#ifndef RPC_EXCEPTION_H
#define RPC_EXCEPTION_H

#include <rpc.h>

#define RPC_STATUS_ASSERT(fct, stat) {                      \
   if (status)                                              \
     {                                                      \
      fprintf(stderr, "Error calling "fct": 0x%x\n", stat); \
      exit(status);                                         \
     }                                                      \
  }


#ifdef __cplusplus

#include <string>

using std::string;


class RpcException
{
public:
	RpcException(RPC_STATUS status, const string& errorText, const string& errorType)
		: status(status), errorText(errorText), errorType(errorType) {}

	RPC_STATUS  GetStatus(void) { return(status); }
	const char *GetErrorText(void) { return(errorText.data()); }
	const char *GetErrorType(void) { return(errorType.data()); }

	static void Raise(RPC_STATUS status, const char *errText, const char *errType)
	{
		throw(RpcException(status, errText, errType));
	}

private:
	RPC_STATUS status;
	string errorText;
	string errorType;
};

#endif

#endif 