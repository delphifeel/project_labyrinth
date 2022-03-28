
void Test_Command()
{
	// Command 		command;
	// uint8 			buffer[256];
	// uint8			*buffer_ptr;
	// uint32 			validation_header;
	// uint32 			command_type;
	// uint32 			player_index;
	// uint32 			session_index;
	// uint32 			temp_uint;
	// const uint8		*payload;
	// uint32 			payload_size;


	// validation_header = 0xAFBEADDE;
	// command_type = kCommandType_StartGame;
	// player_index = 3;
	// session_index = 97;

	// buffer_ptr = buffer;
	// memcpy(buffer_ptr, &validation_header, 4);
	// buffer_ptr += 4;
	// memcpy(buffer_ptr, &command_type, 4);
	// buffer_ptr += 4;
	// memcpy(buffer_ptr, &player_index, 4);
	// buffer_ptr += 4;
	// memcpy(buffer_ptr, &session_index, 4);
	// buffer_ptr += 4;
	// buffer_ptr += TOKEN_SIZE;
	// buffer_ptr[0] = 0;
	// buffer_ptr[1] = 1;
	// buffer_ptr[2] = 2;

	// Command_Init(&command);

	// CORE_Assert(Command_ParseFromBuffer(&command, buffer, sizeof(buffer)) == true);

	// Command_GetType(&command, &temp_uint);
	// CORE_Assert(temp_uint == command_type);

	// Command_GetSessionIndex(&command, &temp_uint);
	// CORE_Assert(temp_uint == session_index);

	// Command_GetPlayerIndex(&command, &temp_uint);
	// CORE_Assert(temp_uint == player_index);

	// Command_GetPayloadPtr(&command, &payload, &payload_size);
	// CORE_Assert(payload_size == sizeof(buffer) - 48);

	// CORE_Assert(payload[0] == 0);
	// CORE_Assert(payload[1] == 1);
	// CORE_Assert(payload[2] == 2);
}