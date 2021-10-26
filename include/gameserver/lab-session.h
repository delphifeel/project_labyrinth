#ifndef _LAB_SESSION_H_
#define _LAB_SESSION_H_

#include "CORE.h"
#include "player.h"
#include "lab-points-map.h"

/*****************************************************************************************************************************/

/*
 *	LabSession - object containing specific game session.
 *	On setup it's create labyrinth and spawn players on spawn points.
 */
CORE_OBJECT_DEFINE(LabSession);

/*****************************************************************************************************************************/

CORE_Bool 	LabSession_HelperFindSession(LabSession sessions[], uint32 sessions_size, uint32 index, LabSession *out_session);

CORE_Bool 	LabSession_AddPlayer(LabSession instance, uint32 player_id, const uint8 player_token[TOKEN_SIZE], uint32 *out_added_player_index);
CORE_Bool 	LabSession_FindPlayer(LabSession instance, uint32 player_index, Player *out_player);
void 		LabSession_Start(LabSession instance);

void 		LabSession_GetLabPointsReader(LabSession instance, LabPointsMapReader *out_lab_points_reader);

void 		LabSession_Setup(LabSession instance, uint32 players_count);

void 		LabSession_Create(LabSession* instance_ptr);
void 		LabSession_Free(LabSession* instance_ptr);

/*****************************************************************************************************************************/

#endif
