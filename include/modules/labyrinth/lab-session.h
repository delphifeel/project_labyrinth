#ifndef _LAB_SESSION_H_
#define _LAB_SESSION_H_

#include "CCORE.h"
#include "modules/player/player.h"
#include "lab-points-map.h"

/*****************************************************************************************************************************/

/*
 *  LabSession - object containing specific game session.
 *  On setup it's create labyrinth and spawn players on spawn points.
 */
typedef struct LabSession_s LabSession;

/*****************************************************************************************************************************/

bool          LabSession_HelperFindSession(const LabSession *sessions[], 
                                           uint              sessions_size, 
                                           uint              index, 
                                           const LabSession **out_session);

LabPointsMap *LabSession_GetLabMap(LabSession *session);
Player       *LabSession_FindPlayer(LabSession *session, uint player_id);
bool          LabSession_AddPlayer(LabSession        *session, 
                                   const uint8        player_token[PLAYER_TOKEN_SIZE], 
                                   uint              *added_player_id);

void          LabSession_Start(LabSession *session);

void          LabSession_MapToJSON(LabSession *session, char **json);
void          LabSession_Setup(LabSession *session, uint players_count);

LabSession   *LabSession_Create(void);
void          LabSession_Free(LabSession *session);

/*****************************************************************************************************************************/

#endif