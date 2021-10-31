#ifndef _COMMANDS_PROCESSOR_H_
#define _COMMANDS_PROCESSOR_H_

#include "CORE.h"

CORE_OBJECT_DEFINE(CommandsProcessor);

typedef CORE_Bool (*CommandProcessFunc)(const Command *command, const Command *out_response_command);

typedef struct CommandToProcessFunc
{
	const uint32 				command_type;
	const CommandProcessFunc 	process_func;
} CommandToProcessFunc;

/*****************************************************************************************************************************/

void 	CommandsProcessor_Process(CommandsProcessor instance, const Command *command, const Command *out_response_command);

void 	CommandsProcessor_Setup(CommandsProcessor instance, const CommandToProcessFunc *command_to_process_func_array);

void 	CommandsProcessor_Create(CommandsProcessor *instance_ptr);
void 	CommandsProcessor_Free(CommandsProcessor *instance_ptr);

#endif