#include "gameserver/player.h"
#include "gameserver/common.h"
#include "gameserver/command.h"

typedef struct PlayerMovePayload 
{
	uint32 	directions[2];
} PlayerMovePayload;

CORE_Bool CommandPlayerMove_Process(Command *command, LabSession sessions[], uint32 sessions_size)
{
	LabSession 					session;
	Player 						player;
	MoveDirection 				directions[2];
	uint32 						directions_size;
	uint32 						session_index;
	uint32 						player_index;
	const uint8					*payload_raw;
	const PlayerMovePayload 	*payload;


	Command_GetSessionIndex(command, &session_index);
	Command_GetPlayerIndex(command, &player_index);
	Command_GetPayload(command, &payload_raw);
	payload = (const PlayerMovePayload *) payload_raw;

	if (LabSession_HelperFindSession(sessions, 
							 		 sessions_size, 
							 		 session_index,
							 		 &session) == FALSE	)
	{
		return FALSE;
	}

	if (LabSession_FindPlayer(session, player_index, &player) == FALSE)
	{
		return FALSE;
	}

	if (payload->directions[1] == 0)
	{
		directions[0] = payload->directions[0];
		directions_size = 1;
	}
	else
	{
		directions[0] = payload->directions[0];
		directions[1] = payload->directions[1];
		directions_size = 2;
	}

	return Player_Move(player, directions, directions_size);
}
