#include "gameserver/gameserver-command-types.h"
#include "gameserver/gameserver-commands-processor.h"

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
	GameServerCommandsProcessor 			commands_processor;
	struct GameServerCommand 				command;
	struct GameServerCommandResponse 		response_command;
	uint32  								player1_index;
	uint32  								player2_index;
	uint32 									session_index;
	uint8 									player_token[TOKEN_SIZE];
	uint32  								directions[2];
	LabSession 								sessions[1];

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



	GameServerCommandsProcessor_Create(&commands_processor);
	GameServerCommandsProcessor_Setup(commands_processor, GetGameServerCommandToProcessFunc(), GetGameServerCommandToProcessFuncSize());
	// CORE_Assert(GameServerCommandsProcessor_Process(commands_processor, co(struct Command *) &command, (struct Command *) &response_command) == TRUE);

	_FreeSession(&sessions[0]);
	GameServerCommandsProcessor_Free(&commands_processor);
}

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
	} players[SESSION_PLAYERS_COUNT];
} StartGameResponsePayload;

/* 
 *		command `StartGame` contains:
 * 			- 0..4 		(4 bytes) 	-> validation header
 *			- 4..48 	(44 bytes)	-> command header
 *			- 48..~ 				-> command payload
 */
void Test_CommandStartGame()
{
	GameServerCommandsProcessor 			commands_processor;
	struct GameServerCommand 				command;
	struct GameServerCommandResponse 		response_command;
	StartGamePayload 						payload;
	const StartGameResponsePayload 			*response_payload_ptr;
	uint32 									response_payload_size;
	CORE_Bool  								is_have_response;
	uint32 									command_type;
	LabSession  							sessions[1];


	CORE_MemZero(sessions, sizeof(sessions));

	// set payload
    payload.players[0].player_id = 1;
    memcpy(payload.players[0].player_token, gameserver_mocked_token, TOKEN_SIZE);

    /*payload.players[1].player_id = 2;
    memcpy(payload.players[1].player_token, gameserver_mocked_token, TOKEN_SIZE);

    payload.players[2].player_id = 3;
    memcpy(payload.players[2].player_token, gameserver_mocked_token, TOKEN_SIZE);

    payload.players[3].player_id = 4;
    memcpy(payload.players[3].player_token, gameserver_mocked_token, TOKEN_SIZE);*/


    command_type = kCommandType_StartGame;

	GameServerCommand_Init(&command);
	GameServerCommand_SetType(&command, command_type);
	GameServerCommand_SetSessionsPtr(&command, sessions, 1);
	GameServerCommand_SetPayload(&command, (const uint8 *) &payload, sizeof(payload));

	is_have_response = FALSE;

	GameServerCommandsProcessor_Create(&commands_processor);
	GameServerCommandsProcessor_Setup(commands_processor, GetGameServerCommandToProcessFunc(), GetGameServerCommandToProcessFuncSize());
	CORE_Assert(
		GameServerCommandsProcessor_Process(commands_processor,
											command_type, 
											&command, 
											&response_command,
											&is_have_response) == TRUE
	);

	CORE_Assert(is_have_response == TRUE);

	GameServerCommandResponse_GetType(&response_command, &command_type);
	CORE_Assert(command_type == kCommandType_StartGame);
	GameServerCommandResponse_GetPayloadPtr(&response_command, 
											(const uint8 **) &response_payload_ptr,
											&response_payload_size);
	CORE_Assert(response_payload_size == sizeof(StartGameResponsePayload));

	CORE_Assert(response_payload_ptr->players[0].player_id == 1);
	CORE_Assert(response_payload_ptr->players[0].player_index == 0);

	GameServerCommandsProcessor_Free(&commands_processor);
}


void Test_CommandsProcessor()
{
	Test_CommandPlayerMove();
	Test_CommandStartGame();
}
