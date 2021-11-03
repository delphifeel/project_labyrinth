#ifndef _COMMANDS_PROCESSOR_H_
#define _COMMANDS_PROCESSOR_H_

#include "CORE.h"
#include "command.h"

CORE_OBJECT_DEFINE(CommandsProcessor);

typedef CORE_Bool (*CommandProcessFunc)(struct Command 	*command, 
										struct Command 	*out_response_command,
										CORE_Bool 		*out_is_have_response);

typedef struct CommandToProcessFunc
{
	uint32 				command_type;
	CommandProcessFunc 	process_func;
} CommandToProcessFunc;

/*****************************************************************************************************************************/

CORE_Bool 	CommandsProcessor_Process(	CommandsProcessor 	instance, 
										struct Command 		*command, 
										struct Command 		*out_response_command,
										CORE_Bool 			*out_is_have_response);

void 		CommandsProcessor_Setup(CommandsProcessor instance, const CommandToProcessFunc *command_to_process_func_array);

void 		CommandsProcessor_Create(CommandsProcessor *instance_ptr);
void 		CommandsProcessor_Free(CommandsProcessor *instance_ptr);

#endif
