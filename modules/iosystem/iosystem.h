#ifndef _IOSYSTEM_H_
#define _IOSYSTEM_H_

#include <functional>
#include "libs/core/core.h"
#include "libs/tcp/tcp-server.h"


class IOSystem
{
public:
    explicit IOSystem(IOSystem&) = delete;

    using Stream = void *;
    using OnReadFunc = std::function< void(IOSystem::Stream ioStream, const uint8 data[], uint data_len) >;
    using TimerFunc = std::function< void() >;

    explicit IOSystem() :
        m_data_start_flag(0),
        m_on_read(nullptr),
        m_tcp_server(nullptr) {};

    void Setup(uint32 data_start_flag, OnReadFunc on_read, TimerFunc on_timer, uint timer_ms);
    void Start() const;
    void Write(IOSystem::Stream ioStream, const uint8 data[], uint data_len) const;

    ~IOSystem();
private:
    friend void _OnRead(TCPServer                      *tcp_server, 
                        void                           *context,
                        TCPServer_ClientConnection      client_connection,
                        const uint8                     data[], 
                        uint32                          data_len);
    friend void _OnTimer(void *context);
    
private:
    uint32                 m_data_start_flag;
    OnReadFunc             m_on_read;
    TimerFunc              m_on_timer;
    TCPServer             *m_tcp_server;
};

#endif