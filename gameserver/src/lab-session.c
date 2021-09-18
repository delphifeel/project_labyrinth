#include "CORE.h"
#include "game-server/lab-generation.h"
#include "game-server/lab-session.h"


CORE_OBJECT_INTERFACE(LabSession,
	uint8  				session_uid[UID_SIZE];

	/* hash map structure containing graph of LabPoint's */
	LabPointsMap 		labyrinth_map;
	LabPointsMapReader 	labyrinth_map_reader;

	/* players that currently in this session */
	Player 				*players_map;
	uint32  			players_map_size;
	uint32 				players_map_capacity;

	uint32  			*spawn_points;
	uint32  			spawn_points_size;
	uint32  			spawn_points_assoc_with_player_count;

	/* etc */
);

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

void LabSession_AddPlayer(LabSession instance, char *player_name, uint32 *out_added_player_index)
{
	Player new_player;
	uint32 player_spawn_point_id;


	if (instance->players_map_size == instance->players_map_capacity)
	{
		CORE_DebugError("players_map_size == players_map_capacity\n");
		return;
	}

	*out_added_player_index = instance->players_map_size;

	if (instance->spawn_points_assoc_with_player_count == instance->spawn_points_size)
	{
		CORE_DebugError("No more spawn points available\n");
		return;
	}

	player_spawn_point_id = instance->spawn_points[instance->spawn_points_assoc_with_player_count++];

	Player_Create(&new_player);
	Player_Setup(new_player, instance->labyrinth_map_reader, player_spawn_point_id);
	Player_SetId(new_player, *out_added_player_index + 1);
	Player_SetName(new_player, player_name);

	instance->players_map[instance->players_map_size] = new_player;
	instance->players_map_size++;
}

void LabSession_GetLabPointsReader(LabSession instance, LabPointsMapReader *out_lab_points_reader)
{
	*out_lab_points_reader = instance->labyrinth_map_reader;
}

/*****************************************************************************************************************************/

void LabSession_Setup(LabSession instance, uint32 players_count)
{
	if (CORE_CreateUID(instance->session_uid) == FALSE)
	{
		CORE_DebugAbort("Can't create UID\n");
		return;
	}

	instance->players_map = CORE_MemAlloc(sizeof(Player) * players_count);
	instance->players_map_size = 0;
	instance->players_map_capacity = players_count;

	LabPointsMap_Create(&instance->labyrinth_map);
	LabPointsMapReader_Create(&instance->labyrinth_map_reader);
	LabPointsMapReader_Setup(instance->labyrinth_map_reader, instance->labyrinth_map);

	instance->spawn_points_assoc_with_player_count = 0;
	LabGeneration_Execute(instance->labyrinth_map, &instance->spawn_points, &instance->spawn_points_size);
}

/*****************************************************************************************************************************/

void LabSession_Create(LabSession* instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, LabSession);
}

void LabSession_Free(LabSession* instance_ptr)
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
