#include "modules/labyrinth/player/player-token.h"
#include "libs/tcp/tcp-server.h"
#include "libs/utils/chunk-splitter.h"
#include "include/config.h"

#include "iosystem.h"

/*****************************************************************************************************************************/

void _TCPServerOnError(TCPServer *tcp_server, void *context, const char *error_message)
{
    CORE_DebugError("TCP Server error: %s\n", error_message);
}

void _TCPServerOnNewConnection(TCPServer *tcp_server, 
                                      void *context, 
                                      TCPServer_ClientConnection client_connection)
{
    // CORE_DebugInfo("TCP Server - new connection\n");
}

void _TCPServerOnCloseConnection(TCPServer *tcp_server, 
                                        void *context, 
                                        TCPServer_ClientConnection client_connection)
{
    // CORE_DebugInfo("TCP Server - close connection\n");
}

/*****************************************************************************************************************************/

void _OnRead(TCPServer                      *tcp_server, 
             void                           *context,
             TCPServer_ClientConnection      client_connection,
             const uint8                     data[], 
             uint32                          data_len) 

{
    uint32       chunk_size             = 0;
    const uint8 *data_ptr               = data;
    const uint8 *data_end               = data + data_len;
    const uint8 *chunk                  = NULL;
    uint32       data_bytes_left        = data_len;
    IOSystem    *iosystem               = (IOSystem *) context;

    while (GetNextChunk(data_ptr, data_bytes_left, &chunk, &chunk_size, iosystem->m_data_start_flag)) {
        data_ptr = chunk + chunk_size;
        data_bytes_left = data_end - data_ptr;

        iosystem->m_on_read((IOSystem::Stream) client_connection, chunk, chunk_size);
    }
}

void 
IOSystem::Write(IOSystem::Stream ioStream, const uint8 data[], uint data_len) const
{
    CORE_AssertPointer(ioStream);
    CORE_AssertPointer(data);
    CORE_Assert(data_len > 0);

    TCPServer_Write(m_tcp_server, ioStream, data, data_len);
}

void IOSystem::Start() const
{
    TCPServer_Start(m_tcp_server);
}

IOSystem::IOSystem(uint32 data_start_flag, OnReadFunc on_read)
{
    m_data_start_flag   = data_start_flag;
    m_on_read           = on_read;

    m_tcp_server        = TCPServer_Create();
    TCPServer_OnError(m_tcp_server, _TCPServerOnError);
    TCPServer_OnNewConnection(m_tcp_server, _TCPServerOnNewConnection);
    TCPServer_OnCloseConnection(m_tcp_server, _TCPServerOnCloseConnection);
    TCPServer_OnRead(m_tcp_server, _OnRead);
    TCPServer_Setup(m_tcp_server, IOSYSTEM_DEFAULT_PORT);
}

IOSystem::~IOSystem()
{
    TCPServer_Free(m_tcp_server);
}

/*****************************************************************************************************************************/