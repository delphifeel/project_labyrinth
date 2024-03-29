#ifndef _GAMESERVER_COMMAND_PRIVATE_H_
#define _GAMESERVER_COMMAND_PRIVATE_H_

#include <lib/commands-processor/command.h>
#include "common.h"
#include "gameserver/lab-session.h"

struct GameServerCommand
{
	struct Command 	base_command;

	uint32 			session_index;
	uint32 			player_index;
	uint8 			player_token[TOKEN_SIZE];
	uint32 			sessions_size;
	LabSession 		*sessions;
};


#endif
