#ifndef __LAB_POINTS_MAP_H__
#define __LAB_POINTS_MAP_H__

#include "CCORE.h"
#include "player.h"
#include "room-info.h"

#define ROOM_SIZE   (40)

CORE_OBJECT_DEFINE(Player);

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(LabPointsMap);

/*****************************************************************************************************************************/

void LabPointsMap_GetRoomInfoByPointId(LabPointsMap instance, uint32 point_id, RoomInfo *room_info);

void LabPointsMap_ToJSON(LabPointsMap instance, char **json);
void LabPointsMap_ToRawData(LabPointsMap, uint8 **raw_data, uint32 *raw_data_size);

void LabPointsMap_AssignPlayerToPoint(LabPointsMap instance, Player player, uint point_id);
void LabPointsMap_ChangePoint(LabPointsMap instance, LabPointStruct point);
void LabPointsMap_GetPointByID(LabPointsMap instance, uint32 id, LabPointStruct *out_point);
void LabPointsMap_GetSize(LabPointsMap instance, uint32 *size);

void LabPointsMap_Create(LabPointsMap* instance_ptr, uint size);
void LabPointsMap_Free(LabPointsMap* instance_ptr);

/*****************************************************************************************************************************/

#endif
