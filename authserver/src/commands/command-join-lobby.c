#include "CORE.h"
#include "authserver/CONFIG.h"
#include "lib/commands-processor/command.h"
#include "authserver/auth-command-types.h"
#include "gameserver/gameserver-command-types.h"

typedef struct JoinLobbyPayload
{
    uint8       token[TOKEN_SIZE];
} JoinLobbyPayload;

typedef struct JoinLobbyResponsePayload
{
    CORE_Bool   is_ok;
} JoinLobbyResponsePayload;

typedef struct StartGamePayload 
{
    struct 
    {
        uint32  player_id;
        uint8   player_token[TOKEN_SIZE];   
    } players[LOBBY_USERS_COUNT];
} StartGamePayload;

static uint32 _users_in_lobby = 0;

static void _SendData(CORE_TCPClient tcp_client, void *context)
{
    StartGamePayload payload;
    uint8 buffer[48 + sizeof(payload)];
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
    payload.players[0].player_id = 1;
    memcpy(payload.players[0].player_token, mocked_token, TOKEN_SIZE);

    payload.players[1].player_id = 2;
    memcpy(payload.players[1].player_token, mocked_token, TOKEN_SIZE);

    payload.players[2].player_id = 3;
    memcpy(payload.players[2].player_token, mocked_token, TOKEN_SIZE);

    payload.players[3].player_id = 4;
    memcpy(payload.players[3].player_token, mocked_token, TOKEN_SIZE);

    memcpy(buffer_ptr, &payload, sizeof(payload));

    CORE_TCPClient_Write(tcp_client, (const uint8 *) buffer, sizeof(buffer));
}

static void _CloseTCPClientConnection(CORE_TCPClient tcp_client, void *context)
{
    CORE_TCPClient_Disconnect(tcp_client);
    CORE_TCPClient_Free(&tcp_client);
}

static CORE_Bool _SendStartGameToGameServer()
{
    CORE_TCPClient tcp_client;


    CORE_TCPClient_Create(&tcp_client);
    CORE_TCPClient_OnConnected(tcp_client, _SendData);
    CORE_TCPClient_OnWrite(tcp_client, _CloseTCPClientConnection);

    CORE_TCPClient_Connect(tcp_client, GAMESERVER_IP_ADDRESS, GAMESERVER_PORT);

    return TRUE;
}

CORE_Bool CommandJoinLobby_Process(struct Command 	*command, 
                                   struct Command 	*out_response_command, 
                                   CORE_Bool     	*out_is_have_response)
{
    const JoinLobbyPayload          *payload;
    const uint8                     *payload_raw;
    uint32                          payload_size; 
    JoinLobbyResponsePayload        response_payload;
    CORE_Bool                       is_ok;

    Command_GetPayloadPtr(command, &payload_raw, &payload_size); 

    if (payload_size != sizeof(JoinLobbyPayload))
    {
        CORE_DebugError("payload_size != sizeof(JoinLobbyPayload)\n");
        return FALSE; 
    }

    payload = (const JoinLobbyPayload *) payload_raw; 

    is_ok = TRUE;

    if (memcmp(payload->token, mocked_token, TOKEN_SIZE) != 0)
    {
        CORE_DebugError("Invalid token\n");
        is_ok = FALSE;
    }

    *out_is_have_response = TRUE;
    Command_SetType(out_response_command, kCommandResponseType_JoinLobby);
    response_payload.is_ok = is_ok;
    if (Command_SetPayload( out_response_command, 
                            (const uint8 *) &response_payload, 
                            sizeof(JoinLobbyResponsePayload)) == FALSE)
    {
        return FALSE;
    }

    CORE_DebugInfo("Player joined lobby\n");
    _users_in_lobby++;


    if (_users_in_lobby == LOBBY_USERS_COUNT)
    {
        CORE_DebugInfo("Lobby is full. Send `StartGame` to game server\n");
        _users_in_lobby = 0;
        _SendStartGameToGameServer();
    }

    return TRUE; 
}
