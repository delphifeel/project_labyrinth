#include <cstring>
#include "packet/packet.h"
#include "gameserver.h"

/*****************************************************************************************************************************/

constexpr uint _VALIDATION_HEADER = 0xDEADBEE;

enum class Status
{
    Ok = 0,
    Error,
};

/*****************************************************************************************************************************/

inline void GameServer::_PrepareSessions()
{
    for ( int i = 0; i < m_sessions.size(); i++ ) {
        m_sessions[i] = nullptr;
    }
}

bool GameServer::_NewPayloadFromTurnInfo(uint player_id, uint session_index, uint8 payload[], uint *payload_size)
{
    if (session_index >= SESSIONS_CAPACITY) {
        CORE_DebugError("Session index out of bounds\n");
        return false;
    }
    LabSession *session = m_sessions[session_index];
    if (!session) {
        CORE_DebugError("Session is nullptr\n");
        return false;
    }
    const Player *player = session->FindPlayer(player_id);
    if (!player) {
        CORE_DebugError("Player not found\n");
        return false;
    }
    const LabPoint *point = player->GetAssignedPoint();
    if (!point) {
        CORE_DebugError("No point assigned to player\n");
        return false;
    }

    // max bytes possible
    uint8 *payload_ptr = payload;
    uint32 value_u32;

    // is exit
    value_u32 = (uint32) point->IsExit();
    memcpy(payload_ptr, &value_u32, sizeof(value_u32));
    payload_ptr += sizeof(value_u32);

    // is spawn
    value_u32 = (uint32) point->IsSpawn();
    memcpy(payload_ptr, &value_u32, sizeof(value_u32));
    payload_ptr += sizeof(value_u32);

    // connections
    const auto& connections = point->GetConnections();

    value_u32 = connections.Top > 0;
    memcpy(payload_ptr, &value_u32, sizeof(value_u32));
    payload_ptr += sizeof(value_u32);

    value_u32 = connections.Right > 0;
    memcpy(payload_ptr, &value_u32, sizeof(value_u32));
    payload_ptr += sizeof(value_u32);

    value_u32 = connections.Bottom > 0;
    memcpy(payload_ptr, &value_u32, sizeof(value_u32));
    payload_ptr += sizeof(value_u32);

    value_u32 = connections.Left > 0;
    memcpy(payload_ptr, &value_u32, sizeof(value_u32));
    payload_ptr += sizeof(value_u32);

    *payload_size = payload_ptr - payload;
    return true;
}

void GameServer::_StartGame(const PlayerToken &token_arr, IOSystem::Stream io_stream)
{
    if ( m_tokens_holder.count(token_arr) > 0 ) {
        CORE_DebugError("Token already registered\n");
        return;
    }

    // new session
    m_sessions[0] = new LabSession();
    LabSession *session = m_sessions[0];
    session->Setup(1);
    const auto [ player_id, player_added ] = session->AddPlayer();
    if (!player_added) {
        CORE_DebugError("Can't add new player\n");
        return;
    }
    if ( !session->IsReadyForStart() ) {
        CORE_DebugError("Session is not ready idk why\n");
        return;
    }
    session->Start();

    // save token record
    TokenRecord record;
    record.IOStream     = io_stream;
    record.PlayerId     = player_id;
    record.SessionIndex = 0;

    const auto [ _, token_added ] = m_tokens_holder.insert( { token_arr, record } );
    if ( !token_added ) {
        CORE_DebugError("Add token error\n");
        return;
    }

    // send start game with turn info
    {
        PacketOut packet;
        packet.ValidationHeader = _VALIDATION_HEADER;
        packet.Status = (uint) Status::Ok;
        packet.Type = PacketType::StartGame;
        packet.Token = token_arr;
        uint8 payload[900];
        uint payload_len = 0;
        if ( !_NewPayloadFromTurnInfo(record.PlayerId, record.SessionIndex, payload, &payload_len) ) {
            CORE_DebugError("Error creating payload from turn info\n");
            return;
        }
        CORE_Assert(payload_len <= sizeof(payload));
        packet.PayloadSize = payload_len;
        packet.Payload = payload;

        uint8 data_out[1024];
        uint data_out_len = packet.ToBuffer(data_out, sizeof(data_out));
        
        m_io_system.Write( io_stream, data_out, data_out_len );
    }
}

void GameServer::_OnInputRead(IOSystem::Stream io_stream, const uint8 data[], uint data_len)
{
    const uint8    *token_ptr          = NULL;
    PacketIn        packet_in;

    // parsing data buffer for token and Packet
    if ( !packet_in.FromBuffer(data, data_len) ) {
        CORE_DebugError("data is not valid buffer\n");
        return;
    }
    if ( packet_in.ValidationHeader != _VALIDATION_HEADER ) {
        CORE_DebugError("Wrong validation header\n");
        return;
    }

    // FOR NOW, we adding just 1 player and start game
    if (packet_in.Type == PacketType::StartGame) {
        _StartGame(packet_in.Token, io_stream);
        return;
    }

    // looking for SessionIndex, PlayerId
    const auto& it_record = m_tokens_holder.find(packet_in.Token);
    if ( it_record == m_tokens_holder.end() ) {
        CORE_DebugError("Can't find registered token\n");
        return;
    }
    // const auto& record      = it_record->second;
    // packet_in.SessionIndex  = record.SessionIndex;
    // packet_in.PlayerId      = record.PlayerId;

   // 
}

void GameServer::Start()
{
    _PrepareSessions();

    m_io_system.Setup(
        _VALIDATION_HEADER, 
        [this](IOSystem::Stream io_stream, const uint8 data[], uint data_len) {
            _OnInputRead(io_stream, data, data_len);
        }
    );
    m_io_system.Start();
}
