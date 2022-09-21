#ifndef _IOSYSTEM_H_
#define _IOSYSTEM_H_

#include <functional>
#include "libs/core/core.h"
#include "libs/packet/packet.h"
#include "libs/tcp/tcp-server.h"


class IOSystem
{
public:
    using Stream = void *;
    using OnReadFunc = std::function< void(IOSystem::Stream ioStream, const uint8 data[], uint data_len) >;

    void Start() const;
    void Write(IOSystem::Stream ioStream, const uint8 data[], uint data_len) const;

    IOSystem(uint32 data_start_flag, OnReadFunc on_read);
    ~IOSystem();

    friend void _OnRead(TCPServer                      *tcp_server, 
                void *context,
                 TCPServer_ClientConnection      client_connection,
                 const uint8                     data[], 
                 uint32                          data_len);
private:
    uint32                 m_data_start_flag;
    OnReadFunc             m_on_read;
    TCPServer             *m_tcp_server;
};

#endif