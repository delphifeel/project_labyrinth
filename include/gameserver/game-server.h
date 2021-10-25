#ifndef _GAME_SERVER_H_
#define _GAME_SERVER_H_

#include "CORE.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(GameServer);

/*****************************************************************************************************************************/

/*
 * 			Init new session. After init 
 *			need to add all players related to this session
 *			and start session.
 */
// CORE_Bool 	GameServer_InitNewSession(GameServer instance, uint32 *out_session_index);
// CORE_Bool	GameServer_AddPlayerToSession(GameServer instance, uint32 session_index, uint32 player_id, uint32 *out_player_index);
// CORE_Bool 	GameServer_StartSession(GameServer instance, uint32 session_index);

void 		GameServer_Process(GameServer instance);
void 		GameServer_Setup(GameServer instance);

void 		GameServer_Create(GameServer* instance_ptr);
void 		GameServer_Free(GameServer* instance_ptr);

/*****************************************************************************************************************************/

#endif
