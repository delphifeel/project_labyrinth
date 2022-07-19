#include <string.h>

#include "CCORE.h"
#include "gameserver/lab-points-map.h"

/*****************************************************************************************************************************/

typedef struct LabPointsMap_s
{
    LabPointStruct          *points_hash_map;
    CList /*of Player*/     **each_point_players;
    uint32                  size;
} *LabPointsMap;

/*****************************************************************************************************************************/

void LabPointsMap_GetRoomInfoByPointId(LabPointsMap instance, uint32 point_id, RoomInfo *room_info)
{
    LabPointStruct point;
    LabPointsMap_GetPointByID(instance, point_id, &point);
    RoomInfo_InitFromLabPoint(room_info, &point);

    Player iter_player = NULL;
    PlayerInfo player_info;
    CList_ForEach(instance->each_point_players[point_id - 1], iter_player) {
        Player_GetId(iter_player, &player_info.id);
        Player_GetPositionCoords(iter_player, &player_info.position);
        RoomInfo_AddPlayerInfo(room_info, &player_info);
    }
}

void LabPointsMap_ToJSON(LabPointsMap instance, char **json)
{
    #define RAW_JSON_OBJECT_MAX_SIZE    (120)
    #define DEC_JSON_LEFT(SIZE_TO_DEC) (json_size_left = ((SIZE_TO_DEC) > json_size_left ? 0 : (json_size_left - (SIZE_TO_DEC)))) 

    int32               json_size_left;
    uint32              max_json;
    LabPointStruct      current_lab_point; 
    char                lab_point_raw_json_object[RAW_JSON_OBJECT_MAX_SIZE];
    uint32              lab_point_raw_json_object_char_count;


    #ifdef CORE_DEBUGENABLED
    LabPointsMap_InternalPrint(instance);
    #endif

    max_json = instance->size * RAW_JSON_OBJECT_MAX_SIZE + 40;
    json_size_left = max_json;
    *json = CORE_MemAlloc(sizeof(char), max_json + 1);
    *json[0] = '\0';

    lab_point_raw_json_object_char_count = sprintf(lab_point_raw_json_object, "{\"count\": %u,\"points\": {", instance->size);
    strncat(*json, lab_point_raw_json_object, lab_point_raw_json_object_char_count);
    DEC_JSON_LEFT(lab_point_raw_json_object_char_count);

    for (uint32 i = 0; i < instance->size; i++)
    {
        
        current_lab_point = instance->points_hash_map[i]; 
        lab_point_raw_json_object_char_count = sprintf(lab_point_raw_json_object, "\"%u\": ", current_lab_point.Id);
        strncat(*json, lab_point_raw_json_object, lab_point_raw_json_object_char_count);
        DEC_JSON_LEFT(lab_point_raw_json_object_char_count);

        lab_point_raw_json_object_char_count = sprintf(lab_point_raw_json_object, 
                                                  "{\"top_id\": %u, \"right_id\": %u, \"bottom_id\": %u, \"left_id\": %u, \"is_exit\": %u, \"is_spawn\": %u}",
                                                  current_lab_point.top_connection_id, current_lab_point.right_connection_id, 
                                                  current_lab_point.bottom_connection_id, current_lab_point.left_connection_id,
                                                  current_lab_point.is_exit, current_lab_point.is_spawn);

        strncat(*json, lab_point_raw_json_object, lab_point_raw_json_object_char_count);
        DEC_JSON_LEFT(lab_point_raw_json_object_char_count);

        if (i != instance->size - 1)
        {
            strncat(*json, ",", 1);
            DEC_JSON_LEFT(1);
        }
    }

    strncat(*json, "}}", 2);
    DEC_JSON_LEFT(2);
}

void LabPointsMap_AssignPlayerToPoint(LabPointsMap instance, Player player, uint point_id)
{
    CORE_AssertPointer(player);
    CORE_Assert(point_id <= instance->size);

    LabPointStruct prev_point;
    Player_GetPositionPoint(player, &prev_point);

    if (prev_point.Id != point_id) {
        // remove player from old point
        CList *prev_point_players = instance->each_point_players[prev_point.Id - 1];
        CORE_Assert(CList_Remove(prev_point_players, player));
    }

    // assign player to @point_id 
    CList *curr_point_players = instance->each_point_players[point_id - 1];
    CList_Append(curr_point_players, player);
}

void LabPointsMap_ChangePoint(LabPointsMap instance, LabPointStruct point)
{
    CORE_Assert(point.Id <= instance->size);

    instance->points_hash_map[point.Id - 1] = point;
}

void LabPointsMap_GetPointByID(LabPointsMap instance, uint32 id, LabPointStruct *out_point)
{
    CORE_Assert(id > 0);
    CORE_Assert(id <= instance->size);

    *out_point = instance->points_hash_map[id - 1];
}

void LabPointsMap_GetSize(LabPointsMap instance, uint32 *size)
{
    *size = instance->size;
}

/*****************************************************************************************************************************/

void LabPointsMap_Create(LabPointsMap *instance_ptr, uint size)
{
    *instance_ptr = CORE_MemAlloc(sizeof(struct LabPointsMap_s), 1);
    LabPointsMap instance = *instance_ptr;

    instance->size = size;
    instance->points_hash_map = CORE_MemCalloc(sizeof(LabPointStruct), instance->size);
    instance->each_point_players = CORE_MemCalloc(sizeof(CList *), instance->size);

    for (uint i = 0; i < instance->size; i++) {
        instance->each_point_players[i] = CList_Create();
    }
}

void LabPointsMap_Free(LabPointsMap *instance_ptr)
{
    LabPointsMap instance = *instance_ptr;
    for (uint i = 0; i < instance->size; i++) {
        CList_Free(&instance->each_point_players[i]);
    }
    CORE_MemFree(instance->each_point_players);

    CORE_MemFree(instance->points_hash_map);
    CORE_MemFree(*instance_ptr);
}

/*****************************************************************************************************************************/
