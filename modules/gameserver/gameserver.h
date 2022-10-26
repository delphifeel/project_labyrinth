#ifndef _GAMESERVER_H_
#define _GAMESERVER_H_

#include <array>
#include <map>
#include <memory>
#include "libs/core/core.h"
#include "modules/labyrinth/lab-session.h"
#include "modules/iosystem/iosystem.h"
#include "include/config.h"
#include "packet/packet.h"
#include "player-token.h"
#include "turn-state.h"


class GameServer
{
public:
    explicit GameServer(GameServer&) = delete;
    explicit GameServer() {};

    void     Start();
private:
    inline void _PrepareSessions();
    void _StartGame(const PlayerToken &token_arr, IOSystem::Stream io_stream);
    void _OnInputRead(IOSystem::Stream io_stream, const uint8 data[], uint data_len);

    bool _NewPayloadFromTurnInfo(uint player_id, uint session_index, uint8 payload[], uint *payload_size);
    void _SendTurnInfo(const PlayerToken& token, const PlayerTokenRecord& record, bool as_start_game);

    bool _RegisterPlayerTurn(const PacketIn& packet_in, IOSystem::Stream io_stream);
    bool _ProcessSpecificTurn(const PlayerTokenRecord& record, const TurnState &turn_state);
    void _ProcessRegisteredTurns();

    LabSession* _GetSessionByIndex(uint index) const;
private:
    std::array<LabSession *, SESSIONS_CAPACITY>         m_sessions;
    IOSystem                                            m_io_system;
    std::map<const PlayerToken, PlayerTokenRecord>      m_tokens_holder;
    std::map<const PlayerToken, TurnState>              m_registered_turns;
};

#endif