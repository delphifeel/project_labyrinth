#include "modules/iosystem/iosystem.h"
#include "modules/player/player-token.h"
#include "libs/tcp/tcp-server.h"
#include "libs/utils/chunk-splitter.h"
#include "config.h"


/*****************************************************************************************************************************/

#define _CONNECTIONS_PER_SESSION  (SESSION_PLAYERS_COUNT)
#define _VALIDATION_HEADER        (0xDEADBEE)

/*****************************************************************************************************************************/

typedef struct IOSystem_s
{
    IOSystemOnReadFunc                     on_read;
    TCPServer                              *tcp_server;
    CMap                                   *token_to_connection_map;
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

static void _SaveConnectionForLaterUse(IOSystem                    *ioSystem, 
                                       const uint8                  token[PLAYER_TOKEN_SIZE], 
                                       TCPServer_ClientConnection   connection)
{
    CORE_AssertPointer(ioSystem);
    CORE_AssertPointer(token);
    CORE_AssertPointer(connection);


    TCPServer_ClientConnection old_connection = CMap_Get(ioSystem->token_to_connection_map, token, PLAYER_TOKEN_SIZE);
    if (old_connection) {
        // connection already exist
        return;
    }
    CMap_Set(ioSystem->token_to_connection_map, token, PLAYER_TOKEN_SIZE, connection);
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
    Packet      *packet                 = Packet_Create();

    while (GetNextChunk(data_ptr, data_bytes_left, &chunk, &chunk_size, _VALIDATION_HEADER)) {
        data_ptr = chunk + chunk_size;
        data_bytes_left = data_end - data_ptr;

        // remove header from chunk
        chunk += sizeof(uint32);
        chunk_size -= sizeof(uint32);

        if (!Packet_Init(packet, chunk, chunk_size)) {
            CORE_DebugInfo("Invalid chunk of size: %u\n", chunk_size);
            continue;
        }
        _SaveConnectionForLaterUse(ioSystem, Packet_GetPlayerToken(packet), client_connection);
        ioSystem->on_read(packet);
    }
    Packet_Free(packet);
}

/*****************************************************************************************************************************/

void IOSystem_OnRead(IOSystem *ioSystem, IOSystemOnReadFunc on_read)
{
    ioSystem->on_read = on_read;
}

bool IOSystem_Start(IOSystem *ioSystem)
{
    CORE_AssertPointer(ioSystem->on_read);

    TCPServer_Start(ioSystem->tcp_server);
    return true;
}

IOSystem *IOSystem_Create(void)
{
    IOSystem *ioSystem = CORE_MemAlloc(sizeof(IOSystem), 1);
    ioSystem->tcp_server = TCPServer_Create();
    TCPServer_OnError(ioSystem->tcp_server, _TCPServerOnError);
    TCPServer_OnNewConnection(ioSystem->tcp_server, _TCPServerOnNewConnection);
    TCPServer_OnCloseConnection(ioSystem->tcp_server, _TCPServerOnCloseConnection);
    TCPServer_OnRead(ioSystem->tcp_server, _TCPServerOnRead);
    TCPServer_SetContext(ioSystem->tcp_server, ioSystem);
    TCPServer_Setup(ioSystem->tcp_server, IOSYSTEM_DEFAULT_PORT);

    ioSystem->token_to_connection_map = CMap_Create(SESSIONS_CAPACITY * SESSION_PLAYERS_COUNT);

    return ioSystem;
}

void IOSystem_Free(IOSystem *ioSystem)
{
    TCPServer_Free(ioSystem->tcp_server);
    CMap_Free(&ioSystem->token_to_connection_map);

    CORE_MemFree(ioSystem);
}

/*****************************************************************************************************************************/