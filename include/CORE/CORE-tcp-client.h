#ifndef _CORE_TCP_CLIENT_H_
#define _CORE_TCP_CLIENT_H_

#include "CORE.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(CORE_TCPClient);

typedef void (*CORE_TCPClient_OnReadFunc)(CORE_TCPClient instance, void *context, const uint8 data[], uint32 data_size);
typedef void (*CORE_TCPClient_OnConnectedFunc)(CORE_TCPClient instance, void *context);
typedef void (*CORE_TCPClient_OnCloseConnectionFunc)(CORE_TCPClient instance, void *context);
typedef void (*CORE_TCPClient_OnWriteFunc)(CORE_TCPClient instance, void *context);

/*****************************************************************************************************************************/

void 	 	CORE_TCPClient_OnRead(CORE_TCPClient instance, CORE_TCPClient_OnReadFunc on_read);
void 	 	CORE_TCPClient_OnWrite(CORE_TCPClient instance, CORE_TCPClient_OnWriteFunc on_write);
void  		CORE_TCPClient_OnConnected(CORE_TCPClient instance, CORE_TCPClient_OnConnectedFunc on_connected);
void  		CORE_TCPClient_OnCloseConnection(CORE_TCPClient instance, CORE_TCPClient_OnCloseConnectionFunc on_close_connection);

void  		CORE_TCPClient_SetContext(CORE_TCPClient instance, void *context);
void 		CORE_TCPClient_Write(CORE_TCPClient instance, const uint8 data[], uint32 data_size);
CORE_Bool 	CORE_TCPClient_Connect(CORE_TCPClient instance, const char *dest_address, uint32 dest_port);
CORE_Bool 	CORE_TCPClient_Disconnect(CORE_TCPClient instance);

void 		CORE_TCPClient_Create(CORE_TCPClient *instance_ptr);
void 		CORE_TCPClient_Free(CORE_TCPClient *instance_ptr);

/*****************************************************************************************************************************/

#endif
