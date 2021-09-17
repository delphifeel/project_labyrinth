#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include "CORE.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(GameServer);

/*****************************************************************************************************************************/

void GameServer_Process(GameServer);

void GameServer_Setup(GameServer);

void GameServer_Create(GameServer*);
void GameServer_Free(GameServer*);

/*****************************************************************************************************************************/

#endif
