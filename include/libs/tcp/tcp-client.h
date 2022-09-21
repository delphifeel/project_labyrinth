#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include "core/core.h"

/*****************************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TCPClient_s TCPClient;

typedef void (*TCPClient_OnReadFunc)(TCPClient *instance, void *context, const uint8 data[], uint32 data_size);
typedef void (*TCPClient_OnConnectedFunc)(TCPClient *instance, void *context);
typedef void (*TCPClient_OnCloseConnectionFunc)(TCPClient *instance, void *context);
typedef void (*TCPClient_OnWriteFunc)(TCPClient *instance, void *context);

/*****************************************************************************************************************************/

void        TCPClient_OnRead(TCPClient *instance, TCPClient_OnReadFunc on_read);
void        TCPClient_OnWrite(TCPClient *instance, TCPClient_OnWriteFunc on_write);
void        TCPClient_OnConnected(TCPClient *instance, TCPClient_OnConnectedFunc on_connected);
void        TCPClient_OnCloseConnection(TCPClient *instance, TCPClient_OnCloseConnectionFunc on_close_connection);

void        TCPClient_SetContext(TCPClient *instance, void *context);
void        TCPClient_Write(TCPClient *instance, const uint8 data[], uint32 data_size);
bool        TCPClient_Connect(TCPClient *instance, const char *dest_address, uint32 dest_port);
bool        TCPClient_Disconnect(TCPClient *instance);

TCPClient   *TCPClient_Create(void);
void        TCPClient_Free(TCPClient *instance);

#ifdef __cplusplus
}
#endif

/*****************************************************************************************************************************/

#endif