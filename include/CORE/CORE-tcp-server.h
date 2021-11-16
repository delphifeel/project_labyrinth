#ifndef _CORE_TCP_SERVER_H_
#define _CORE_TCP_SERVER_H_

#include "CORE-object.h"
#include "CORE-types.h"
#include "CORE-debug.h"

/*****************************************************************************************************************************/

/*
 * 		TODO(delphifeel): Make possible to create several TCP server's.
 * 		Right now after you call `CORE_TCPServer_Start` we are bounded to 
 * 		infinite event loop so after that we can't start another TCP server. 
 */

typedef void *CORE_TCPServer_ClientConnection;

CORE_OBJECT_DEFINE(CORE_TCPServer);

typedef void (*OnReadFunc)(	CORE_TCPServer instance, void *context, CORE_TCPServer_ClientConnection client_connection, 
							const uint8 data[], uint32 data_size);
typedef void (*OnNewConnectionFunc)(CORE_TCPServer instance, void *context, CORE_TCPServer_ClientConnection client_connection);
typedef void (*OnCloseConnectionFunc)(CORE_TCPServer instance, void *context, CORE_TCPServer_ClientConnection client_connection);
typedef void (*OnErrorFunc)(CORE_TCPServer instance, void *context, const char *error_message);

/*****************************************************************************************************************************/

void  	CORE_TCPServer_OnRead(CORE_TCPServer instance, OnReadFunc on_read);
void  	CORE_TCPServer_OnError(CORE_TCPServer instance, OnErrorFunc on_error);
void  	CORE_TCPServer_OnNewConnection(CORE_TCPServer instance, OnNewConnectionFunc on_new_connection);
void  	CORE_TCPServer_OnCloseConnection(CORE_TCPServer instance, OnCloseConnectionFunc on_close_connection);

void    CORE_TCPServer_CloseConnection(CORE_TCPServer instance, CORE_TCPServer_ClientConnection client_connection);
void  	CORE_TCPServer_SetContext(CORE_TCPServer instance, void *context);
void 	CORE_TCPServer_Write(CORE_TCPServer instance, CORE_TCPServer_ClientConnection client_connection, 
							 const uint8 data[], uint32 data_size);
void  	CORE_TCPServer_Setup(CORE_TCPServer instance, uint32 port);
void 	CORE_TCPServer_Start(CORE_TCPServer instance);

void 	CORE_TCPServer_Create(CORE_TCPServer *instance_ptr);
void 	CORE_TCPServer_Free(CORE_TCPServer *instance_ptr);

/*****************************************************************************************************************************/

#endif
