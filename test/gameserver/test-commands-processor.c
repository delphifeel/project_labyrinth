#include "gameserver/commands-processor.h"

static void _FillBuffer(uint32 command_type, uint32 player_index, 
						uint32 session_index, const uint8 player_token[TOKEN_SIZE],
						const uint8 payload[], uint32 payload_size, uint8 buffer[])
{
	uint8 		*buffer_ptr;
	uint32 		validation_header;


	validation_header = 0xAFBEADDE;
	buffer_ptr = buffer;

	memcpy(buffer_ptr, &validation_header, 4);
	buffer_ptr += 4;
	memcpy(buffer_ptr, &command_type, 4);
	buffer_ptr += 4;
	memcpy(buffer_ptr, &player_index, 4);
	buffer_ptr += 4;
	memcpy(buffer_ptr, &session_index, 4);
	buffer_ptr += 4;
	memcpy(buffer_ptr, player_token, TOKEN_SIZE);
	buffer_ptr += TOKEN_SIZE;
	memcpy(buffer_ptr, payload, payload_size);
}

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
	Command 		command;
	uint8 			buffer[56];
	uint32  		player1_index;
	uint32  		player2_index;
	uint32 			session_index;
	uint8 			player_token[TOKEN_SIZE];
	uint32  		directions[2];
	LabSession 		sessions[1];

	session_index = 0;
	directions[0] = kMoveDirection_Left;
	directions[1] = 0;

	const uint32 player1_id = 32;
	const uint32 player2_id = 34;

	_InitSession(&sessions[0], player1_id, player2_id, &player1_index, &player2_index);

	_FillBuffer(kCommandType_PlayerMove, player1_index, session_index, player_token, 
				(const uint8 *) directions, sizeof(directions), buffer);


	Command_Init(&command);
	Command_ParseFromBuffer(&command, buffer, sizeof(buffer));

	// assert(CommandsProcessor_Process(&command, sessions, 1) == TRUE);

	_FreeSession(&sessions[0]);
}


void Test_CommandsProcessor()
{
	Test_CommandPlayerMove();
}
