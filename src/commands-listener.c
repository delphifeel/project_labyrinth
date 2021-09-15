#include "../include/commands-listener.h"

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(CommandsListener,
	uint32 		sessions_size;
	LabSession 	*sessions;
);

typedef struct CommandParserStruct
{
	CommandType command_type;
	CORE_Bool (*func)(CommandsListener instance, CommandStruct *command_to_process);
} CommandParserStruct;

/*****************************************************************************************************************************/

static CORE_Bool CommandsListener_HelperFindSession(LabSession *sessions, uint32 sessions_size, uint32 index, LabSession *out_session)
{
	if (index + 1 > sessions_size)
	{
		CORE_DebugError("Session index out of bounds\n");
		return FALSE;
	}

	*out_session = sessions[index];
	return TRUE;
}

/*****************************************************************************************************************************/

static CORE_Bool CommandsListener_ProcessPlayerMove(CommandsListener instance, CommandStruct *command_to_process)
{
	LabSession 	session;
	Player 		player;


	if (CommandsListener_HelperFindSession(instance->sessions, 
										   instance->sessions_size, 
										   command_to_process->session_index,
										   &session) == FALSE)
		return FALSE;

	if (LabSession_FindPlayer(session, command_to_process->player_index, &player) == FALSE)
		return FALSE;

	return Player_Move(player, 
					   command_to_process->player_move_payload.directions, 
					   command_to_process->player_move_payload.directions_size);
}

static CommandParserStruct command_to_function[] = 
{
	{kCommandType_PlayerMove, CommandsListener_ProcessPlayerMove},
};

static uint32 command_to_function_size = sizeof(command_to_function) / sizeof(CommandParserStruct);

/*****************************************************************************************************************************/

CORE_Bool CommandsListener_Process(CommandsListener instance, CommandStruct *command_to_process)
{
	CommandParserStruct command_parser;


	if (command_to_process->type + 1 > command_to_function_size)
	{
		CORE_DebugError("Unknown command type\n");
		return FALSE;
	}

	command_parser = command_to_function[command_to_process->type];
	if (command_parser.command_type != command_to_process->type)
	{
		CORE_DebugError("Wrong association in command_to_function. Fix command_to_function to resolve\n");
		return FALSE;
	}

	return command_parser.func(instance, command_to_process);
}

void CommandsListener_Setup(CommandsListener instance, LabSession *sessions, uint32 sessions_size)
{
	instance->sessions_size = sessions_size;
	instance->sessions = sessions;
}

/*****************************************************************************************************************************/

void CommandsListener_Create(CommandsListener *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, CommandsListener);
}

void CommandsListener_Free(CommandsListener *instance_ptr)
{
	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
