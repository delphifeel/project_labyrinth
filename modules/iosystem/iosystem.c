#include "modules/iosystem/iosystem.h"
#include "modules/player/player-token.h"
#include "libs/tcp/tcp-server.h"
#include "libs/utils/chunk-splitter.h"
#include "config.h"


/*****************************************************************************************************************************/

#define _CONNECTIONS_PER_SESSION  (SESSION_PLAYERS_COUNT)

/*****************************************************************************************************************************/

typedef struct IOSystem_s
{
    uint32                 data_start_flag;
    IOSystemOnReadFunc     on_read;
    TCPServer             *tcp_server;
} IOSystem;

/*****************************************************************************************************************************/

static void _TCPServerOnError(TCPServer *tcp_server, void *context, const char *error_message)
{
    CORE_DebugError("TCP Server error: %s\n", error_message);
}

static void _TCPServerOnNewConnection(TCPServer *tcp_server, 
                                      void *context, 
                                      TCPServer_ClientConnection client_connection)
{
    // CORE_DebugInfo("TCP Server - new connection\n");
}

static void _TCPServerOnCloseConnection(TCPServer *tcp_server, 
                                        void *context, 
                                        TCPServer_ClientConnection client_connection)
{
    // CORE_DebugInfo("TCP Server - close connection\n");
}

static void _TCPServerOnRead(TCPServer                      *tcp_server, 
                             void                           *context, 
                             TCPServer_ClientConnection     client_connection,
                             const uint8                    data[], 
                             uint32                         data_len)
{
    IOSystem    *ioSystem               = (IOSystem *) context;
    uint32       chunk_size             = 0;
    const uint8 *data_ptr               = data;
    const uint8 *data_end               = data + data_len;
    const uint8 *chunk                  = NULL;
    uint32       data_bytes_left        = data_len;

    while (GetNextChunk(data_ptr, data_bytes_left, &chunk, &chunk_size, ioSystem->data_start_flag)) {
        data_ptr = chunk + chunk_size;
        data_bytes_left = data_end - data_ptr;

        ioSystem->on_read((IOStream) client_connection, chunk, chunk_size);
    }
}

/*****************************************************************************************************************************/

void IOSystem_OnRead(IOSystem *ioSystem, IOSystemOnReadFunc on_read)
{
    CORE_AssertPointer(ioSystem);
    CORE_AssertPointer(on_read);


    ioSystem->on_read = on_read;
}

void IOSystem_Write(const IOSystem *ioSystem, IOStream ioStream, const uint8 data[], uint data_len)
{
    CORE_AssertPointer(ioSystem);
    CORE_AssertPointer(ioStream);
    CORE_AssertPointer(data);
    CORE_Assert(data_len > 0);

    TCPServer_Write(ioSystem->tcp_server, ioStream, data, data_len);
}

bool IOSystem_Start(IOSystem *ioSystem)
{
    CORE_AssertPointer(ioSystem);
    CORE_AssertPointer(ioSystem->on_read);


    TCPServer_Start(ioSystem->tcp_server);
    return true;
}

IOSystem *IOSystem_Create(uint32 data_start_flag)
{
    IOSystem *ioSystem = CORE_MemAlloc(sizeof(IOSystem), 1);
    ioSystem->data_start_flag = data_start_flag;
    ioSystem->tcp_server = TCPServer_Create();
    TCPServer_OnError(ioSystem->tcp_server, _TCPServerOnError);
    TCPServer_OnNewConnection(ioSystem->tcp_server, _TCPServerOnNewConnection);
    TCPServer_OnCloseConnection(ioSystem->tcp_server, _TCPServerOnCloseConnection);
    TCPServer_OnRead(ioSystem->tcp_server, _TCPServerOnRead);
    TCPServer_SetContext(ioSystem->tcp_server, ioSystem);
    TCPServer_Setup(ioSystem->tcp_server, IOSYSTEM_DEFAULT_PORT);

    return ioSystem;
}

void IOSystem_Free(IOSystem *ioSystem)
{
    CORE_AssertPointer(ioSystem);

    
    TCPServer_Free(ioSystem->tcp_server);

    CORE_MemFree(ioSystem);
}

/*****************************************************************************************************************************/