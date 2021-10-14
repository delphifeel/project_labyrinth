#ifndef _CORE_TCP_SERVER_H_
#define _CORE_TCP_SERVER_H_

#include "CORE-object.h"
#include "CORE-types.h"
#include "CORE-debug.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(CORE_TCPServer);

typedef void (*OnReadFunc)(	CORE_TCPServer instance, const uint8 data[], uint32 data_size, 
							uint8 **out_response_data, uint32 *out_response_data_size);
typedef void (*OnNewConnectionFunc)(CORE_TCPServer instance);
typedef void (*OnCloseConnectionFunc)(CORE_TCPServer instance);
typedef void (*OnErrorFunc)(CORE_TCPServer instance, const char *error_message);

/*****************************************************************************************************************************/

void  	CORE_TCPServer_OnRead(CORE_TCPServer instance, OnReadFunc on_read);
void  	CORE_TCPServer_OnError(CORE_TCPServer instance, OnErrorFunc on_error);
void  	CORE_TCPServer_OnNewConnection(CORE_TCPServer instance, OnNewConnectionFunc on_new_connection);
void  	CORE_TCPServer_OnCloseConnection(CORE_TCPServer instance, OnCloseConnectionFunc on_close_connection);

void  	CORE_TCPServer_Setup(CORE_TCPServer instance, uint32 port);
void 	CORE_TCPServer_Start(CORE_TCPServer instance);

void 	CORE_TCPServer_Create(CORE_TCPServer *instance_ptr);
void 	CORE_TCPServer_Free(CORE_TCPServer *instance_ptr);

/*****************************************************************************************************************************/

#endif
