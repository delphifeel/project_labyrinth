#ifndef _ROOM_INFO_H_
#define _ROOM_INFO_H_

#include "CCORE.h"
#include "common.h"

typedef struct PlayerInfo
{
    uint32          id;
    PositionStruct  position;
} PlayerInfo;

#include "room-info-private.h"
typedef struct RoomInfo RoomInfo;

void RoomInfo_InitFromLabPoint(RoomInfo *room_info, const LabPointStruct *lab_point);
void RoomInfo_AddPlayerInfo(RoomInfo *room_info, const PlayerInfo *player_info);
uint RoomInfo_WriteToBuffer(const RoomInfo *room_info, uint8 buff[], uint buff_max_size);

#endif