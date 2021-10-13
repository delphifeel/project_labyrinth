#include "gameserver/game-server.h"
#include "gameserver/lab-session.h"
#include "gameserver/commands-listener.h"
#include "gameserver/in-out-system.h"

/*****************************************************************************************************************************/

#define SESSIONS_SIZE_CAPACITY 	(10)
#define SESSION_PLAYERS_COUNT	(40)

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(GameServer,
	LabSession 				sessions[SESSIONS_SIZE_CAPACITY];
	CommandsListener 		commands_listener;
	InOutSystem 			in_out_system;
);

/*****************************************************************************************************************************/

static void INTERNAL_PrepareSessions(GameServer instance)
{
	CORE_MemZero(instance->sessions, sizeof(instance->sessions));
}

static void INTERNAL_FreeSessions(GameServer instance)
{
	for (uint32 i = 0; i < SESSIONS_SIZE_CAPACITY; i++)
	{
		if (instance->sessions[i] != NULL)
		{
			LabSession_Free(&instance->sessions[i]);
		}
	}
}

static void INTERNAL_OnIOSystemRead(void *context, uint8 data[], uint32 data_size)
{
	CORE_Assert(data_size == sizeof(CommandStruct));

	CommandStruct 	*command_ptr;
	GameServer 		game_server;


	game_server = (GameServer) context;

	// convert data to CommandStruct
	command_ptr = (CommandStruct *) data;

	if (Command_Verificate(command_ptr) == FALSE)
	{
		CORE_DebugError("data is not a CommandStruct\n");
		return;
	}

	if (CommandsListener_Process(game_server->commands_listener, command_ptr) == FALSE)
	{
		CORE_DebugError("Command processing error\n");
		return;
	}
}

/*****************************************************************************************************************************/

CORE_Bool GameServer_InitNewSession(GameServer instance, uint32 *out_session_index)
{
	LabSession 	new_session;
	CORE_Bool 	found_free_session;
	uint32  	new_session_index;


	found_free_session = FALSE;

	for (uint32 i = 0; i < SESSIONS_SIZE_CAPACITY; i++)
	{
		if (instance->sessions[i] != NULL)
		{
			continue;
		}

		found_free_session = TRUE;
		new_session_index = i;
		break;
	}

	if (found_free_session == FALSE)
	{
		CORE_DebugError("No free session.\n");
		return FALSE;
	}

	LabSession_Create(&new_session);
	LabSession_Setup(new_session, SESSION_PLAYERS_COUNT);

	instance->sessions[new_session_index] = new_session;
	*out_session_index = new_session_index;
	return TRUE;
}

CORE_Bool GameServer_AddPlayerToSession(GameServer instance, uint32 session_index, char *player_name, uint32 *out_player_index)
{
	CORE_AssertPointer(player_name);
	CORE_AssertPointer(out_player_index);

	LabSession session;


	if (LabSession_HelperFindSession(instance->sessions, SESSIONS_SIZE_CAPACITY, session_index, &session) == FALSE)
	{
		CORE_DebugError("Can't add player\n");
		return FALSE;
	}

	return LabSession_AddPlayer(session, player_name, out_player_index);
}

CORE_Bool GameServer_StartSession(GameServer instance, uint32 session_index)
{
	LabSession session;


	if (LabSession_HelperFindSession(instance->sessions, SESSIONS_SIZE_CAPACITY, session_index, &session) == FALSE)
	{
		CORE_DebugError("Can't start session\n");
		return FALSE;
	}

	LabSession_Start(session);
	return TRUE;
}

void GameServer_Process(GameServer instance)
{
	// all processing in INTERNAL_OnIOSystemRead
}

void GameServer_Setup(GameServer instance)
{
	CommandsListener_Setup(instance->commands_listener, instance->sessions, SESSIONS_SIZE_CAPACITY);

	InOutSystem_Setup(instance->in_out_system, instance);
	InOutSystem_OnRead(instance->in_out_system, INTERNAL_OnIOSystemRead);
}

/*****************************************************************************************************************************/

void GameServer_Create(GameServer *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, GameServer);

	CORE_DebugPrint("GameServer: Init sessions\n");
	INTERNAL_PrepareSessions(*instance_ptr);

	CORE_DebugPrint("GameServer: Init commands listener\n");
	CommandsListener_Create(&(*instance_ptr)->commands_listener);

	CORE_DebugPrint("GameServer: Init in-out system\n");
	InOutSystem_Create(&(*instance_ptr)->in_out_system);
}

void GameServer_Free(GameServer *instance_ptr)
{
	INTERNAL_FreeSessions(*instance_ptr);
	CommandsListener_Free(&(*instance_ptr)->commands_listener);
	InOutSystem_Free(&(*instance_ptr)->in_out_system);

	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
