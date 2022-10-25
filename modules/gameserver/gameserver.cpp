#include <cstring>
#include "gameserver.h"

/*****************************************************************************************************************************/

constexpr uint _VALIDATION_HEADER = 0xDEADBEE;
constexpr uint kTurnTimeMs = 1 * 1000;

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
    auto session = _GetSessionByIndex(session_index);
    if (!session) {
        CORE_DebugError("Session is nullptr\n");
        return false;
    }
    const auto player = session->FindPlayer(player_id);
    if (!player) {
        CORE_DebugError("Player not found\n");
        return false;
    }
    const auto point = player->GetAssignedPoint();
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

    // turn time ms
    value_u32 = (uint32) kTurnTimeMs;
    memcpy(payload_ptr, &value_u32, sizeof(value_u32));
    payload_ptr += sizeof(value_u32);


    *payload_size = payload_ptr - payload;
    return true;
}

void GameServer::_SendTurnInfo(const PlayerToken& token, const PlayerTokenRecord& record, bool as_start_game)
{
    PacketOut packet;
    packet.ValidationHeader = _VALIDATION_HEADER;
    packet.Status = (uint) Status::Ok;
    packet.Type = as_start_game ? PacketType::StartGame : PacketType::TurnInfo;
    packet.Token = token;
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
    m_io_system.Write( record.IOStream, data_out, data_out_len );
}

void GameServer::_StartGame(const PlayerToken &token, IOSystem::Stream io_stream)
{
    if ( m_tokens_holder.count(token) > 0 ) {
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
    PlayerTokenRecord record;
    record.IOStream     = io_stream;
    record.PlayerId     = player_id;
    record.SessionIndex = 0;

    const auto [ _, token_added ] = m_tokens_holder.insert( { token, record } );
    if ( !token_added ) {
        CORE_DebugError("Add token error\n");
        return;
    }
    
    _SendTurnInfo(token, record, true);
}

bool GameServer::_RegisterPlayerTurn(const PacketIn& packet_in, IOSystem::Stream io_stream)
{
    const uint32 *payload_u32_ptr = (uint32 *) packet_in.Payload;
    TurnState   turn_state;

    turn_state.IsPlayerMoving = (bool) *payload_u32_ptr;
    payload_u32_ptr++;

    turn_state.PlayerMoveDirection = (MoveDirection) *payload_u32_ptr;
    payload_u32_ptr++;

    uint expected_size = (const uint8 *) payload_u32_ptr - packet_in.Payload;
    if ( packet_in.PayloadSize < expected_size ) {
        CORE_DebugError("packet_in payload size < then expected (%d vs %d)\n", packet_in.PayloadSize, expected_size);
        return false;
    }
    m_registered_turns.insert_or_assign(packet_in.Token, turn_state);

    PacketOut packet;
    packet.ValidationHeader = _VALIDATION_HEADER;
    packet.Status = (uint) Status::Ok;
    packet.Type = PacketType::RegisterTurn;
    packet.Token = packet_in.Token;
    packet.PayloadSize = 0;
    packet.Payload = nullptr;
    uint8 data_out[1024];
    uint data_out_len = packet.ToBuffer(data_out, sizeof(data_out));
    m_io_system.Write( io_stream, data_out, data_out_len );

    return true;
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

    if ( ( packet_in.Type == PacketType::RegisterTurn ) &&
         ( !_RegisterPlayerTurn(packet_in, io_stream) ) ) 
    {
        CORE_DebugError("Register turn error\n");
        return;
    }
}

bool GameServer::_ProcessSpecificTurn(const PlayerTokenRecord& record, const TurnState &turn_state)
{
    if ( !turn_state.IsPlayerMoving ) {
        return true;
    }

    auto session = _GetSessionByIndex(record.SessionIndex);
    if (!session) {
        CORE_DebugError("Session is nullptr\n");
        return false;
    }
    auto player = session->FindPlayer(record.PlayerId);
    if (!player) {
        CORE_DebugError("Player not found\n");
        return false;
    }
    const auto point = player->GetAssignedPoint();
    if (!point) {
        CORE_DebugError("No point assigned to player\n");
        return false;
    }
    const auto& connections = point->GetConnections();

    uint new_point_id;
    switch (turn_state.PlayerMoveDirection) {
        case MoveDirection::Top:
            new_point_id = connections.Top;
            break;
        case MoveDirection::Right:
            new_point_id = connections.Right;
            break;
        case MoveDirection::Bottom:
            new_point_id = connections.Bottom;
            break;
        case MoveDirection::Left:
            new_point_id = connections.Left;
            break;
    }

    auto& lab_map = session->GetLabMap();
    const auto new_point = lab_map.GetPointByID(new_point_id);
    if ( !new_point ) {
        CORE_DebugError("Point is nullptr\n");
        return false;
    }

    player->MoveTo(*new_point);
    return true;
}

void GameServer::_ProcessRegisteredTurns()
{ 
    for (const auto& [ token, turn_state ] : m_registered_turns) {
        const auto& it_record = m_tokens_holder.find(token);
        if ( it_record == m_tokens_holder.end() ) {
            CORE_DebugError("Can't find registered token\n");
            continue;
        }
        if ( !_ProcessSpecificTurn(it_record->second, turn_state) ) {
            continue;
        }
    }
    m_registered_turns.clear();

    for (const auto & [token, record] : m_tokens_holder) {
        _SendTurnInfo(token, record, false);
    }
}

LabSession* GameServer::_GetSessionByIndex(uint index) const
{
    if (index >= SESSIONS_CAPACITY) {
        CORE_DebugError("Session index out of bounds\n");
        return nullptr;
    }
    return m_sessions[index];
}

void GameServer::Start()
{
    _PrepareSessions();

    m_io_system.Setup(
        _VALIDATION_HEADER, 
        [this](IOSystem::Stream io_stream, const uint8 data[], uint data_len) {
            _OnInputRead(io_stream, data, data_len);
        },
        [this]() {
            _ProcessRegisteredTurns();
        },
        kTurnTimeMs
    );
    m_io_system.Start();
}
