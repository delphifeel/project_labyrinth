#include "CCORE.h"
#include "modules/labyrinth/lab-generation.h"
#include "modules/labyrinth/lab-session.h"


typedef struct LabSession_s
{
    LabPointsMap       *lab_map;

    /**
     *      Players added to session
     */
    Player            **players_map;
    uint                players_map_size;
    uint                players_map_capacity;

    /**
     *      Spawn points for players
     */
    uint               *spawn_points;
    uint                spawn_points_size;
    uint                spawn_points_assoc_with_player_count;

    bool                is_session_started;

    /* etc */
} LabSession;

/*****************************************************************************************************************************/

static inline bool _SessionIsFull(const LabSession *session)
{
    return session->players_map_size == session->players_map_capacity;
}

static inline bool _NoMoreSpawnPoints(const LabSession *session)
{
    return session->spawn_points_assoc_with_player_count == session->spawn_points_size;
}

/*****************************************************************************************************************************/

LabPointsMap *LabSession_GetLabMap(LabSession *session)
{
    CORE_AssertPointer(session);
    CORE_Assert(session->is_session_started);


    return session->lab_map;
}

Player *LabSession_FindPlayer(LabSession *session, uint player_id)
{
    CORE_AssertPointer(session);


    if (player_id > session->players_map_size)
    {
        CORE_DebugError("Player id out of bounds\n");
        return NULL;
    }

    return session->players_map[player_id - 1];
}

bool LabSession_AddPlayer(LabSession        *session, 
                          uint              *added_player_id)
{
    CORE_AssertPointer(session);
    CORE_AssertPointer(added_player_id);
    CORE_Assert(session->is_session_started == false);


    if (_SessionIsFull(session)) {
        CORE_DebugError("No more free spots - session is FULL\n");
        return false;
    }

    *added_player_id = session->players_map_size + 1;

    if (_NoMoreSpawnPoints(session)) {
        CORE_DebugError("No more spawn points available\n");
        return false;
    }

    uint player_spawn_point_id = session->spawn_points[session->spawn_points_assoc_with_player_count++];

    Player *new_player = Player_Create(*added_player_id);
    LabPointsMap_AssignPlayerToPoint(session->lab_map, new_player, player_spawn_point_id);

    CORE_DebugInfo(
        "Player #%u added. Spawn point id: %u\n", 
        *added_player_id,
        player_spawn_point_id
    );
    session->players_map[session->players_map_size] = new_player;
    session->players_map_size++;
    return true;
}

bool LabSession_IsReadyForStart(const LabSession *session)
{
    return _SessionIsFull(session);
}

void LabSession_Start(LabSession *session)
{
    CORE_AssertPointer(session);
    CORE_Assert(LabSession_IsReadyForStart(session));
    CORE_Assert(session->is_session_started == false);


    session->is_session_started = true;
    CORE_DebugInfo("Session started\n");
}

void LabSession_MapToJSON(LabSession *session, char **json)
{
    CORE_AssertPointer(session);
    LabPointsMap_ToJSON(session->lab_map, json);
}

/*****************************************************************************************************************************/

void LabSession_Setup(LabSession *session, uint players_count)
{
    CORE_AssertPointer(session);
    CORE_Assert(players_count > 0);


    CORE_DebugInfo("Setup session. Player count: %u\n", players_count);
    session->is_session_started = false;

    session->players_map = CORE_MemAlloc(sizeof(Player *), players_count);
    session->players_map_size = 0;
    session->players_map_capacity = players_count;
    session->spawn_points_assoc_with_player_count = 0;

    LabGeneration_Execute(&session->lab_map, &session->spawn_points, &session->spawn_points_size);
}

/*****************************************************************************************************************************/

LabSession *LabSession_Create(void)
{
    LabSession *session = CORE_MemAlloc(sizeof(LabSession), 1);
    return session;
}

void LabSession_Free(LabSession *session)
{
    CORE_AssertPointer(session);


    CORE_MemFree(session->spawn_points);
    LabPointsMap_Free(session->lab_map);
    for (uint i = 0; i < session->players_map_size; i++) {
        Player_Free(session->players_map[i]);
    }
    CORE_MemFree(session->players_map);

    CORE_MemFree(session);
}

/*****************************************************************************************************************************/