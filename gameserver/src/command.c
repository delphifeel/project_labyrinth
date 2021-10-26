#include "gameserver/command.h"

/*****************************************************************************************************************************/

#define _COMMANDHEADER_VALIDATION_ID 		(0xAFBEADDE) // DE AD BE AF in memory

#define _DEFINE_PROCESS_FUNC(_NAME) 															\
	extern CORE_Bool _NAME(Command *command, LabSession sessions[], uint32 sessions_size) 		\

/*****************************************************************************************************************************/

typedef CORE_Bool (*CommandProcessFunc)(Command *command, LabSession sessions[], uint32 sessions_size);

typedef struct CommandProcessorStruct
{
	CommandType 		command_type;
	CommandProcessFunc 	process_cb;
} CommandProcessorStruct;

struct CommandHeader
{
	uint32 			type;
	uint32			player_index;
	uint32			session_index;
	uint8 			player_token[TOKEN_SIZE];
};

/*****************************************************************************************************************************/

_DEFINE_PROCESS_FUNC(CommandPlayerMove_Process);
_DEFINE_PROCESS_FUNC(CommandStartGame_Process);

static CommandProcessorStruct _commands_processors[] =
{
	{	kCommandType_PlayerMove, 	CommandPlayerMove_Process	},
	{	kCommandType_StartGame, 	CommandStartGame_Process	},
};

/*****************************************************************************************************************************/

void Command_GetType(Command *instance, CommandType *out_command_type)
{
	CORE_AssertPointer(instance->_header);
	CORE_AssertPointer(out_command_type);

	*out_command_type = (CommandType) instance->_header->type;
}

void Command_GetSessionIndex(Command *instance, uint32 *out_session_index)
{
	CORE_AssertPointer(instance->_header);
	CORE_AssertPointer(out_session_index);

	*out_session_index = instance->_header->session_index;
}

void Command_GetPlayerIndex(Command *instance, uint32 *out_player_index)
{
	CORE_AssertPointer(instance->_header);
	CORE_AssertPointer(out_player_index);

	*out_player_index = instance->_header->player_index;
}

void Command_GetPayload(Command *instance, const uint8 **out_payload)
{
	CORE_AssertPointer(instance->_payload);
	CORE_AssertPointer(out_payload);

	*out_payload = instance->_payload;
}

/*****************************************************************************************************************************/

CORE_Bool Command_ParseFromBuffer(Command *instance, const uint8 *buffer_ptr)
{
	uint32 validation_header;


	// 0...4 	(4 bytes) 	- validation header
	validation_header = *((uint32 *) buffer_ptr);
	if (validation_header != _COMMANDHEADER_VALIDATION_ID)
	{
		CORE_DebugError("no validation header - `buffer` is not a command\n");
		return FALSE;
	}

	// 4...48 	(44 bytes)	- command header
	instance->_header = (CommandHeader *) (buffer_ptr + 4);

	// 48...~				- command payload
	instance->_payload = (uint8 *) (buffer_ptr + 4 + sizeof(CommandHeader));

	return TRUE;
}

void Command_Process(Command *instance, LabSession sessions[], uint32 sessions_size)
{
	CORE_AssertPointer(instance->_header);
	CORE_AssertPointer(instance->_payload);

	CommandType   			command_type;
    CommandProcessorStruct 	command_processor;


    Command_GetType(instance, &command_type);

    command_processor = _commands_processors[command_type];
    if (command_processor.command_type != command_type)
    {
        CORE_DebugAbort("Wrong association in `_commands_processors`. Fix `_commands_processors` order to resolve\n");
        return;
    }

    if (command_processor.process_cb(instance, sessions, sessions_size) == FALSE)
    {
        CORE_DebugError("Command processing error\n");
        return;
    }
}

void Command_Init(Command *instance)
{
	instance->_header = NULL;
	instance->_payload = NULL;
}

/*****************************************************************************************************************************/
