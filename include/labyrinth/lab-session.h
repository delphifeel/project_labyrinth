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

CORE_OBJECT_INTERFACE(LabSession,
	uint32  		SessionId;

	/* hash map structure containing graph of LabPoint's */
	LabPointsMap 	LabyrinthMap;

	/* players that currently in this session */
	Player 			*PlayerList;
	uint32  		PlayerListSize;

	/* etc */
)

/*****************************************************************************************************************************/

void LabSession_GetLabPointById(LabSession, uint32 Id, LabPoint *OUT_LabPoint);

/*****************************************************************************************************************************/

#endif