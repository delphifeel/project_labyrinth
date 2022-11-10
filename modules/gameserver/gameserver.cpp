#include <cstring>
#include "gameserver.h"


/*****************************************************************************************************************************/

const std::string _VALIDATION_HEADER = "DEADBEE";
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

static uint _connectionAsBoolNumber(uint connection)
{
    return connection > 0 ? 1 : 0;
}

bool GameServer::_NewPayloadFromTurnInfo(uint player_id, uint session_index, json& json_data_out)
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

    auto& payload = json_data_out["Payload"];
    payload["TurnTimeMS"]  = (uint) kTurnTimeMs;
    payload["IsExit"]      = point->IsExit();
    payload["IsSpawn"]     = point->IsSpawn();

    const auto& connections = point->GetConnections();
    payload["Connections"]["Top"]      = connections.Top > 0;
    payload["Connections"]["Right"]    = connections.Right > 0;
    payload["Connections"]["Bottom"]   = connections.Bottom > 0;
    payload["Connections"]["Left"]     = connections.Left > 0;

    return true;
}

void GameServer::_SendTurnInfo(const PlayerToken& token, const PlayerTokenRecord& record, bool as_start_game)
{
    json json_data_out = json({});
    json_data_out["ValidationHeader"] = _VALIDATION_HEADER;
    json_data_out["Status"] = Status::Ok;
    json_data_out["Type"] = as_start_game ? PacketType::StartGame : PacketType::TurnInfo;
    json_data_out["Token"] = token;

    if ( !_NewPayloadFromTurnInfo(record.PlayerId, record.SessionIndex, json_data_out) ) {
        CORE_DebugError("Error creating payload from turn info\n");
        return;
    }
    const auto& json_str = json_data_out.dump();
    m_io_system.Write( record.IOStream, (const uint8*) json_str.c_str(), json_str.size() );
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

bool GameServer::_RegisterPlayerTurn(const json& json_data_in, IOSystem::Stream io_stream)
{
    TurnState   turn_state;
    turn_state.IsPlayerMoving       = (bool)            json_data_in["Payload"]["IsPlayerMoving"];
    turn_state.PlayerMoveDirection  = (MoveDirection)   json_data_in["Payload"]["PlayerMoveDirection"];
    const auto& token               =                   json_data_in["Token"];

    m_registered_turns.insert_or_assign(token, turn_state);

    auto json_data_out = json({});
    json_data_out["ValidationHeader"]   = _VALIDATION_HEADER;
    json_data_out["Status"]             = (uint) Status::Ok;
    json_data_out["Type"]               = PacketType::RegisterTurn;
    json_data_out["Token"]              = token;

    const auto& json_str = json_data_out.dump();
    m_io_system.Write( io_stream, (const uint8*) json_str.c_str(), json_str.size() );

    return true;
}

void GameServer::_OnInputRead(IOSystem::Stream io_stream, const uint8 data[], uint data_len)
{
    CORE_DebugInfo("data: %s\n", data);

    json json_data_in = json::parse(data);

    if ( json_data_in["ValidationHeader"] != _VALIDATION_HEADER ) {
        CORE_DebugError("Wrong validation header\n");
        return;
    }

    const auto& token       = json_data_in["Token"];
    const auto& packetType  = json_data_in["Type"];

    // FOR NOW, we adding just 1 player and start game
    if (packetType == PacketType::StartGame) {
        _StartGame(token, io_stream);
        return;
    }

    // looking for SessionIndex, PlayerId
    const auto& it_record = m_tokens_holder.find(token);
    if ( it_record == m_tokens_holder.end() ) {
        CORE_DebugError("Can't find registered token\n");
        return;
    }

    if ( ( packetType == PacketType::RegisterTurn ) &&
         ( !_RegisterPlayerTurn(json_data_in, io_stream) ) ) 
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
        0, 
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
