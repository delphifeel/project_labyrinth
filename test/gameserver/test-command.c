#include "gameserver/command.h"

void Test_Command()
{
	Command 		command;
	uint8 			buffer[256];
	uint8			*buffer_ptr;
	uint32 			validation_header;
	uint32 			command_type;
	uint32 			player_index;
	uint32 			session_index;
	uint32 			temp_uint;
	const uint8		*payload;


	validation_header = 0xAFBEADDE;
	command_type = kCommandType_StartGame;
	player_index = 3;
	session_index = 97;

	buffer_ptr = buffer;
	memcpy(buffer_ptr, &validation_header, 4);
	buffer_ptr += 4;
	memcpy(buffer_ptr, &command_type, 4);
	buffer_ptr += 4;
	memcpy(buffer_ptr, &player_index, 4);
	buffer_ptr += 4;
	memcpy(buffer_ptr, &session_index, 4);
	buffer_ptr += 4;
	buffer_ptr += TOKEN_SIZE;
	buffer_ptr[0] = 0;
	buffer_ptr[1] = 1;
	buffer_ptr[2] = 2;

	Command_Init(&command);
	assert(Command_ParseFromBuffer(&command, buffer));

	Command_GetType(&command, &temp_uint);
	assert(temp_uint == command_type);

	Command_GetSessionIndex(&command, &temp_uint);
	assert(temp_uint == session_index);

	Command_GetPlayerIndex(&command, &temp_uint);
	assert(temp_uint == player_index);

	Command_GetPayload(&command, &payload);

	assert(payload[0] == 0);
	assert(payload[1] == 1);
	assert(payload[2] == 2);
}