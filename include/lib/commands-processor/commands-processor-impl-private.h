#ifndef _COMMANDS_PROCESSOR_IMPL_PRIVATE_H_
#define _COMMANDS_PROCESSOR_IMPL_PRIVATE_H_

#include "commands-processor-private.h"

/*****************************************************************************************************************************/

#define _COMMANDS_PROCESSOR_IMPLEMENTATION(_NAME, _IN_COMMAND_TYPE, _OUT_COMMAND_TYPE)                                  \
                                                                                                                        \
CORE_OBJECT_INTERFACE(_NAME,                                                                                            \
	const _NAME##_CommandToProcessFunc     *command_to_process_func_array;                                              \
    uint32                                 command_to_process_func_array_size;                                          \
);                                                                                                                      \
                                                                                                                        \
/********************************************************************************************************************/  \
                                                                                                                        \
bool _NAME##_Process(  _NAME               instance,                                                               \
                            uint32              command_type,                                                           \
                            _IN_COMMAND_TYPE    command,                                                                \
                            _OUT_COMMAND_TYPE   out_response_command,                                                   \
                            bool           *out_is_have_response)                                                  \
{                                                                                                                       \
	CORE_AssertPointer(instance->command_to_process_func_array);                                                        \
                                                                                                                        \
    _NAME##_CommandToProcessFunc 		 command_to_process_func;                                                       \
                                                                                                                        \
                                                                                                                        \
    if (command_type >= instance->command_to_process_func_array_size)                                                   \
    {                                                                                                                   \
    	CORE_DebugError("Command type doesn't exists\n");                                                               \
    	return false;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    command_to_process_func = instance->command_to_process_func_array[command_type];                                    \
    if (command_to_process_func.command_type != command_type)                                                           \
    {                                                                                                                   \
        CORE_DebugError("Wrong order in `command_to_process_func_array`. `command` index is not %u\n", command_type);   \
        return false;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    if (command_to_process_func.process_func(command, out_response_command, out_is_have_response) == false)             \
    {                                                                                                                   \
        CORE_DebugError("Command %u processing error\n", command_type);                                                 \
        return false;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    return true;                                                                                                        \
}                                                                                                                       \
                                                                                                                        \
void _NAME##_Setup(_NAME                                instance,                                                       \
                   const _NAME##_CommandToProcessFunc   *command_to_process_func_array,                                 \
                   uint32                               command_to_process_func_array_size)                             \
{                                                                                                                       \
	CORE_AssertPointer(command_to_process_func_array);                                                                  \
	CORE_Assert(command_to_process_func_array_size > 0);                                                                \
                                                                                                                        \
	instance->command_to_process_func_array = command_to_process_func_array;                                            \
	instance->command_to_process_func_array_size = command_to_process_func_array_size;                                  \
}                                                                                                                       \
                                                                                                                        \
/********************************************************************************************************************/  \
                                                                                                                        \
void _NAME##_Create(_NAME *instance_ptr)                                                                                \
{                                                                                                                       \
	_NAME instance;                                                                                                     \
                                                                                                                        \
                                                                                                                        \
	CORE_OBJECT_CREATE(instance_ptr, _NAME);                                                                            \
	instance = *instance_ptr;                                                                                           \
                                                                                                                        \
	instance->command_to_process_func_array = NULL;                                                                     \
}                                                                                                                       \
                                                                                                                        \
void _NAME##_Free(_NAME *instance_ptr)                                                                                  \
{                                                                                                                       \
	CORE_OBJECT_FREE(instance_ptr);                                                                                     \
}                                                                                                                       \

/********************************************************************************************************************/

#endif
