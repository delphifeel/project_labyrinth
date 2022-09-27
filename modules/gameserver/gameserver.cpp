#include <cstring>
#include "modules/packet-processor/packet-processor.h"
#include "modules/packet-processor/packet-type.h"
#include "libs/packet/packet.h"
#include "libs/utils/bytes-to-number.h"
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

constexpr uint _BUFFER_MIN_SIZE = 40;
constexpr auto PLAYER_TOKEN_SIZE = kPlayerTokenSize;

static bool _BufferToPacket(const uint8    buffer[], 
                            uint           buffer_len, 
                            const uint8  **token_ptr,
                            Packet        *packet)
{
    CORE_AssertPointer(buffer);
    CORE_AssertPointer(packet);
    CORE_AssertPointer(token_ptr);

    if (buffer_len < _BUFFER_MIN_SIZE) {
        CORE_DebugError("Buffer is too small\n");
        return false;
    }

    const uint8 *buffer_ptr = buffer;

    // 0...4   (4 bytes)  - validation header
    uint32 header_value = 0;
    BytesToNumber(buffer_ptr, sizeof(header_value), header_value);
    if (header_value != _VALIDATION_HEADER) {
        CORE_DebugError("Wrong validation header\n");
        return false;
    }
    buffer_ptr += 4;

    // 4...8   (4 bytes)  - type
    uint32 type_value = 0;
    BytesToNumber(buffer_ptr, sizeof(type_value), type_value);
    packet->Type = type_value;
    buffer_ptr += 4;

    // 4...40   (32 bytes) - player token
    *token_ptr = buffer_ptr;
    buffer_ptr += PLAYER_TOKEN_SIZE;

    // 40...44   (4 bytes) - payload size
    uint32 payload_size_value = 0;
    BytesToNumber(buffer_ptr, sizeof(payload_size_value), payload_size_value);
    packet->PayloadSize = payload_size_value;
    buffer_ptr += 4;

    // 44...~   (~ bytes) - payload
    packet->Payload = buffer_ptr;
    buffer_ptr += payload_size_value;
    return true;
}

static void _PacketToBuffer(const Packet *packet, 
                            Status        status,
                            const uint8   token[PLAYER_TOKEN_SIZE],
                            uint8         buffer[], 
                            uint          buffer_size, 
                            uint         *buffer_len)
{
    CORE_AssertPointer(packet);
    CORE_AssertPointer(buffer);
    CORE_AssertPointer(buffer_len);

    uint8       *buffer_ptr = buffer;

    // 0...4   (4 bytes)  - validation header
    const uint32 validation_header_temp = _VALIDATION_HEADER;
    memcpy(buffer_ptr, &validation_header_temp, 4);
    buffer_ptr += 4;

    // 4...8   (4 bytes)  - status
    uint32 status_uint = (uint32) status;
    memcpy(buffer_ptr, &status_uint, 4);
    buffer_ptr += 4;

    // 8...12   (4 bytes)  - type
    memcpy(buffer_ptr, &packet->Type, 4);
    buffer_ptr += 4;

    // 12...44   (32 bytes) - player token
    memcpy(buffer_ptr, token, PLAYER_TOKEN_SIZE);
    buffer_ptr += 32;

    // 44...48   (4 bytes) - payload size
    memcpy(buffer_ptr, &packet->PayloadSize, 4);
    buffer_ptr += 4;

    // 48...~   (~ bytes) - payload
    if (packet->PayloadSize > 0) {
        memcpy(buffer_ptr, packet->Payload, packet->PayloadSize);
        buffer_ptr += packet->PayloadSize;
    }

    *buffer_len = buffer_ptr - buffer;
    CORE_Assert(*buffer_len <= buffer_size);
}

bool _ProcessNewPlayerJoined(GameServer *gameserver, uint *player_id, uint *session_index, bool *session_ready)
{
    LabSession *session = nullptr;
    for ( uint i = 0; i < gameserver->m_sessions.size(); i++ ) {
        if ( gameserver->m_sessions[i] == nullptr ) {
            gameserver->m_sessions[i] = new LabSession();
            session = gameserver->m_sessions[i];
            session->Setup(SESSION_PLAYERS_COUNT);
            *session_index = i;
            CORE_DebugInfo("Found free session (index %u)\n", i);
            break;
        }
        if ( (gameserver->m_sessions[i]) && (!gameserver->m_sessions[i]->IsFull()) ) {
            break;
        }
    }

    if (session == nullptr) {
        CORE_DebugError("No free session.\n");
        return false;
    }

    const auto [ id, player_added ] = session->AddPlayer();
    if (!player_added) {
        CORE_DebugError("Can't add new player\n");
        return false;
    }
    *player_id = id;
    if ( session->IsReadyForStart() ) {
        *session_ready = true;
        session->Start();
    }
    return true;
}

void _ProcessJoinLobby(GameServer *gameserver, PlayerToken &token_arr, IOSystem::Stream io_stream)
{
    if ( gameserver->m_tokens_holder.count(token_arr) > 0 ) {
        CORE_DebugError("Token already registered\n");
        return;
    }

    TokenRecord record;
    record.IOStream = io_stream;
    bool session_ready = false;
    if ( !_ProcessNewPlayerJoined(gameserver, &record.PlayerId, &record.SessionIndex, &session_ready) ) {
        return;
    }
    record.Token = token_arr;
    const auto [ _, token_added ] = gameserver->m_tokens_holder.insert( { token_arr, record } );
    if ( !token_added ) {
        CORE_DebugError("Add token error\n");
    }

    if ( session_ready ) {
        for ( const auto& [ _, i_record ] : gameserver->m_tokens_holder ) {
            Packet packet;
            packet.PayloadSize = 0;
            packet.Type = PacketType::JoinLobby;
            uint8 data_out[1024];
            uint  data_out_len = 0;
            _PacketToBuffer(&packet, Status::Ok, token_arr.data(), data_out, sizeof(data_out), &data_out_len);
            gameserver->m_io_system.Write( i_record.IOStream, data_out, data_out_len );
        }
    }
}

void _TokenPtrToArr(const uint8 token_ptr[], PlayerToken &arr)
{
    for (uint i = 0; i < PLAYER_TOKEN_SIZE; i++) {
        arr[i] = token_ptr[i];
    }
}

void _OnInputRead(GameServer *gameserver, IOSystem::Stream io_stream, const uint8 data[], uint data_len)
{
    uint32          type               = 0;
    const uint8    *token_ptr          = NULL;
    Packet          packet_in;

    // parsing data buffer for token and Packet
    if ( !_BufferToPacket(data, data_len, &token_ptr, &packet_in) ) {
        CORE_DebugError("data is not valid buffer\n");
        return;
    }

    PlayerToken token_as_arr;
    _TokenPtrToArr(token_ptr, token_as_arr);

    if (type == PacketType::JoinLobby) {
        _ProcessJoinLobby(gameserver, token_as_arr, io_stream);
        return;
    }

    // looking for SessionIndex, PlayerId
    const auto& it_record = gameserver->m_tokens_holder.find(token_as_arr);
    if ( it_record == gameserver->m_tokens_holder.end() ) {
        CORE_DebugError("Can't find registered token\n");
        return;
    }
    const auto& record = it_record->second;
    packet_in.SessionIndex  = record.SessionIndex;
    packet_in.PlayerId      = record.PlayerId;

    Packet packet_out;
    Status status = Status::Ok;
    switch ( gameserver->m_packet_processor.Process(packet_in, &packet_out) )
    {
    case PacketProcessor::BadInput :
        CORE_DebugError("Packet %u process error\n", packet_in.Type);
        return;
    case PacketProcessor::Error :
        status = Status::Error;
        break;
    case PacketProcessor::Ok :
        status = Status::Ok;
        break;
    default:
        CORE_DebugError("Unknown packet process status code\n");
        return;
    }

    // converting Packet and token to response data buffer
    uint8 data_out[1024];
    uint  data_out_len = 0;
    _PacketToBuffer(&packet_out, status, token_ptr, data_out, sizeof(data_out), &data_out_len);
    gameserver->m_io_system.Write(io_stream, data_out, data_len);
}

void GameServer::Start()
{
    m_io_system.Setup(
        _VALIDATION_HEADER, 
        [this](IOSystem::Stream io_stream, const uint8 data[], uint data_len) {
            _OnInputRead(this, io_stream, data, data_len);
        }
    );
    _PrepareSessions();
    m_packet_processor.Setup(m_sessions, m_io_system);

    m_io_system.Start();
}
