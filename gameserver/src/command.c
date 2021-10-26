#include "gameserver/command.h"

/*****************************************************************************************************************************/

#define _COMMANDHEADER_VALIDATION_ID 		(0xAFBEADDE) // DE AD BE AF in memory

/*****************************************************************************************************************************/

void Command_GetType(Command *instance, CommandType *out_command_type)
{
	CORE_Assert(instance->is_header_set == TRUE);
	CORE_AssertPointer(out_command_type);

	*out_command_type = (CommandType) instance->header.type;
}

void Command_GetSessionIndex(Command *instance, uint32 *out_session_index)
{
	CORE_Assert(instance->is_header_set == TRUE);
	CORE_AssertPointer(out_session_index);

	*out_session_index = instance->header.session_index;
}

void Command_GetPlayerIndex(Command *instance, uint32 *out_player_index)
{
	CORE_Assert(instance->is_header_set == TRUE);
	CORE_AssertPointer(out_player_index);

	*out_player_index = instance->header.player_index;
}

void Command_GetPayloadPtr(Command *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size)
{
	CORE_AssertPointer(instance->payload);
	CORE_AssertPointer(out_payload_ptr);
	CORE_AssertPointer(out_payload_size);

	*out_payload_ptr = instance->payload;
	*out_payload_size = instance->payload_size;
}

/*****************************************************************************************************************************/

CORE_Bool Command_ParseFromBuffer(Command *instance, const uint8 buffer[], uint32 buffer_size)
{
	CORE_AssertPointer(buffer);

	uint32 			validation_header;
	const uint8 	*buffer_ptr;
	uint32 			buffer_size_left;


	buffer_ptr = buffer;
	buffer_size_left = buffer_size;

	// 0...4 	(4 bytes) 	- validation header
	validation_header = *((uint32 *) buffer_ptr);
	if (validation_header != _COMMANDHEADER_VALIDATION_ID)
	{
		CORE_DebugError("no validation header - `buffer` is not a command\n");
		return FALSE;
	}
	buffer_ptr += 4;
	buffer_size_left -= 4;

	// 4...48 	(44 bytes)	- command header
	memcpy(&instance->header, buffer_ptr, sizeof(_CommandHeader));
	buffer_ptr += sizeof(_CommandHeader);
	buffer_size_left -= sizeof(_CommandHeader);
	instance->is_header_set = TRUE;

	// 48...~				- command payload
	if (buffer_size_left > _COMMAND_MAX_PAYLOAD_SIZE)
	{
		CORE_DebugError("buffer size > command max payload size\n");
		return FALSE;
	}

	instance->payload_size = buffer_size_left;
	memcpy(&instance->payload, buffer_ptr, instance->payload_size);

	return TRUE;
}

void Command_Init(Command *instance)
{
	CORE_MemZero(instance, sizeof(Command));
}

/*****************************************************************************************************************************/
