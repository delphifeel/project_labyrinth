#include "gameserver/gameserver-command-response.h"

/*****************************************************************************************************************************/

void GameServerCommandResponse_GetType(struct GameServerCommandResponse *instance, uint32 *out_command_type)
{
	Command_GetType(&instance->base_command, out_command_type);
}

void GameServerCommandResponse_GetPlayerIndexesPtr(struct GameServerCommandResponse 	*instance, 
												   const uint32 						**out_player_indexes_ptr,
												   uint32 								*out_player_indexes_size)
{
	CORE_AssertPointer(out_player_indexes_ptr);
	CORE_AssertPointer(out_player_indexes_size);

	*out_player_indexes_ptr = instance->player_index_array;
	*out_player_indexes_size = instance->player_index_array_size;
}

void GameServerCommandResponse_GetPayloadPtr(struct GameServerCommandResponse *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size)
{
	Command_GetPayloadPtr(&instance->base_command, out_payload_ptr, out_payload_size);
}

void GameServerCommandResponse_SetType(struct GameServerCommandResponse *instance, uint32 command_type)
{
	Command_SetType(&instance->base_command, command_type);
}

CORE_Bool GameServerCommandResponse_AddPlayerIndex(struct GameServerCommandResponse *instance, uint32 player_index)
{
	if (instance->player_index_array_size >= _GAMESERVERCOMMANDRESPONSE__ARRAY_MAX_SIZE)
	{
		CORE_DebugError("Players index array is full\n");
		return FALSE;
	}

	instance->player_index_array[instance->player_index_array_size++] = player_index;
	return TRUE;
}

CORE_Bool GameServerCommandResponse_SetPayload(struct GameServerCommandResponse *instance, const uint8 payload[], uint32 payload_size)
{
	return Command_SetPayload(&instance->base_command, payload, payload_size);
}

void GameServerCommandResponse_Init(struct GameServerCommandResponse *instance)
{
	Command_Init(&instance->base_command);
	instance->player_index_array_size = 0;
}
