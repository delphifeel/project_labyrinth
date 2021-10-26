#include "gameserver/commands-processor.h"

/*****************************************************************************************************************************/

#define _DEFINE_PROCESS_FUNC(_NAME) 															\
	extern CORE_Bool _NAME(Command *command, LabSession sessions[], uint32 sessions_size) 		\

/*****************************************************************************************************************************/

typedef CORE_Bool (*CommandProcessFunc)(Command *command, LabSession sessions[], uint32 sessions_size);

typedef struct CommandProcessorStruct
{
	CommandType 		command_type;
	CommandProcessFunc 	process_cb;
} CommandProcessorStruct;

/*****************************************************************************************************************************/

_DEFINE_PROCESS_FUNC(CommandPlayerMove_Process);
_DEFINE_PROCESS_FUNC(CommandStartGame_Process);

static CommandProcessorStruct _commands_processors[] =
{
	{	kCommandType_PlayerMove, 	CommandPlayerMove_Process	},
	{	kCommandType_StartGame, 	CommandStartGame_Process	},
};

/*****************************************************************************************************************************/

CORE_Bool CommandsProcessor_Process(Command *command, LabSession sessions[], uint32 sessions_size)
{
	CommandType   			command_type;
    CommandProcessorStruct 	command_processor;


    Command_GetType(command, &command_type);

    command_processor = _commands_processors[command_type];
    if (command_processor.command_type != command_type)
    {
        CORE_DebugError("Wrong association in `_commands_processors`. Fix `_commands_processors` order to resolve\n");
        return FALSE;
    }

    if (command_processor.process_cb(command, sessions, sessions_size) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************************************************************/
