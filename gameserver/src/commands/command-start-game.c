#include "lib/commands-processor/command.h"
#include "gameserver/CONFIG.h"
#include "gameserver/player.h"
#include "lib/commands-processor/command.h"
#include "gameserver/gameserver-command-response.h"
#include "gameserver/gameserver-command.h"
#include "gameserver/gameserver-command-types.h"

typedef struct StartGamePayload 
{
	struct 
	{
		uint32 	player_id;
		uint8 	player_token[TOKEN_SIZE];	
	} players[SESSION_PLAYERS_COUNT];
} StartGamePayload;

typedef struct StartGameResponsePayload 
{
	struct 
	{
		uint32 	player_id;
		uint32 	player_index;	
		uint32 	session_index;
	} players[SESSION_PLAYERS_COUNT];
} StartGameResponsePayload;

bool CommandStartGame_Process(	struct GameServerCommand 			*game_server_command, 
									struct GameServerCommandResponse 	*out_response_command,
									bool 							*out_is_have_response)
{
	LabSession 					*sessions;
	uint32 						sessions_size;
	const uint8 				*payload_raw;
	const StartGamePayload 		*payload;
	uint32 						payload_size;


	GameServerCommand_GetSessionsPtr(game_server_command, &sessions, &sessions_size);
	GameServerCommand_GetPayloadPtr(game_server_command, &payload_raw, &payload_size);

	if (payload_size != sizeof(StartGamePayload))
	{
		CORE_DebugError("payload_size != sizeof(StartGamePayload)\n");
		return false;
	}

	payload = (const StartGamePayload *) payload_raw;


	/**
	 * 			find free session
	 */
	LabSession 	new_session;
	bool 	found_free_session;
	uint32  	new_session_index;


	found_free_session = false;

	for (uint32 i = 0; i < sessions_size; i++)
	{
		if (sessions[i] != NULL)
		{
			continue;
		}

		found_free_session = true;
		new_session_index = i;
		CORE_DebugInfo("Found free session (index %u)\n", new_session_index);
		break;
	}

	if (found_free_session == false)
	{
		CORE_DebugError("No free session.\n");
		return false;
	}

	LabSession_Create(&new_session);
	LabSession_Setup(new_session, SESSION_PLAYERS_COUNT);

	sessions[new_session_index] = new_session;
	

	/**
	 * 			add all players to created session
	 */
	uint32 						current_player_id;
	const uint8 				*current_player_token;
	uint32 						current_player_index;
	StartGameResponsePayload 	response_payload;


	for (uint32 i = 0; i < SESSION_PLAYERS_COUNT; i++)
	{
		current_player_id = payload->players[i].player_id; 
		if (current_player_id == 0)
		{
			CORE_DebugError("Player #%u have id = 0\n", i);
			break;
		}
		current_player_token = payload->players[i].player_token; 

		LabSession_AddPlayer(new_session, current_player_id, current_player_token, &current_player_index);

		response_payload.players[i].player_id = current_player_id;
		response_payload.players[i].player_index = current_player_index;
		response_payload.players[i].session_index = new_session_index;
	}

	/**
	 * 			set response
	 */
	*out_is_have_response = true;
	GameServerCommandResponse_SetType(out_response_command, kCommandType_StartGame);
	if (GameServerCommandResponse_SetPayload(out_response_command, 
										 	(const uint8 *) &response_payload,
										 	sizeof(response_payload)) == false)
	{
		return false;
	}

	/**
	 * 			start session
	 */
	LabSession_Start(new_session);

	return true;
}
