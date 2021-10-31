#include "command.h"

/*****************************************************************************************************************************/

void Command_GetType(Command *instance, uint32 *out_command_type)
{
	CORE_AssertPointer(out_command_type);

	*out_command_type = instance->header.type;
}

void Command_GetPayloadPtr(Command *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size)
{
	CORE_AssertPointer(instance->payload);
	CORE_AssertPointer(out_payload_ptr);
	CORE_AssertPointer(out_payload_size);

	*out_payload_ptr = instance->payload;
	*out_payload_size = instance->payload_size;
}

void Command_SetType(Command *instance, uint32 command_type)
{
	instance->type = command_type;
}

void Command_SetPayload(Command *instance, const uint8 payload[], uint32 payload_size);
{
	CORE_Assert((payload_size > 0) && (payload_size <= sizeof(instance->payload)));
	CORE_AssertPointer(payload);

	instance->payload_size = payload_size;
	memcpy(instance->payload, payload, payload_size);
}

/*****************************************************************************************************************************/

void Command_Init(Command *instance)
{
	CORE_MemZero(instance, sizeof(Command));
}

/*****************************************************************************************************************************/
