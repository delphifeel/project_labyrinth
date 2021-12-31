#ifndef _GAMESERVER_COMMAND_TYPES_H_
#define _GAMESERVER_COMMAND_TYPES_H_

#include "gameserver/gameserver-commands-processor.h"

enum 
{
	kCommandType_PlayerMove,
	kCommandType_StartGame,
	kCommandType_PlayerInit,
};

const GameServerCommandsProcessor_CommandToProcessFunc 	*GetGameServerCommandToProcessFunc();
uint32 													GetGameServerCommandToProcessFuncSize();

#endif
