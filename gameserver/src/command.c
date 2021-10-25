#include "gameserver/command.h"
#include "gameserver/commands/command-player-move.h"

/*****************************************************************************************************************************/

#define _COMMANDHEADER_VALIDATION_ID 		(0xDEADBEAF)

/*****************************************************************************************************************************/

CommandProcessorStruct CommandsProcessors[] =
{
	{kCommandType_PlayerMove, CommandPlayerMove_Process},
};

/*****************************************************************************************************************************/

struct CommandHeader
{
	uint32 			type;
	uint32			player_index;
	uint32			session_index;
	uint8 			player_token[32];
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

void Command_GetPayload(Command *instance, uint8 **out_payload)
{
	CORE_AssertPointer(instance->_payload);
	CORE_AssertPointer(out_payload);

	*out_payload = instance->_payload;
}

/*****************************************************************************************************************************/

CORE_Bool Command_ParseFromBuffer(Command *instance, const uint8 buffer[])
{
	uint32 validation_header;


	// 0...4 	(4 bytes) 	- validation header
	validation_header = *((uint32 *) buffer);
	if (validation_header != _COMMANDHEADER_VALIDATION_ID)
	{
		CORE_DebugError("no validation header - `buffer` is not a command\n");
		return FALSE;
	}

	// 4...48 	(44 bytes)	- command header
	instance->_header = (CommandHeader *) buffer + 4;

	// 48...~				- command payload
	instance->_payload = (uint8 *) buffer + 4 + sizeof(CommandHeader);

	return TRUE;
}

void Command_Init(Command *instance)
{
	instance->_header = NULL;
	instance->_payload = NULL;
}

/*****************************************************************************************************************************/
