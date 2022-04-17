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
    PositionStruct  position;
} ResponseHeader;

// TODO: Create BytesBuffer object
// TODO: duplicate for command-player-init. Make command-get-room-info ? 
static void _MakeResponsePayload(uint8                  buffer[], 
                                 uint                   buffer_max_size, 
                                 uint                   *buffer_len,
                                 const ResponseHeader   *header,
                                 const RoomInfo         *room_info)
{
    uint8 *buffer_ptr = buffer;
    uint buffer_bytes_left = buffer_max_size;

    // Header
    CORE_MemCpy(buffer_ptr, header, sizeof(*header));
    buffer_ptr += sizeof(*header);

    buffer_bytes_left -= buffer_ptr - buffer;

    // Room info
    uint bytes = RoomInfo_WriteToBuffer(room_info, buffer_ptr, buffer_bytes_left);
    buffer_ptr += bytes;
    *buffer_len = buffer_ptr - buffer;
}

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


    GameServerCommand_GetSessionsPtr(command, &sessions, &sessions_size);
    GameServerCommand_GetSessionIndex(command, &session_index);
    GameServerCommand_GetPlayerIndex(command, &player_index);
    GameServerCommand_GetPayloadPtr(command, &payload_raw, &payload_size);

    if (payload_size != sizeof(RequestPayload)) {
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

    ResponseHeader response_header;
    if (Player_Move(player, directions, directions_size)) {
        response_header.is_ok = 1;
        CORE_MemCpy(response_header.directions, 
                    payload->directions, 
                    sizeof(response_header.directions));
    }
    else {
        response_header.is_ok = 0;
        CORE_MemZero(response_header.directions, 
                     sizeof(response_header.directions));
    }
    LabPointStruct point;
    Player_GetPositionCoords(player, &response_header.position);
    Player_GetPositionPoint(player, &point);
    LabPointsMap lab_map = LabSession_GetLabMap(session);
    RoomInfo room_info;
    LabPointsMap_GetRoomInfoByPointId(lab_map, point.Id, &room_info);

    uint8 response_buff[COMMAND_MAX_PAYLOAD_SIZE];
    uint response_buff_size = 0;
    _MakeResponsePayload(response_buff, sizeof(response_buff), &response_buff_size, &response_header, &room_info);
    
    *is_have_response = true;
    GameServerCommandResponse_SetType(response_command, kCommandType_PlayerMove);
    GameServerCommandResponse_AddPlayerIndex(response_command, player_index);
    return GameServerCommandResponse_SetPayload(response_command, 
                                                response_buff,
                                                response_buff_size);
}
