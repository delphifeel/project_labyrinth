#ifndef _GAMESERVER_COMMAND_PRIVATE_H_
#define _GAMESERVER_COMMAND_PRIVATE_H_

#include "common.h"
#include "../../lib/commands-processor/include/command.h"
#include "gameserver/lab-session.h"

CORE_OBJECT_DEFINE_AS_STRUCT(GameServerCommand,
	struct Command 	base_command;

	uint32 			session_index;
	uint32 			player_index;
	uint8 			player_token[TOKEN_SIZE];
	uint32 			sessions_size;
	LabSession 		*sessions;
);


#endif
