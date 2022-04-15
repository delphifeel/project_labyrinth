#include "lib/commands-processor/command.h"
#include "gameserver/player.h"
#include "lib/commands-processor/command.h"
#include "gameserver/gameserver-command-response.h"
#include "gameserver/gameserver-command.h"
#include "gameserver/gameserver-command-types.h"

typedef struct 
{
    PositionStruct  position;
    float32         speed;
} PlayerInitInfo;

typedef struct 
{
    uint32  dummy;
} RequestPayload;

typedef struct
{
    PlayerInitInfo  player_info;
    RoomInfo        room_info;
    uint32          room_size;
} ResponsePayload;

bool CommandPlayerInit_Process(struct GameServerCommand            *command, 
                               struct GameServerCommandResponse    *response_command,
                               bool                                *is_have_response)
{
    uint32                      session_index;
    uint32                      player_index;
    LabSession                  session;
    Player                      player;
    LabSession                  *sessions;
    uint32                      sessions_size;
    uint32                      payload_size;
    const uint8                 *payload_raw;   
    ResponsePayload             response_payload;
    LabPointStruct              position_point;


    GameServerCommand_GetSessionsPtr(command, &sessions, &sessions_size);
    GameServerCommand_GetSessionIndex(command, &session_index);
    GameServerCommand_GetPlayerIndex(command, &player_index);
    GameServerCommand_GetPayloadPtr(command, &payload_raw, &payload_size);

    if (payload_size != sizeof(RequestPayload)) {
        CORE_DebugError("payload_size != sizeof(RequestPayload)\n");
        return false;
    }
    if (!LabSession_HelperFindSession(sessions, 
                                     sessions_size, 
                                     session_index,
                                     &session)) {
        return false;
    }
    if (!LabSession_FindPlayer(session, player_index, &player)) {
        return false;
    }

    response_payload.player_info.speed = Player_GetSpeed(player);
    Player_GetPositionCoords(player, &response_payload.player_info.position);
    Player_GetPositionPoint(player, &position_point);

    LabPointsMap_HelperPointToRoomInfo(&position_point, &response_payload.room_info);

    response_payload.room_size = ROOM_SIZE;

    *is_have_response = true;
    GameServerCommandResponse_SetType(response_command, kCommandType_PlayerInit);
    GameServerCommandResponse_AddPlayerIndex(response_command, player_index);
    return GameServerCommandResponse_SetPayload(response_command, 
                                                (const uint8 *) &response_payload,
                                                sizeof(response_payload));
    
}