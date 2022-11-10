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
    // uint32       chunk_size             = 0;
    // const uint8 *data_ptr               = data;
    // const uint8 *data_end               = data + data_len;
    IOSystem    *iosystem               = (IOSystem *) context;

    // TODO: split chunks if needed
    iosystem->m_on_read((IOSystem::Stream) client_connection, data, data_len);
}

void _OnTimer(void *context)
{
    IOSystem *iosystem = (IOSystem *) context;
    iosystem->m_on_timer();
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
    CORE_AssertPointer(m_tcp_server);
    CORE_AssertPointer(m_on_read);

    TCPServer_Start(m_tcp_server);
}

void IOSystem::Setup(uint8 data_start_flag, OnReadFunc on_read, TimerFunc on_timer, uint timer_ms)
{
    m_data_start_flag   = data_start_flag;
    m_on_read           = on_read;
    m_on_timer          = on_timer;

    m_tcp_server        = TCPServer_Create();
    TCPServer_OnError(m_tcp_server, _TCPServerOnError);
    TCPServer_OnNewConnection(m_tcp_server, _TCPServerOnNewConnection);
    TCPServer_OnCloseConnection(m_tcp_server, _TCPServerOnCloseConnection);
    TCPServer_SetContext(m_tcp_server, this);
    TCPServer_OnRead(m_tcp_server, _OnRead);
    TCPServer_EnableTimer(m_tcp_server, _OnTimer, timer_ms);
    TCPServer_Setup(m_tcp_server, IOSYSTEM_DEFAULT_PORT);
}

IOSystem::~IOSystem()
{
    TCPServer_Free(m_tcp_server);
}

/*****************************************************************************************************************************/