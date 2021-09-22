#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include "CORE.h"

/*****************************************************************************************************************************/

CORE_OBJECT_DEFINE(GameServer);

/*****************************************************************************************************************************/

void GameServer_Process(GameServer);

void GameServer_Setup(GameServer instance);

void GameServer_Create(GameServer* instance_ptr);
void GameServer_Free(GameServer* instance_ptr);

/*****************************************************************************************************************************/

#endif
