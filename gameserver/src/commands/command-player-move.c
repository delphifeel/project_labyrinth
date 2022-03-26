#include "lib/commands-processor/command.h"
#include "gameserver/player.h"
#include "lib/commands-processor/command.h"
#include "gameserver/gameserver-command-response.h"
#include "gameserver/gameserver-command.h"
#include "gameserver/gameserver-command-types.h"

typedef struct PlayerMovePayload 
{
    uint32  directions[2];
} PlayerMovePayload;

typedef struct PlayerMoveResponsePayload 
{
    CORE_Bool   is_ok;
    uint32      directions[2];
} PlayerMoveResponsePayload;

CORE_Bool CommandPlayerMove_Process(struct GameServerCommand            *game_server_command, 
                                    struct GameServerCommandResponse    *out_response_command,
                                    CORE_Bool                           *out_is_have_response)
{
    LabSession                  *sessions;
    uint32                      sessions_size;
    LabSession                  session;
    Player                      player;
    MoveDirection               directions[2];
    uint32                      directions_size;
    uint32                      session_index;
    uint32                      player_index;
    const PlayerMovePayload     *payload;
    const uint8                 *payload_raw;
    uint32                      payload_size;
    PlayerMoveResponsePayload   response_payload;



    GameServerCommand_GetSessionsPtr(game_server_command, &sessions, &sessions_size);
    GameServerCommand_GetSessionIndex(game_server_command, &session_index);
    GameServerCommand_GetPlayerIndex(game_server_command, &player_index);
    GameServerCommand_GetPayloadPtr(game_server_command, &payload_raw, &payload_size);

    if (payload_size != sizeof(PlayerMovePayload))
    {
        CORE_DebugError("payload_size != sizeof(PlayerMovePayload)\n");
        return FALSE;
    }

    payload = (const PlayerMovePayload *) payload_raw;

    if (LabSession_HelperFindSession(sessions, 
                                     sessions_size, 
                                     session_index,
                                     &session) == FALSE)
    {
        return FALSE;
    }
    CORE_DebugInfo("Found specific session\n");

    if (LabSession_FindPlayer(session, player_index, &player) == FALSE)
    {
        return FALSE;
    }
    CORE_DebugInfo("Found specific player\n");

    if (payload->directions[1] == 0)
    {
        directions[0] = payload->directions[0];
        directions_size = 1;
    }
    else
    {
        directions[0] = payload->directions[0];
        directions[1] = payload->directions[1];
        directions_size = 2;
    }

    if (Player_Move(player, directions, directions_size) == TRUE)
    {
        response_payload.is_ok = TRUE;
        memcpy(response_payload.directions, 
               payload->directions, 
               sizeof(response_payload.directions));
    }
    else
    {
        response_payload.is_ok = FALSE;
    }

    *out_is_have_response = TRUE;
    GameServerCommandResponse_SetType(out_response_command, kCommandType_PlayerMove);
    if (GameServerCommandResponse_SetPayload(out_response_command, 
                                            (const uint8 *) &response_payload,
                                            sizeof(response_payload)) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}
