#include "lib/commands-processor/command.h"
#include "gameserver/player.h"
#include "lib/commands-processor/command.h"
#include "gameserver/gameserver-command-response.h"
#include "gameserver/gameserver-command.h"
#include "gameserver/gameserver-command-types.h"

typedef struct 
{
    uint32          has_top_connection; 
    uint32          has_right_connection; 
    uint32          has_bottom_connection; 
    uint32          has_left_connection; 
    uint32          is_exit; 
    uint32          is_spawn;
} PointInitInfo;

typedef struct RequestPayload 
{
    uint32  dummy;
} RequestPayload;

typedef struct ResponsePayload 
{
    PositionStruct  position_coords;
    PointInitInfo   point_init_info;
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

    Player_GetPositionCoords(player, &response_payload.position_coords);
    Player_GetPositionPoint(player, &position_point);

    // convert position point to point init info
    PointInitInfo *point_init_info_ptr          = &response_payload.point_init_info;
    point_init_info_ptr->has_top_connection     = position_point.top_connection_id != 0;
    point_init_info_ptr->has_right_connection   = position_point.right_connection_id != 0;
    point_init_info_ptr->has_bottom_connection  = position_point.bottom_connection_id != 0;
    point_init_info_ptr->has_left_connection    = position_point.left_connection_id != 0;
    point_init_info_ptr->is_exit                = position_point.is_exit;
    point_init_info_ptr->is_spawn               = position_point.is_spawn;

    response_payload.room_size = LABPOINT_SIZE;

    *is_have_response = true;
    GameServerCommandResponse_SetType(response_command, kCommandType_PlayerInit);
    GameServerCommandResponse_AddPlayerIndex(response_command, player_index);
    return GameServerCommandResponse_SetPayload(response_command, 
                                                (const uint8 *) &response_payload,
                                                sizeof(response_payload));
    
}