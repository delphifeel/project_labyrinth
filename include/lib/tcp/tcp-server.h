#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "CCORE.h"

/*****************************************************************************************************************************/

/*
 * 		TODO(delphifeel): Make possible to create several TCP server's.
 * 		Right now after you call `TCPServer_Start` we are bounded to 
 * 		infinite event loop so after that we can't start another TCP server. 
 */

typedef void *TCPServer_ClientConnection;

CORE_OBJECT_DEFINE(TCPServer);

typedef void (*OnReadFunc)(	TCPServer instance, void *context, TCPServer_ClientConnection client_connection, 
							const uint8 data[], uint32 data_size);
typedef void (*OnNewConnectionFunc)(TCPServer instance, void *context, TCPServer_ClientConnection client_connection);
typedef void (*OnCloseConnectionFunc)(TCPServer instance, void *context, TCPServer_ClientConnection client_connection);
typedef void (*OnErrorFunc)(TCPServer instance, void *context, const char *error_message);

/*****************************************************************************************************************************/

void  	TCPServer_OnRead(TCPServer instance, OnReadFunc on_read);
void  	TCPServer_OnError(TCPServer instance, OnErrorFunc on_error);
void  	TCPServer_OnNewConnection(TCPServer instance, OnNewConnectionFunc on_new_connection);
void  	TCPServer_OnCloseConnection(TCPServer instance, OnCloseConnectionFunc on_close_connection);

void    TCPServer_CloseConnection(TCPServer instance, TCPServer_ClientConnection client_connection);
void  	TCPServer_SetContext(TCPServer instance, void *context);
void 	TCPServer_Write(TCPServer instance, TCPServer_ClientConnection client_connection, 
							 const uint8 data[], uint32 data_size);
void  	TCPServer_Setup(TCPServer instance, uint32 port);
void 	TCPServer_Start(TCPServer instance);

void 	TCPServer_Create(TCPServer *instance_ptr);
void 	TCPServer_Free(TCPServer *instance_ptr);

/*****************************************************************************************************************************/

#endif
