#ifndef _COMMANDS_PROCESSOR_H_
#define _COMMANDS_PROCESSOR_H_

#include "command.h"
#include "lab-session.h"

#define _COMMANDRESPONSE_MAX_PAYLOAD_SIZE 	(256)

typedef struct CommandResponse
{
	CORE_Bool 	is_response_for_all_players;
	uint32 		session_index;
	uint32 		player_index;
	uint32 		payload_size;
	uint8		payload[_COMMANDRESPONSE_MAX_PAYLOAD_SIZE];
} CommandResponse;

CORE_Bool CommandsProcessor_Process(Command *command, LabSession sessions[], uint32 sessions_size, CommandResponse *out_command_response);

#endif