#ifndef _GAMESERVER_COMMAND_TYPES_H_
#define _GAMESERVER_COMMAND_TYPES_H_

#include "gameserver/gameserver-commands-processor.h"

enum 
{
	kCommandType_PlayerMove,
	kCommandType_StartGame,
};

enum 
{
	kCommandResponseType_PlayerMoved,
};

const GameServerCommandsProcessor_CommandToProcessFunc 	*GetGameServerCommandToProcessFunc();
uint32 													GetGameServerCommandToProcessFuncSize();

#endif
