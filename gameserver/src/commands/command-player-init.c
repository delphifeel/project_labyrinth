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

// TODO: duplicate for command-player-move
static void _MakeResponsePayload(uint8                  buffer[], 
                                 uint                   buffer_max_size, 
                                 uint                   *buffer_len,
                                 const PlayerInitInfo   *player_info,
                                 const RoomInfo         *room_info)
{
    uint8 *buffer_ptr = buffer;
    uint buffer_bytes_left = buffer_max_size;

    // Player init info
    CORE_MemCpy(buffer_ptr, player_info, sizeof(*player_info));
    buffer_ptr += sizeof(*player_info);

    // Room size
    *((uint32 *) buffer_ptr) = ROOM_SIZE;
    buffer_ptr += sizeof(uint32);

    buffer_bytes_left -= buffer_ptr - buffer;

    // Room info
    uint bytes = RoomInfo_WriteToBuffer(room_info, buffer_ptr, buffer_bytes_left);
    buffer_ptr += bytes;
    *buffer_len = buffer_ptr - buffer;
}

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
    const uint8                 *_unused;   


    GameServerCommand_GetSessionsPtr(command, &sessions, &sessions_size);
    GameServerCommand_GetSessionIndex(command, &session_index);
    GameServerCommand_GetPlayerIndex(command, &player_index);
    GameServerCommand_GetPayloadPtr(command, &_unused, &payload_size);

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

    PlayerInitInfo player_info;
    player_info.speed = Player_GetSpeed(player);
    Player_GetPositionCoords(player, &player_info.position);

    LabPointStruct position_point;
    Player_GetPositionPoint(player, &position_point);
    LabPointsMap lab_map = LabSession_GetLabMap(session);
    RoomInfo room_info;
    LabPointsMap_GetRoomInfoByPointId(lab_map, position_point.Id, &room_info);

    uint8 response_buff[COMMAND_MAX_PAYLOAD_SIZE];
    uint response_buff_size = 0;
    _MakeResponsePayload(response_buff, sizeof(response_buff), &response_buff_size, &player_info, &room_info);

    *is_have_response = true;
    GameServerCommandResponse_SetType(response_command, kCommandType_PlayerInit);
    GameServerCommandResponse_AddPlayerIndex(response_command, player_index);
    return GameServerCommandResponse_SetPayload(response_command, 
                                                response_buff,
                                                response_buff_size);
    
}