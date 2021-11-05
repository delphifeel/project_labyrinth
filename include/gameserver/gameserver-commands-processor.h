#ifndef _GAMESERVER_COMMANDS_PROCESSOR_H_
#define _GAMESERVER_COMMANDS_PROCESSOR_H_

#include <commands-processor.h>
#include "gameserver-command.h"
#include "gameserver-command-response.h"

COMMANDS_PROCESSOR_DEFINE(GameServerCommandsProcessor, 
						  struct GameServerCommand *, 
						  struct GameServerCommandResponse *)


#endif