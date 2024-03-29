#include "CCORE.h"
#include "gameserver/lab-generation.h"
#include "gameserver/lab-session.h"


typedef struct LabSession_s
{
    LabPointsMap        labyrinth_map;

    /**
     *      Players added to session
     */
    Player              *players_map;
    uint32              players_map_size;
    uint32              players_map_capacity;

    /**
     *      Spawn points for players
     */
    uint32              *spawn_points;
    uint32              spawn_points_size;
    uint32              spawn_points_assoc_with_player_count;

    bool                is_session_started;

    /* etc */
} *LabSession;

/*****************************************************************************************************************************/

bool LabSession_HelperFindSession(LabSession sessions[], uint32 sessions_size, uint32 index, LabSession *out_session)
{
    CORE_AssertPointer(sessions);
    CORE_AssertPointer(out_session);

    if (index + 1 > sessions_size)
    {
        CORE_DebugError("Session index out of bounds\n");
        return false;
    }

    *out_session = sessions[index];
    if (*out_session == NULL)
    {
        CORE_DebugError("Found session is NULL\n");
        return false;
    }

    return true;
}

/*****************************************************************************************************************************/

LabPointsMap LabSession_GetLabMap(LabSession session)
{
    CORE_Assert(session->is_session_started);
    return session->labyrinth_map;
}

bool LabSession_FindPlayer(LabSession instance, uint32 player_index, Player *out_player)
{
    if (player_index + 1 > instance->players_map_size)
    {
        CORE_DebugError("Player index out of bounds\n");
        return false;
    }

    *out_player = instance->players_map[player_index];
    return true;
}

bool LabSession_AddPlayer(LabSession        instance, 
                          uint32            player_id, 
                          const uint8       player_token[TOKEN_SIZE], 
                          uint32            *out_added_player_index)
{
    CORE_AssertPointer(out_added_player_index);
    CORE_Assert(instance->is_session_started == false);

    Player new_player;
    uint32 player_spawn_point_id;


    if (instance->players_map_size == instance->players_map_capacity)
    {
        CORE_DebugError("No more free spots - session is FULL\n");
        return false;
    }

    *out_added_player_index = instance->players_map_size;

    if (instance->spawn_points_assoc_with_player_count == instance->spawn_points_size)
    {
        CORE_DebugError("No more spawn points available\n");
        return false;
    }

    player_spawn_point_id = instance->spawn_points[instance->spawn_points_assoc_with_player_count++];

    Player_Create(&new_player);
    Player_Setup(new_player, instance->labyrinth_map, player_spawn_point_id);
    Player_SetId(new_player, player_id);
    Player_SetToken(new_player, player_token);
    LabPointsMap_AssignPlayerToPoint(instance->labyrinth_map, new_player, player_spawn_point_id);

    CORE_DebugInfo(
        "Player #%u added. id: %u, spawn point id: %u\n", 
        *out_added_player_index, 
        player_id,
        player_spawn_point_id
    );
    instance->players_map[instance->players_map_size] = new_player;
    instance->players_map_size++;
    return true;
}

void LabSession_Start(LabSession instance)
{
    CORE_Assert(instance->is_session_started == false);

    instance->is_session_started = true;
    CORE_DebugInfo("Session started\n");
}

void LabSession_MapToJSON(LabSession instance, char **json)
{
    LabPointsMap_ToJSON(instance->labyrinth_map, json);
}

/*****************************************************************************************************************************/

void LabSession_Setup(LabSession instance, uint32 players_count)
{
    CORE_DebugInfo("Setup session. Player count: %u\n", players_count);
    instance->is_session_started = false;

    instance->players_map = CORE_MemAlloc(sizeof(Player), players_count);
    instance->players_map_size = 0;
    instance->players_map_capacity = players_count;
    instance->spawn_points_assoc_with_player_count = 0;

    LabGeneration_Execute(&instance->labyrinth_map, &instance->spawn_points, &instance->spawn_points_size);
}

/*****************************************************************************************************************************/

void LabSession_Create(LabSession *instance_ptr)
{
    *instance_ptr = CORE_MemAlloc(sizeof(struct LabSession_s), 1);
    LabSession instance = *instance_ptr;
}

void LabSession_Free(LabSession *instance_ptr)
{
    CORE_MemFree((*instance_ptr)->spawn_points);

    LabPointsMap_Free(&(*instance_ptr)->labyrinth_map);

    for (uint32 i = 0; i < (*instance_ptr)->players_map_size; i++) {
        CORE_MemFree((*instance_ptr)->players_map[i]);
    }

    CORE_MemFree((*instance_ptr)->players_map);
    CORE_MemFree(*instance_ptr);
}

/*****************************************************************************************************************************/
