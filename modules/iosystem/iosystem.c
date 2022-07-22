#include "modules/iosystem/iosystem.h"
#include "libs/tcp/tcp-server.h"
#include "config.h"


/*****************************************************************************************************************************/

#define CONNECTIONS_PER_SESSION  (SESSION_PLAYERS_COUNT)

/*****************************************************************************************************************************/

typedef struct IOSystem_s
{
    IOSystemOnReadFunc                     on_read;
    TCPServer                              *tcp_server;

    // array of [session index] [player index]
    TCPServer_ClientConnection   tcp_clients_map[SESSIONS_CAPACITY][CONNECTIONS_PER_SESSION];
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
    IOSystem *ioSystem = (IOSystem *) context;
    ioSystem->on_read(data, data_len);
}

/*****************************************************************************************************************************/

bool IOSystem_Start(IOSystem *ioSystem)
{
    CORE_AssertPointer(ioSystem->on_read);

    TCPServer_Start(ioSystem->tcp_server);
    return true;
}

IOSystem *IOSystem_Create(void)
{
    IOSystem *ioSystem = CORE_MemAlloc(sizeof(IOSystem), 1);

    CORE_MemZero(&ioSystem->tcp_clients_map, sizeof(ioSystem->tcp_clients_map));

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
    TCPServer_Free(ioSystem->tcp_server);

    CORE_MemFree(ioSystem);
}

/*****************************************************************************************************************************/