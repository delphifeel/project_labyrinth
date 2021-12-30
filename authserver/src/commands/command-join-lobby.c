#include <pthread.h>

#include "CORE.h"
#include "authserver/CONFIG.h"
#include "lib/commands-processor/command.h"
#include "authserver/auth-command-types.h"
#include "gameserver/gameserver-command-types.h"

typedef struct JoinLobbyPayload
{
    uint8       token[TOKEN_SIZE];
} JoinLobbyPayload;

typedef struct StartGameResponsePayload 
{
    struct 
    {
        uint32  player_id;
        uint32  player_index;   
        uint32  session_index;
    } players[LOBBY_USERS_COUNT];
} StartGameResponsePayload;

typedef struct JoinLobbyResponsePayload
{
    /**
     *  status code possible values:
     *      0 - error
     *      1 - joined lobby successfully
     *      2 - lobby is full. start game
     */
    uint32                      status_code;
    StartGameResponsePayload    structure;
} JoinLobbyResponsePayload;

typedef struct StartGamePayload 
{
    struct 
    {
        uint32  player_id;
        uint8   player_token[TOKEN_SIZE];   
    } players[LOBBY_USERS_COUNT];
} StartGamePayload;

static uint32                       _users_in_lobby         = 0;
static StartGamePayload             _start_game_payload;
static JoinLobbyResponsePayload     _response_payload;


static void _SetStartGameResponse(CORE_TCPClient tcp_client, void *context, const uint8 data[], uint32 data_size)
{
    if (data_size != 8 + sizeof(StartGameResponsePayload))
    {
        CORE_DebugError("payload size is small (Expected %lu, Got %u)\n", sizeof(StartGameResponsePayload), data_size - 8);
        return;
    }

    memcpy(&_response_payload.structure, data + 8, sizeof(_response_payload.structure));

    CORE_TCPClient_Disconnect(tcp_client);
    CORE_TCPClient_Free(&tcp_client);
}

static void _SendData(CORE_TCPClient tcp_client, void *context)
{
    uint8 buffer[48 + sizeof(_start_game_payload)];
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

    // payload
    memcpy(buffer_ptr, &_start_game_payload, sizeof(_start_game_payload));

    CORE_TCPClient_Write(tcp_client, (const uint8 *) buffer, sizeof(buffer));
}

static void _ProcessStartGame()
{
    CORE_TCPClient tcp_client;


    CORE_TCPClient_Create(&tcp_client);
    CORE_TCPClient_OnConnected(tcp_client, _SendData);
    CORE_TCPClient_OnRead(tcp_client, _SetStartGameResponse);

    CORE_TCPClient_Connect(tcp_client, GAMESERVER_IP_ADDRESS, GAMESERVER_PORT);
}

static void _AddPlayerToLobby(const JoinLobbyPayload *payload)
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
}

CORE_Bool CommandJoinLobby_Process(struct Command 	*command, 
                                   struct Command 	*out_response_command, 
                                   CORE_Bool     	*out_is_have_response)
{
    const JoinLobbyPayload          *payload;
    const uint8                     *payload_raw;
    uint32                          payload_size; 
    uint32                          status;

    Command_GetPayloadPtr(command, &payload_raw, &payload_size); 

    if (payload_size != sizeof(JoinLobbyPayload))
    {
        CORE_DebugError("payload_size != sizeof(JoinLobbyPayload)\n");
        return FALSE; 
    }

    payload = (const JoinLobbyPayload *) payload_raw; 

    do 
    {
        if (memcmp(payload->token, mocked_token, TOKEN_SIZE) != 0)
        {
            CORE_DebugError("Invalid token\n");
            status = 0;
            break;
        }

        _AddPlayerToLobby(payload);

        if (_users_in_lobby == LOBBY_USERS_COUNT)
        {
            status = 2;
            CORE_DebugInfo("Lobby is full. Send `StartGame` to game server\n");
            _users_in_lobby = 0;
            _ProcessStartGame();
            break;
        }

        status = 1;
    } while (0);

    *out_is_have_response = TRUE;
    Command_SetType(out_response_command, kCommandResponseType_JoinLobby);
    _response_payload.status_code = status;
    if (Command_SetPayload( out_response_command, 
                            (const uint8 *) &_response_payload, 
                            sizeof(_response_payload)) == FALSE)
    {
        return FALSE;
    }

    return TRUE; 
}
