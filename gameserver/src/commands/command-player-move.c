#include "lib/commands-processor/command.h"
#include "gameserver/player.h"
#include "lib/commands-processor/command.h"
#include "gameserver/gameserver-command-response.h"
#include "gameserver/gameserver-command.h"
#include "gameserver/gameserver-command-types.h"

typedef struct
{
    uint32  directions[2];
} RequestPayload;

typedef struct 
{
    uint32          is_ok;
    uint32          directions[2];
    PositionStruct  player_position;
    RoomInfo        room_info;
} ResponsePayload;

// TODO: pass session and player in function args 
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
    const RequestPayload        *payload;
    const uint8                 *payload_raw;
    uint32                      payload_size;
    ResponsePayload             response_payload;
    LabPointStruct              point;



    GameServerCommand_GetSessionsPtr(command, &sessions, &sessions_size);
    GameServerCommand_GetSessionIndex(command, &session_index);
    GameServerCommand_GetPlayerIndex(command, &player_index);
    GameServerCommand_GetPayloadPtr(command, &payload_raw, &payload_size);

    if (payload_size != sizeof(RequestPayload))
    {
        CORE_DebugError("payload_size != sizeof(RequestPayload)\n");
        return false;
    }

    payload = (const RequestPayload *) payload_raw;

    if (!LabSession_HelperFindSession(sessions, 
                                     sessions_size, 
                                     session_index,
                                     &session)) {
        return false;
    }

    if (!LabSession_FindPlayer(session, player_index, &player)) {
        return false;
    }

    if (payload->directions[1] == 0) {
        directions[0] = payload->directions[0];
        directions_size = 1;
    }
    else {
        directions[0] = payload->directions[0];
        directions[1] = payload->directions[1];
        directions_size = 2;
    }

    if (Player_Move(player, directions, directions_size)) {
        response_payload.is_ok = 1;
        CORE_MemCpy(response_payload.directions, 
                    payload->directions, 
                    sizeof(response_payload.directions));
    }
    else {
        response_payload.is_ok = 0;
        CORE_MemZero(response_payload.directions, 
                    sizeof(response_payload.directions));
    }
    Player_GetPositionCoords(player, &response_payload.player_position);
    Player_GetPositionPoint(player, &point);
    LabPointsMap_HelperPointToRoomInfo(&point, &response_payload.room_info);

    *is_have_response = true;
    GameServerCommandResponse_SetType(response_command, kCommandType_PlayerMove);
    return GameServerCommandResponse_SetPayload(response_command, 
                                                (const uint8 *) &response_payload,
                                                sizeof(response_payload));
}
