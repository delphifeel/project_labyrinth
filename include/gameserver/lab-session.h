#ifndef _LAB_SESSION_H_
#define _LAB_SESSION_H_

#include "CORE.h"
#include "player.h"
#include "lab-points-map.h"

/*****************************************************************************************************************************/

/*
	LabSession - object containing specific game session.
	On create it's create labyrinth and spawn players on spawn points.
	When its freed, thats mean session ended 
*/
CORE_OBJECT_DEFINE(LabSession);

/*****************************************************************************************************************************/

void LabSession_AddPlayer(LabSession instance, char *player_name, uint32 *out_added_player_index);
CORE_Bool LabSession_FindPlayer(LabSession instance, uint32 player_index, Player *out_player);

void LabSession_GetLabPointsReader(LabSession instance, LabPointsMapReader *out_lab_points_reader);

void LabSession_Setup(LabSession instance, uint32 players_count);

void LabSession_Create(LabSession* instance_ptr);
void LabSession_Free(LabSession* instance_ptr);

/*****************************************************************************************************************************/

#endif
