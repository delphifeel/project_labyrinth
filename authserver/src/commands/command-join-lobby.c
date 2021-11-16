#include "CORE.h"
#include "authserver/CONFIG.h"
#include "lib/commands-processor/command.h"
#include "authserver/auth-command-types.h"

typedef struct JoinLobbyPayload
{
    uint8       token[TOKEN_SIZE];
} JoinLobbyPayload;

typedef struct JoinLobbyResponsePayload
{
    CORE_Bool   is_ok;
} JoinLobbyResponsePayload;

static uint32 _users_in_lobby = 0;

static CORE_Bool _SendStartGameToGameServer()
{
    
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

    _users_in_lobby++;


    if (_users_in_lobby == LOBBY_USERS_COUNT)
    {
        CORE_DebugInfo("Lobby is full. Send `StartGame` to game server\n");
        _users_in_lobby = 0;
        _SendStartGameToGameServer();
    }

    return TRUE; 
}
