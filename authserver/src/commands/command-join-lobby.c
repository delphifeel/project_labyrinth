#include "CCORE.h"
#include "authserver/config.h"
#include "lib/commands-processor/command.h"
#include "lib/tcp/tcp-client.h"
#include "authserver/auth-command-types.h"
#include "gameserver/gameserver-command-types.h"

typedef struct RequestPayload
{
    uint8       token[TOKEN_SIZE];
} RequestPayload;

enum
{
    STATUS_ERROR,
    STATUS_JOINED_LOBBY,
    STATUS_LOBBY_COMPLETED,
};

typedef struct ResponsePayload
{
    uint32                      status_code;
    uint32                      player_id;
    uint32                      player_index;
    uint32                      session_index;
} ResponsePayload;

typedef struct StartGamePayload 
{
    struct 
    {
        uint32  player_id;
        uint8   player_token[TOKEN_SIZE];   
    } players[LOBBY_USERS_COUNT];
} StartGamePayload;

static uint32              _users_in_lobby         = 0;
static StartGamePayload    _start_game_payload;
static ResponsePayload     _response_payload;

static void _GotRespFromGS(TCPClient *tcp_client, void *context, const uint8 data[], uint32 data_size)
{
    TCPClient_Disconnect(tcp_client);
    TCPClient_Free(tcp_client);
}

static void _SendStartGameToGS(TCPClient *tcp_client, void *context)
{
    uint8 buffer[2048];
    uint8 *buffer_ptr;


    buffer_ptr = buffer;

    // validation header
    *((uint32 *) buffer_ptr) = 0xDEADBEAF;
    buffer_ptr += 4;

    // command type
    *((uint32 *) buffer_ptr) = kCommandType_StartGame;
    buffer_ptr += 4;

    // session index
    *((uint32 *) buffer_ptr) = 0;
    buffer_ptr += 4;

    // player index
    *((uint32 *) buffer_ptr) = 0;
    buffer_ptr += 4;

    // player token
    memset(buffer_ptr, 0, 32);
    buffer_ptr += 32;

    // payload size
    *((uint32 *) buffer_ptr) = sizeof(_start_game_payload);
    buffer_ptr += 4;

    // payload
    memcpy(buffer_ptr, &_start_game_payload, sizeof(_start_game_payload));
    buffer_ptr += sizeof(_start_game_payload);

    TCPClient_Write(tcp_client, (const uint8 *) buffer, buffer_ptr - buffer);
}

static void _ProcessFullLobby()
{
    TCPClient *tcp_client = TCPClient_Create();


    CORE_DebugInfo("Lobby is full. Send `StartGame` to game server\n");
    TCPClient_OnConnected(tcp_client, _SendStartGameToGS);
    TCPClient_OnRead(tcp_client, _GotRespFromGS);

    TCPClient_Connect(tcp_client, GAMESERVER_IP_ADDRESS, GAMESERVER_PORT);
}

static void _AddPlayerToLobby(const RequestPayload *payload)
{
    uint32 player_id = _users_in_lobby + 1;

    _start_game_payload.players[_users_in_lobby].player_id = player_id;
    memcpy(
        _start_game_payload.players[_users_in_lobby].player_token,
        payload->token,
        sizeof(_start_game_payload.players[_users_in_lobby].player_token)
    );
    _users_in_lobby++;
    CORE_DebugInfo("Player joined lobby. Player id: %d\n", player_id);

    _response_payload.player_id = player_id;
    _response_payload.player_index = player_id - 1;
    _response_payload.session_index = 0;
}

bool CommandJoinLobby_Process(struct Command 	*command, 
                                   struct Command 	*out_response_command, 
                                   bool     	*out_is_have_response)
{
    const RequestPayload          *payload;
    const uint8                   *payload_raw;
    uint32                        payload_size; 
    uint32                        status;

    Command_GetPayloadPtr(command, &payload_raw, &payload_size); 

    if (payload_size != sizeof(RequestPayload))
    {
        CORE_DebugError("payload_size != sizeof(RequestPayload)\n");
        return false; 
    }

    payload = (const RequestPayload *) payload_raw; 

    do 
    {
        if (memcmp(payload->token, mocked_token, TOKEN_SIZE) != 0)
        {
            CORE_DebugError("Invalid token\n");
            status = STATUS_ERROR;
            break;
        }

        _AddPlayerToLobby(payload);

        if (_users_in_lobby == LOBBY_USERS_COUNT)
        {
            _users_in_lobby = 0;
            status = STATUS_LOBBY_COMPLETED;
            _ProcessFullLobby();
            break;
        }

        status = STATUS_JOINED_LOBBY;
    } while (0);

    *out_is_have_response = true;
    Command_SetType(out_response_command, kCommandResponseType_JoinLobby);
    _response_payload.status_code = status;
    if (Command_SetPayload(out_response_command, 
                           (const uint8 *) &_response_payload, 
                           sizeof(_response_payload)) == false)
    {
        return false;
    }

    return true; 
}
