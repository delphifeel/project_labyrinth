#include "commands-processor.h"
#include "gameserver/gameserver-command.h"
#include "gameserver/gameserver-command-types.h"

static void _InitSession(LabSession *out_session, 
						 uint32 player1_id, uint32 player2_id, 
						 uint32 *out_player1_index, uint32 *out_player2_index)
{
	uint8 				player1_token[TOKEN_SIZE];
	uint8 				player2_token[TOKEN_SIZE];
	uint32  			player1_token_value;
	uint32  			player2_token_value;
	LabSession 			session;


	player1_token_value = 0xBADBEE;
	player2_token_value = 0xDEADBEE;
	memcpy(player1_token, &player1_token_value, sizeof(player1_token_value));
	memcpy(player2_token, &player2_token_value, sizeof(player2_token_value));

	LabSession_Create(out_session);
	session = *out_session;
	LabSession_Setup(session, 2);

	LabSession_AddPlayer(session, player1_id, player1_token, out_player1_index);
	LabSession_AddPlayer(session, player2_id, player2_token, out_player2_index);
}

static void _FreeSession(LabSession *session_ptr)
{
	LabSession_Free(session_ptr);
}

/* 
 *		command `PlayerMove` contains:
 * 			- 0..4 		(4 bytes) 	-> validation header
 *			- 4..48 	(44 bytes)	-> command header
 *			- 48..52	(4 bytes)	-> direction #1	
 *			- 52..56	(4 bytes)	-> direction #2
 */
void Test_CommandPlayerMove()
{
	CommandsProcessor 				commands_processor;
	struct GameServerCommand 		command;
	struct GameServerCommand 		response_command;
	uint32  						player1_index;
	uint32  						player2_index;
	uint32 							session_index;
	uint8 							player_token[TOKEN_SIZE];
	uint32  						directions[2];
	LabSession 						sessions[1];

	session_index = 0;
	directions[0] = kMoveDirection_Left;
	directions[1] = 0;

	const uint32 player1_id = 32;
	const uint32 player2_id = 34;

	_InitSession(&sessions[0], player1_id, player2_id, &player1_index, &player2_index);


	GameServerCommand_Init(&command);
	GameServerCommand_SetType(&command, kCommandType_PlayerMove);
	GameServerCommand_SetPlayerIndex(&command, player1_index);
	GameServerCommand_SetSessionIndex(&command, session_index);
	GameServerCommand_SetSessionsPtr(&command, sessions, 1);
	GameServerCommand_SetPlayerToken(&command, player_token);
	GameServerCommand_SetPayload(&command, (const uint8 *) directions, sizeof(directions));



	CommandsProcessor_Create(&commands_processor);
	CommandsProcessor_Setup(commands_processor, GetGameServerCommandToProcessFunc());
	// CORE_Assert(CommandsProcessor_Process(commands_processor, (struct Command *) &command, (struct Command *) &response_command) == TRUE);

	_FreeSession(&sessions[0]);
	CommandsProcessor_Free(&commands_processor);
}


void Test_CommandsProcessor()
{
	Test_CommandPlayerMove();
}
