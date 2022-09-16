#include "CCORE.h"
#include "modules/labyrinth/lab-session.h"
#include "modules/iosystem/iosystem.h"
#include "modules/packet-processor/packet-processor.h"
#include "modules/packet-processor/packet-type.h"
#include "libs/packet/packet.h"
#include "libs/tokens-holder/tokens-holder.h"
#include "libs/utils/bytes-to-number.h"
#include "config.h"

/*****************************************************************************************************************************/

#define _VALIDATION_HEADER        (0xDEADBEE)

typedef enum Status
{
    kStatus_Ok = 0, 
    kStatus_Error, 
} Status;

/*****************************************************************************************************************************/

typedef struct GameServer_s
{
    LabSession              *sessions[SESSIONS_CAPACITY];
    IOSystem                *io_system;
    PacketProcessor         *packet_processor;
    TokensHolder            *tokens_holder;
} GameServer;

static GameServer *_gameserver;

/*****************************************************************************************************************************/

static inline void _PrepareSessions(void)
{
    CORE_MemZero(_gameserver->sessions, sizeof(_gameserver->sessions));
}

static inline void _FreeSessions(void)
{
    for (uint i = 0; i < SESSIONS_CAPACITY; i++) {
        if (_gameserver->sessions[i] != NULL) {
            LabSession_Free(_gameserver->sessions[i]);
        }
    }
}

static inline uint _GetSessionsLen(void)
{
    return sizeof(_gameserver->sessions) / sizeof(*_gameserver->sessions);
}

#define _BUFFER_MIN_SIZE    (40)

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
    CORE_MemCpy(buffer_ptr, &validation_header_temp, 4);
    buffer_ptr += 4;

    // 4...8   (4 bytes)  - status
    uint32 status_uint = status;
    CORE_MemCpy(buffer_ptr, &status_uint, 4);
    buffer_ptr += 4;

    // 8...12   (4 bytes)  - type
    CORE_MemCpy(buffer_ptr, &packet->Type, 4);
    buffer_ptr += 4;

    // 12...44   (32 bytes) - player token
    CORE_MemCpy(buffer_ptr, token, PLAYER_TOKEN_SIZE);
    buffer_ptr += 32;

    // 44...48   (4 bytes) - payload size
    CORE_MemCpy(buffer_ptr, &packet->PayloadSize, 4);
    buffer_ptr += 4;

    // 48...~   (~ bytes) - payload
    if (packet->PayloadSize > 0) {
        CORE_MemCpy(buffer_ptr, packet->Payload, packet->PayloadSize);
        buffer_ptr += packet->PayloadSize;
    }

    *buffer_len = buffer_ptr - buffer;
    CORE_Assert(*buffer_len <= buffer_size);
}

static bool _ProcessNewPlayerJoined(uint *player_id, uint *session_index, bool *session_ready)
{
    LabSession *session = NULL;

    for (uint i = 0; i < _GetSessionsLen(); i++) {
        session = _gameserver->sessions[i];

        if (session == NULL) {
            session = LabSession_Create();
            LabSession_Setup(session, SESSION_PLAYERS_COUNT);
            _gameserver->sessions[i] = session;
            *session_index = i;
            CORE_DebugInfo("Found free session (index %u)\n", i);
            break;
        }
        if ((session != NULL) && (!LabSession_IsFull(session))) {
            break;
        }
    }

    if (session == NULL) {
        CORE_DebugError("No free session.\n");
        return false;
    }

    if (!LabSession_AddPlayer(session, player_id)) {
        CORE_DebugError("Can't add new player\n");
        return false;
    }
    if (LabSession_IsReadyForStart(session)) {
        *session_ready = true;
        LabSession_Start(session);
    }
    return true;
}

static void _ProcessJoinLobby(const uint8 *token_ptr, IOStream io_stream)
{
    if (TokensHolder_Exist(_gameserver->tokens_holder, token_ptr)) {
        CORE_DebugError("Token already registered\n");
        return;
    }

    TokensHolderRecord record = {
        .IOStream = io_stream,
    };
    bool session_ready = false;
    if (!_ProcessNewPlayerJoined(&record.PlayerId, &record.SessionIndex, &session_ready)) {
        return;
    }
    CORE_MemCpy(record.Token, token_ptr, sizeof(record.Token));
    if (!TokensHolder_Add(_gameserver->tokens_holder, &record)) {
        CORE_DebugError("Add token error\n");
    }

    if (session_ready) {
        for (uint i = 0; i < TokensHolder_Size(_gameserver->tokens_holder); i++) {
            const TokensHolderRecord *record_ptr = TokensHolder_Get(_gameserver->tokens_holder, i);
            Packet packet = {
                .Type = kPacket_JoinLobby,
                .PayloadSize = 0,
            };
            uint8 data_out[1024];
            uint  data_out_len = 0;
            _PacketToBuffer(&packet, kStatus_Ok, token_ptr, data_out, sizeof(data_out), &data_out_len);
            IOSystem_Write(_gameserver->io_system, record_ptr->IOStream, data_out, data_out_len);
        }
    }
}

static void _OnInputRead(IOStream io_stream, const uint8 data[], uint data_len)
{
    uint32          type               = 0;
    const uint8    *token_ptr          = NULL;
    Packet          packet_in;

    // parsing data buffer for token and Packet
    if (!_BufferToPacket(data, data_len, &token_ptr, &packet_in)) {
        CORE_DebugError("data is not valid buffer\n");
        return;
    }
    if (type == kPacket_JoinLobby) {
        _ProcessJoinLobby(token_ptr, io_stream);
        return;
    }

    // looking for SessionIndex, PlayerId
    const TokensHolderRecord *record = TokensHolder_Find(_gameserver->tokens_holder, token_ptr);
    if (record == NULL) {
        CORE_DebugError("Can't find registered token\n");
        return;
    }
    packet_in.SessionIndex  = record->SessionIndex;
    packet_in.PlayerId      = record->PlayerId;

    Packet packet_out;
    Status status = kStatus_Ok;
    switch (PacketProcessor_Process(_gameserver->packet_processor, &packet_in, &packet_out))
    {
    case kPacketProcessStatusCode_BadInput:
        CORE_DebugError("Packet %u process error\n", packet_in.Type);
        return;
    case kPacketProcessStatusCode_Error:
        status = kStatus_Error;
        break;
    case kPacketProcessStatusCode_Ok:
        status = kStatus_Ok;
        break;
    default:
        CORE_DebugError("Unknown packet process status code\n");
        return;
    }

    // converting Packet and token to response data buffer
    uint8 data_out[1024];
    uint  data_out_len = 0;
    _PacketToBuffer(&packet_out, status, token_ptr, data_out, sizeof(data_out), &data_out_len);
    IOSystem_Write(_gameserver->io_system, io_stream, data_out, data_len);
}

static void _StartGameServer(void)
{
    _gameserver = CORE_MemAlloc(sizeof(GameServer), 1);

    CORE_DebugInfo("Init sessions\n");
    _PrepareSessions();

    CORE_DebugInfo("Init tokens holder\n");
    _gameserver->tokens_holder = TokensHolder_Create(SESSIONS_CAPACITY * SESSION_PLAYERS_COUNT);

    CORE_DebugInfo("Init in-out system\n");
    _gameserver->io_system = IOSystem_Create(_VALIDATION_HEADER);
    IOSystem_OnRead(_gameserver->io_system, _OnInputRead);

    CORE_DebugInfo("Init input processor\n");
    _gameserver->packet_processor = PacketProcessor_Create(_gameserver->sessions, 
                                                           _GetSessionsLen(), 
                                                           _gameserver->io_system);

    IOSystem_Start(_gameserver->io_system);
}

static void _FreeGameServer(void)
{
    PacketProcessor_Free(_gameserver->packet_processor);
    IOSystem_Free(_gameserver->io_system);
    TokensHolder_Free(_gameserver->tokens_holder);
    _FreeSessions();

    CORE_MemFree(_gameserver);
}

/*****************************************************************************************************************************/

int main(void)
{
    _StartGameServer();
    return 0;
}