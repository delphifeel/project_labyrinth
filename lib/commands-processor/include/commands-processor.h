#ifndef _COMMANDS_PROCESSOR_H_
#define _COMMANDS_PROCESSOR_H_

#include "CORE.h"
#include "command.h"

CORE_OBJECT_DEFINE(CommandsProcessor);

typedef CORE_Bool (*CommandProcessFunc)(Command *command, Command *out_response_command);

typedef struct CommandToProcessFunc
{
	uint32 				command_type;
	CommandProcessFunc 	process_func;
} CommandToProcessFunc;

/*****************************************************************************************************************************/

CORE_Bool 	CommandsProcessor_Process(CommandsProcessor instance, Command *command, Command *out_response_command);

void 		CommandsProcessor_Setup(CommandsProcessor instance, const CommandToProcessFunc *command_to_process_func_array);

void 		CommandsProcessor_Create(CommandsProcessor *instance_ptr);
void 		CommandsProcessor_Free(CommandsProcessor *instance_ptr);

#endif