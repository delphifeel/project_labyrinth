#ifndef _GAMESERVER_H_
#define _GAMESERVER_H_

#include <array>
#include <map>
#include <memory>
#include "libs/core/core.h"
#include "modules/labyrinth/lab-session.h"
#include "modules/labyrinth/player/player-token.h"
#include "modules/iosystem/iosystem.h"
#include "modules/packet-processor/packet-processor.h"
#include "include/config.h"

class GameServer
{
public:
    void     Start();
private:
    inline void _PrepareSessions();
    void _ProcessJoinLobby(PlayerToken &token_arr, IOSystem::Stream io_stream);

    friend void _OnInputRead(GameServer *gameserver, IOSystem::Stream io_stream, const uint8 data[], uint data_len);
private:
    std::array<LabSession *, SESSIONS_CAPACITY> m_sessions;
    IOSystem                                    m_io_system;
    PacketProcessor                             m_packet_processor;
    std::map<const PlayerToken, TokenRecord>    m_tokens_holder;
};

#endif