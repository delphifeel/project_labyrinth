#include "gameserver/room-info.h"

void RoomInfo_InitFromLabPoint(RoomInfo *room_info, const LabPointStruct *lab_point)
{
    room_info->has_top_connection     = lab_point->top_connection_id != 0;
    room_info->has_right_connection   = lab_point->right_connection_id != 0;
    room_info->has_bottom_connection  = lab_point->bottom_connection_id != 0;
    room_info->has_left_connection    = lab_point->left_connection_id != 0;
    room_info->is_exit                = lab_point->is_exit == true;
    room_info->is_spawn               = lab_point->is_spawn == true;
    room_info->players_count          = 0;
}

void RoomInfo_AddPlayerInfo(RoomInfo *room_info, const PlayerInfo *player_info)
{
    room_info->players_info[room_info->players_count] = *player_info;
    room_info->players_count++; 
}

uint RoomInfo_WriteToBuffer(const RoomInfo *room_info, uint8 buff[], uint buff_max_size)
{
    uint header_bytes = offsetof(RoomInfo, players_info);
    uint players_info_bytes = room_info->players_count * sizeof(PlayerInfo);
    uint bytes_written = header_bytes + players_info_bytes;
    CORE_AssertWithMessage(buff_max_size >= bytes_written, "Buffer is too small\n");

    uint8 *buff_ptr = buff;
    CORE_MemCpy(buff_ptr, room_info, header_bytes);
    buff_ptr += header_bytes;
    CORE_MemCpy(buff_ptr, &room_info->players_info, players_info_bytes);
    return bytes_written;
}
