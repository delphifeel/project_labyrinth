#ifndef _GAME_SERVER_H_
#define _GAME_SERVER_H_

#include "CCORE.h"

/*****************************************************************************************************************************/

typedef struct GameServer_s *GameServer;

/*****************************************************************************************************************************/

/*
 * 			Init new session. After init 
 *			need to add all players related to this session
 *			and start session.
 */
/* bool 	GameServer_InitNewSession(GameServer instance, uint32 *out_session_index);
   bool	GameServer_AddPlayerToSession(GameServer instance, uint32 session_index, uint32 player_id, uint32 *out_player_index);
   bool 	GameServer_StartSession(GameServer instance, uint32 session_index);
*/

void 		GameServer_Process(GameServer instance);
void 		GameServer_Setup(GameServer instance);

void 		GameServer_Create(GameServer* instance_ptr);
void 		GameServer_Free(GameServer* instance_ptr);

/*****************************************************************************************************************************/

#endif
