#ifndef __LAB_SESSION_H__
#define __LAB_SESSION_H__

#include "../CORE.h"
#include "../player.h"
#include "lab-points-map.h"

/*****************************************************************************************************************************/

/*
	LabSession - object containing specific game session.
	On create it's create labyrinth and spawn players on spawn points.
	When its freed, thats mean session ended 
*/
CORE_OBJECT_DEFINE(LabSession);

/*****************************************************************************************************************************/

void LabSession_AddPlayer(LabSession, char *player_name, uint32 *out_added_player_index);
CORE_Bool LabSession_FindPlayer(LabSession, uint32 player_index, Player *out_player);

void LabSession_GetLabPointsReader(LabSession, LabPointsMapReader *out_lab_points_reader);

void LabSession_Setup(LabSession, uint32 players_count);

void LabSession_Create(LabSession*);
void LabSession_Free(LabSession*);

/*****************************************************************************************************************************/

#endif