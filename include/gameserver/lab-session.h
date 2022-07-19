#ifndef _LAB_SESSION_H_
#define _LAB_SESSION_H_

#include "CCORE.h"
#include "player.h"
#include "lab-points-map.h"

/*****************************************************************************************************************************/

/*
 *  LabSession - object containing specific game session.
 *  On setup it's create labyrinth and spawn players on spawn points.
 */
typedef struct LabSession_s *LabSession;

/*****************************************************************************************************************************/

bool    LabSession_HelperFindSession(LabSession sessions[], uint32 sessions_size, uint32 index, LabSession *out_session);

LabPointsMap LabSession_GetLabMap(LabSession session);
bool    LabSession_AddPlayer(LabSession         instance, 
                             uint32             player_id, 
                             const uint8        player_token[TOKEN_SIZE], 
                             uint32             *out_added_player_index);

bool    LabSession_FindPlayer(LabSession instance, uint32 player_index, Player *out_player);
void    LabSession_Start(LabSession instance);

void    LabSession_MapToJSON(LabSession instance, char **json);
void    LabSession_Setup(LabSession instance, uint32 players_count);

void    LabSession_Create(LabSession* instance_ptr);
void    LabSession_Free(LabSession* instance_ptr);

/*****************************************************************************************************************************/

#endif
