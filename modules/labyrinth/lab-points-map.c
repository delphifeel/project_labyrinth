#include "CCORE.h"
#include "config.h"
#include "modules/labyrinth/lab-points-map.h"

/*****************************************************************************************************************************/

typedef struct LabPointsMap_s
{
    LabPoint                 *points_hash_map;
    CList /*of Players*/    **each_point_players;
    uint                      player_to_point_map[SESSION_PLAYERS_COUNT];
    uint                      size;
} LabPointsMap;

/*****************************************************************************************************************************/

static inline bool _GetPlayerAssignedPoint(const LabPointsMap *lab_map, uint player_id, uint *point_id)
{
    const uint player_to_point_map_size = sizeof(lab_map->player_to_point_map) / sizeof(*lab_map->player_to_point_map);
    if (player_id > player_to_point_map_size) {
        return false;
    }

    *point_id = lab_map->player_to_point_map[player_id - 1];
    if (*point_id == 0) {
        return false;
    }
    return true;
}

/*****************************************************************************************************************************/

void LabPointsMap_ToJSON(const LabPointsMap *lab_map, char **json)
{
    #define RAW_JSON_OBJECT_MAX_SIZE    (120)
    #define DEC_JSON_LEFT(SIZE_TO_DEC) (json_size_left = ((SIZE_TO_DEC) > json_size_left ? 0 : (json_size_left - (SIZE_TO_DEC)))) 

    int32               json_size_left;
    uint32              max_json;
    LabPoint            current_lab_point; 
    char                lab_point_raw_json_object[RAW_JSON_OBJECT_MAX_SIZE];
    uint32              lab_point_raw_json_object_char_count;


    max_json = lab_map->size * RAW_JSON_OBJECT_MAX_SIZE + 40;
    json_size_left = max_json;
    *json = CORE_MemAlloc(sizeof(char), max_json + 1);
    *json[0] = '\0';

    lab_point_raw_json_object_char_count = sprintf(lab_point_raw_json_object, "{\"count\": %u,\"points\": {", lab_map->size);
    strncat(*json, lab_point_raw_json_object, lab_point_raw_json_object_char_count);
    DEC_JSON_LEFT(lab_point_raw_json_object_char_count);

    for (uint32 i = 0; i < lab_map->size; i++)
    {
        
        current_lab_point = lab_map->points_hash_map[i]; 
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

        if (i != lab_map->size - 1)
        {
            strncat(*json, ",", 1);
            DEC_JSON_LEFT(1);
        }
    }

    strncat(*json, "}}", 2);
    DEC_JSON_LEFT(2);
}

bool LabPointsMap_AssignPlayerToPoint(LabPointsMap *lab_map, Player *player, uint point_id)
{
    CORE_AssertPointer(player);
    CORE_Assert(point_id > 0);

    if (point_id > lab_map->size) {
        return false;
    }

    uint prev_point_id;
    _GetPlayerAssignedPoint(lab_map, Player_GetId(player), &prev_point_id);

    if ((prev_point_id != 0) && (prev_point_id != point_id)) {
        // remove player from old point
        CList *prev_point_players = lab_map->each_point_players[prev_point_id - 1];
        CORE_Assert(CList_Remove(prev_point_players, player));
    }

    // assign player to @point_id 
    CList *curr_point_players = lab_map->each_point_players[point_id - 1];
    CList_Append(curr_point_players, player);
    return true;
}

bool LabPointsMap_ChangePoint(LabPointsMap *lab_map, const LabPoint point)
{
    CORE_Assert(point.Id > 0);

    if (point.Id > lab_map->size) {
        return false;
    }

    lab_map->points_hash_map[point.Id - 1] = point;
    return true;
}

bool LabPointsMap_GetPointByID(const LabPointsMap *lab_map, uint id, LabPoint *point)
{
    CORE_Assert(id > 0);

    if (id > lab_map->size) {
        return false;
    }

    *point = lab_map->points_hash_map[id - 1];
    return true;
}

uint LabPointsMap_GetSize(const LabPointsMap *lab_map)
{
    return lab_map->size;
}

/*****************************************************************************************************************************/

LabPointsMap *LabPointsMap_Create(uint size)
{
    LabPointsMap *lab_map = CORE_MemAlloc(sizeof(LabPointsMap), 1);

    lab_map->size = size;
    lab_map->points_hash_map = CORE_MemCalloc(sizeof(LabPoint), lab_map->size);
    lab_map->each_point_players = CORE_MemCalloc(sizeof(CList *), lab_map->size);
    CORE_MemZero(lab_map->player_to_point_map, sizeof(lab_map->player_to_point_map));

    for (uint i = 0; i < lab_map->size; i++) {
        lab_map->each_point_players[i] = CList_Create();
    }

    return lab_map;
}

void LabPointsMap_Free(LabPointsMap *lab_map)
{
    for (uint i = 0; i < lab_map->size; i++) {
        CList_Free(&lab_map->each_point_players[i]);
    }
    CORE_MemFree(lab_map->each_point_players);
    CORE_MemFree(lab_map->points_hash_map);

    CORE_MemFree(lab_map);
}

/*****************************************************************************************************************************/