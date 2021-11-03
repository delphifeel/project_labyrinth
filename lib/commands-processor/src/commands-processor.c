#include "commands-processor.h"

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(CommandsProcessor,
	const CommandToProcessFunc     *command_to_process_func_array;
    uint32                         command_to_process_func_array_size;
);

/*****************************************************************************************************************************/

CORE_Bool CommandsProcessor_Process(CommandsProcessor instance, 
                                    struct Command *command, 
                                    struct Command *out_response_command,
                                    CORE_Bool      *out_is_have_response)
{
	CORE_AssertPointer(instance->command_to_process_func_array);

	uint32   					command_type;
    CommandToProcessFunc 		command_to_process_func;


    Command_GetType(command, &command_type);

    if (command_type >= instance->command_to_process_func_array_size)
    {
    	CORE_DebugError("Command type doesn't exists\n");
    	return FALSE;
    }

    CORE_DebugInfo("Processing command %u\n", command_type);

    command_to_process_func = instance->command_to_process_func_array[command_type];
    if (command_to_process_func.command_type != command_type)
    {
        CORE_DebugError("Wrong order in `command_to_process_func_array`. `command` index is not %u\n", command_type);
        return FALSE;
    }

    if (command_to_process_func.process_func(command, out_response_command, out_is_have_response) == FALSE)
    {
        CORE_DebugError("Command %u processing error\n", command_type);
        return FALSE;
    }

    CORE_DebugInfo("Processing completed %u\n", command_type);
    return TRUE;
}

void CommandsProcessor_Setup(   CommandsProcessor           instance, 
                                const CommandToProcessFunc  *command_to_process_func_array,
                                uint32                      command_to_process_func_array_size)
{
	CORE_AssertPointer(command_to_process_func_array);
	CORE_Assert(command_to_process_func_array_size > 0);

	instance->command_to_process_func_array = command_to_process_func_array;
	instance->command_to_process_func_array_size = command_to_process_func_array_size;
}

/*****************************************************************************************************************************/

void CommandsProcessor_Create(CommandsProcessor *instance_ptr)
{
	CommandsProcessor instance;


	CORE_OBJECT_CREATE(instance_ptr, CommandsProcessor);
	instance = *instance_ptr;

	instance->command_to_process_func_array = NULL;
}

void CommandsProcessor_Free(CommandsProcessor *instance_ptr)
{
	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/

