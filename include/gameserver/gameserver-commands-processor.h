#ifndef _GAMESERVER_COMMANDS_PROCESSOR_H_
#define _GAMESERVER_COMMANDS_PROCESSOR_H_

#include "lib/commands-processor/commands-processor.h"
#include "gameserver/gameserver-command.h"
#include "gameserver/gameserver-command-response.h"

COMMANDS_PROCESSOR_DEFINE(GameServerCommandsProcessor, 
						  struct GameServerCommand *, 
						  struct GameServerCommandResponse *)


#endif