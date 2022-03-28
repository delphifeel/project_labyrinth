#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include "CCORE.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(TCPClient);

typedef void (*TCPClient_OnReadFunc)(TCPClient instance, void *context, const uint8 data[], uint32 data_size);
typedef void (*TCPClient_OnConnectedFunc)(TCPClient instance, void *context);
typedef void (*TCPClient_OnCloseConnectionFunc)(TCPClient instance, void *context);
typedef void (*TCPClient_OnWriteFunc)(TCPClient instance, void *context);

/*****************************************************************************************************************************/

void 	 	TCPClient_OnRead(TCPClient instance, TCPClient_OnReadFunc on_read);
void 	 	TCPClient_OnWrite(TCPClient instance, TCPClient_OnWriteFunc on_write);
void  		TCPClient_OnConnected(TCPClient instance, TCPClient_OnConnectedFunc on_connected);
void  		TCPClient_OnCloseConnection(TCPClient instance, TCPClient_OnCloseConnectionFunc on_close_connection);

void  		TCPClient_SetContext(TCPClient instance, void *context);
void 		TCPClient_Write(TCPClient instance, const uint8 data[], uint32 data_size);
bool 	TCPClient_Connect(TCPClient instance, const char *dest_address, uint32 dest_port);
bool 	TCPClient_Disconnect(TCPClient instance);

void 		TCPClient_Create(TCPClient *instance_ptr);
void 		TCPClient_Free(TCPClient *instance_ptr);

/*****************************************************************************************************************************/

#endif
