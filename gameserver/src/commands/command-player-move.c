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
    uint32          is_ok;
    uint32          directions[2];
    PositionStruct  position_coords;
} PlayerMoveResponsePayload;

bool CommandPlayerMove_Process(struct GameServerCommand            *command, 
                               struct GameServerCommandResponse    *response_command,
                               bool                                *is_have_response)
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



    GameServerCommand_GetSessionsPtr(command, &sessions, &sessions_size);
    GameServerCommand_GetSessionIndex(command, &session_index);
    GameServerCommand_GetPlayerIndex(command, &player_index);
    GameServerCommand_GetPayloadPtr(command, &payload_raw, &payload_size);

    if (payload_size != sizeof(PlayerMovePayload))
    {
        CORE_DebugError("payload_size != sizeof(PlayerMovePayload)\n");
        return false;
    }

    payload = (const PlayerMovePayload *) payload_raw;

    if (LabSession_HelperFindSession(sessions, 
                                     sessions_size, 
                                     session_index,
                                     &session) == false)
    {
        return false;
    }

    if (LabSession_FindPlayer(session, player_index, &player) == false)
    {
        return false;
    }

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

    if (Player_Move(player, directions, directions_size))
    {
        response_payload.is_ok = 1;
        CORE_MemCpy(response_payload.directions, 
                    payload->directions, 
                    sizeof(response_payload.directions));
    }
    else
    {
        response_payload.is_ok = 0;
        CORE_MemZero(response_payload.directions, 
                    sizeof(response_payload.directions));
    }
    Player_GetPositionCoords(player, &response_payload.position_coords);

    *is_have_response = true;
    GameServerCommandResponse_SetType(response_command, kCommandType_PlayerMove);
    if (GameServerCommandResponse_SetPayload(response_command, 
                                            (const uint8 *) &response_payload,
                                            sizeof(response_payload)) == false)
    {
        return false;
    }

    return true;
}
