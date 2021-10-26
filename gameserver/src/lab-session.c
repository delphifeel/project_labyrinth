#include "CORE.h"
#include "gameserver/lab-generation.h"
#include "gameserver/lab-session.h"


CORE_OBJECT_INTERFACE(LabSession,
	LabPointsMap 		labyrinth_map;
	LabPointsMapReader 	labyrinth_map_reader;

	/*
	 *		Players added to session
	 */
	Player 				*players_map;
	uint32  			players_map_size;
	uint32 				players_map_capacity;

	/*
	 *		Spawn points for players
	 */
	uint32  			*spawn_points;
	uint32  			spawn_points_size;
	uint32  			spawn_points_assoc_with_player_count;

	CORE_Bool 			is_session_started;

	/* etc */
);

/*****************************************************************************************************************************/

CORE_Bool LabSession_HelperFindSession(LabSession sessions[], uint32 sessions_size, uint32 index, LabSession *out_session)
{
	CORE_AssertPointer(sessions);
	CORE_AssertPointer(out_session);

	if (index + 1 > sessions_size)
	{
		CORE_DebugError("Session index out of bounds\n");
		return FALSE;
	}

	*out_session = sessions[index];
	if (*out_session == NULL)
	{
		CORE_DebugError("Found session is NULL\n");
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************************************************************/

CORE_Bool LabSession_FindPlayer(LabSession instance, uint32 player_index, Player *out_player)
{
	if (player_index + 1 > instance->players_map_size)
	{
		CORE_DebugError("Player index out of bounds\n");
		return FALSE;
	}

	*out_player = instance->players_map[player_index];
	return TRUE;
}

CORE_Bool LabSession_AddPlayer(LabSession instance, uint32 player_id, const uint8 player_token[TOKEN_SIZE], uint32 *out_added_player_index)
{
	CORE_AssertPointer(out_added_player_index);
	CORE_Assert(instance->is_session_started == FALSE);

	Player new_player;
	uint32 player_spawn_point_id;


	if (instance->players_map_size == instance->players_map_capacity)
	{
		CORE_DebugError("No more free spots - session is FULL\n");
		return FALSE;
	}

	*out_added_player_index = instance->players_map_size;

	if (instance->spawn_points_assoc_with_player_count == instance->spawn_points_size)
	{
		CORE_DebugError("No more spawn points available\n");
		return FALSE;
	}

	player_spawn_point_id = instance->spawn_points[instance->spawn_points_assoc_with_player_count++];

	Player_Create(&new_player);
	Player_Setup(new_player, instance->labyrinth_map_reader, player_spawn_point_id);
	Player_SetId(new_player, player_id);
	Player_SetToken(new_player, player_token);

	instance->players_map[instance->players_map_size] = new_player;
	instance->players_map_size++;
	return TRUE;
}

void LabSession_Start(LabSession instance)
{
	CORE_Assert(instance->is_session_started == FALSE);

	instance->is_session_started = TRUE;
}

void LabSession_GetLabPointsReader(LabSession instance, LabPointsMapReader *out_lab_points_reader)
{
	CORE_AssertPointer(out_lab_points_reader);

	*out_lab_points_reader = instance->labyrinth_map_reader;
}

void LabSession_MapToJSON(LabSession instance, char **json)
{
	LabPointsMap_ToJSON(instance->labyrinth_map, json);
}

/*****************************************************************************************************************************/

void LabSession_Setup(LabSession instance, uint32 players_count)
{
	instance->is_session_started = FALSE;

	instance->players_map = CORE_MemAlloc(sizeof(Player) * players_count);
	instance->players_map_size = 0;
	instance->players_map_capacity = players_count;

	
	LabPointsMapReader_Setup(instance->labyrinth_map_reader, instance->labyrinth_map);

	instance->spawn_points_assoc_with_player_count = 0;
	LabGeneration_Execute(instance->labyrinth_map, &instance->spawn_points, &instance->spawn_points_size);
}

/*****************************************************************************************************************************/

void LabSession_Create(LabSession *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, LabSession);
	LabSession instance = *instance_ptr;

	LabPointsMap_Create(&instance->labyrinth_map);
	LabPointsMapReader_Create(&instance->labyrinth_map_reader);
}

void LabSession_Free(LabSession *instance_ptr)
{
	CORE_MemFree((*instance_ptr)->spawn_points);

	LabPointsMapReader_Free(&(*instance_ptr)->labyrinth_map_reader);
	LabPointsMap_Free(&(*instance_ptr)->labyrinth_map);

	for (uint32 i = 0; i < (*instance_ptr)->players_map_size; i++)
	{
		CORE_MemFree((*instance_ptr)->players_map[i]);
	}

	CORE_MemFree((*instance_ptr)->players_map);
	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
