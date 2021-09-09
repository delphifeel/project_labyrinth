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

void LabSession_AddPlayer(LabSession, char *PlayerName, uint32 *OUT_AddedPlayerId);

void LabSession_Setup(LabSession, uint32 PlayersCount);

void LabSession_Create(LabSession*);
void LabSession_Free(LabSession*);

/*****************************************************************************************************************************/

#endif