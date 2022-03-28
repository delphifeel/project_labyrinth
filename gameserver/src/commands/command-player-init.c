#include "lib/commands-processor/command.h"
#include "gameserver/player.h"
#include "lib/commands-processor/command.h"
#include "gameserver/gameserver-command-response.h"
#include "gameserver/gameserver-command.h"
#include "gameserver/gameserver-command-types.h"

typedef struct PlayerInitPayload 
{
	uint32 	dummy;
} PlayerInitPayload;

typedef struct PlayerInitResponsePayload 
{
	PositionStruct 	position_coords;
	LabPointStruct 	position_point;
} PlayerInitResponsePayload;

bool CommandPlayerInit_Process(struct GameServerCommand 			*game_server_command, 
									struct GameServerCommandResponse	*out_response_command,
									bool 							*out_is_have_response)
{
	uint32 						session_index;
	uint32 						player_index;
	LabSession 					session;
	Player 						player;
	LabSession 					*sessions;
	uint32 						sessions_size;
	uint32 						payload_size;
	const uint8 				*payload_raw;	
	PlayerInitResponsePayload 	response_payload;


	GameServerCommand_GetSessionsPtr(game_server_command, &sessions, &sessions_size);
	GameServerCommand_GetSessionIndex(game_server_command, &session_index);
	GameServerCommand_GetPlayerIndex(game_server_command, &player_index);
	GameServerCommand_GetPayloadPtr(game_server_command, &payload_raw, &payload_size);

	if (payload_size != sizeof(PlayerInitPayload))
	{
		CORE_DebugError("payload_size != sizeof(PlayerInitPayload)\n");
		return false;
	}

	if (LabSession_HelperFindSession(sessions, 
							 		 sessions_size, 
							 		 session_index,
							 		 &session) == false)
	{
		return false;
	}

	if (LabSession_FindPlayer(session, player_index, &player) == false)
	{
		return false;
	}

	Player_GetPositionCoords(player, &response_payload.position_coords);
	Player_GetPositionPoint(player, &response_payload.position_point);

	*out_is_have_response = true;
	GameServerCommandResponse_SetType(out_response_command, kCommandType_PlayerInit);
	GameServerCommandResponse_AddPlayerIndex(out_response_command, player_index);
	if (GameServerCommandResponse_SetPayload(out_response_command, 
										 	(const uint8 *) &response_payload,
										 	sizeof(response_payload)) == false)
	{
		return false;
	}

	return true;
}