#include "gameserver/gameserver-command.h"

/*****************************************************************************************************************************/

void GameServerCommand_GetType(struct GameServerCommand *instance, uint32 *out_command_type)
{
	Command_GetType(&instance->base_command, out_command_type);
}

void GameServerCommand_GetSessionsPtr(struct GameServerCommand *instance, LabSession *out_sessions_ptr[], uint32 *out_sessions_size)
{
	CORE_AssertPointer(out_sessions_ptr);
	CORE_AssertPointer(out_sessions_size);


	*out_sessions_ptr = instance->sessions;
	*out_sessions_size = instance->sessions_size;
}

void GameServerCommand_GetSessionIndex(struct GameServerCommand *instance, uint32 *out_session_index)
{
	CORE_AssertPointer(out_session_index);

	*out_session_index = instance->session_index;
}

void GameServerCommand_GetPlayerIndex(struct GameServerCommand *instance, uint32 *out_player_index)
{
	CORE_AssertPointer(out_player_index);

	*out_player_index = instance->player_index;
}

void GameServerCommand_GetPlayerTokenPtr(struct GameServerCommand *instance, const uint8 *out_player_token_ptr[TOKEN_SIZE])
{
	CORE_AssertPointer(out_player_token_ptr);

	*out_player_token_ptr = instance->player_token;
}

void GameServerCommand_GetPayloadPtr(struct GameServerCommand *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size)
{
	Command_GetPayloadPtr(&instance->base_command, out_payload_ptr, out_payload_size);
}

void GameServerCommand_SetType(struct GameServerCommand *instance, uint32 command_type)
{
	Command_SetType(&instance->base_command, command_type);
}

void GameServerCommand_SetSessionsPtr(struct GameServerCommand *instance, LabSession *sessions_ptr, uint32 sessions_size)
{
	CORE_AssertPointer(sessions_ptr);


	instance->sessions = sessions_ptr;
	instance->sessions_size = sessions_size;
}

void GameServerCommand_SetSessionIndex(struct GameServerCommand *instance, uint32 session_index)
{
	instance->session_index = session_index;
}

void GameServerCommand_SetPlayerIndex(struct GameServerCommand *instance, uint32 player_index)
{
	instance->player_index = player_index;
}

void GameServerCommand_SetPlayerToken(struct GameServerCommand *instance, const uint8 player_token[TOKEN_SIZE])
{
	CORE_AssertPointer(player_token);

	memcpy(instance->player_token, player_token, TOKEN_SIZE);
}

CORE_Bool GameServerCommand_SetPayload(struct GameServerCommand *instance, const uint8 payload[], uint32 payload_size)
{
	return Command_SetPayload(&instance->base_command, payload, payload_size);
}

void GameServerCommand_Init(struct GameServerCommand *instance)
{
	Command_Init(&instance->base_command);
	instance->session_index = 0;
	instance->player_index = 0;
	instance->sessions = NULL;
	instance->sessions_size = 0;
}
