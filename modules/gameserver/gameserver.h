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

    friend void _OnInputRead(GameServer *gameserver, IOSystem::Stream io_stream, const uint8 data[], uint data_len);
    friend bool _ProcessNewPlayerJoined(GameServer *gameserver, uint *player_id, uint *session_index, bool *session_ready);
    friend void _ProcessJoinLobby(GameServer *gameserver, PlayerToken &token_arr, IOSystem::Stream io_stream);

    std::array<LabSession *, SESSIONS_CAPACITY> m_sessions;
    std::unique_ptr<IOSystem>                   m_io_system;
    std::unique_ptr<PacketProcessor>            m_packet_processor;
    std::map<const PlayerToken, TokenRecord>    m_tokens_holder;
};

#endif