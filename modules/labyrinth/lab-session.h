#ifndef _LAB_SESSION_H_
#define _LAB_SESSION_H_

#include "libs/core/core.h"
#include "modules/player/player.h"
#include "lab-points-map.h"

/*****************************************************************************************************************************/

/*
 *  LabSession - object containing specific game session.
 *  On setup it's create labyrinth and spawn players on spawn points.
 */
typedef struct LabSession_s LabSession;

/*****************************************************************************************************************************/

bool          LabSession_IsFull(const LabSession *session);
LabPointsMap *LabSession_GetLabMap(LabSession *session);
Player       *LabSession_FindPlayer(LabSession *session, uint player_id);
bool          LabSession_AddPlayer(LabSession        *session, 
                                   uint              *added_player_id);

bool          LabSession_IsReadyForStart(const LabSession *session);
void          LabSession_Start(LabSession *session);

void          LabSession_Setup(LabSession *session, uint players_count);

LabSession   *LabSession_Create(void);
void          LabSession_Free(LabSession *session);

/*****************************************************************************************************************************/

#endif