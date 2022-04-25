#ifndef _GAMESERVER_COMMAND_RESPONSE_PRIVATE_H_
#define _GAMESERVER_COMMAND_RESPONSE_PRIVATE_H_

#include <lib/commands-processor/command.h>
#include "gameserver/config.h"

#define _GAMESERVERCOMMANDRESPONSE__ARRAY_MAX_SIZE 	(SESSION_PLAYERS_COUNT)

struct GameServerCommandResponse
{
	struct Command 	base_command;

	uint32 			player_index_array_size;
	uint32 			player_index_array[_GAMESERVERCOMMANDRESPONSE__ARRAY_MAX_SIZE];
};


#endif
