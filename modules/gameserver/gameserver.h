#ifndef _GAMESERVER_H_
#define _GAMESERVER_H_

#include <array>
#include <map>
#include <memory>
#include "libs/core/core.h"
#include "modules/labyrinth/lab-session.h"
#include "modules/iosystem/iosystem.h"
#include "include/config.h"
#include "player-token.h"


class GameServer
{
public:
    void     Start();
private:
    inline void _PrepareSessions();
    void _StartGame(const PlayerToken &token_arr, IOSystem::Stream io_stream);
    void _OnInputRead(IOSystem::Stream io_stream, const uint8 data[], uint data_len);
    bool _NewPayloadFromTurnInfo(uint player_id, uint session_index, uint8 payload[], uint *payload_size);
private:
    std::array<LabSession *, SESSIONS_CAPACITY> m_sessions;
    IOSystem                                    m_io_system;
    std::map<const PlayerToken, TokenRecord>    m_tokens_holder;
};

#endif