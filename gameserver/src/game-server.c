#include "gameserver/game-server.h"
#include "gameserver/lab-session.h"
#include "gameserver/commands-io-system.h"
#include "gameserver/game-server-config.h"

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(GameServer,
	LabSession 				sessions[SESSIONS_CAPACITY];
	CommandsIOSystem 		commands_io_system;
);

/*****************************************************************************************************************************/

static void _PrepareSessions(GameServer instance)
{
	CORE_MemZero(instance->sessions, sizeof(instance->sessions));
}

static void _FreeSessions(GameServer instance)
{
	for (uint32 i = 0; i < SESSIONS_CAPACITY; i++)
	{
		if (instance->sessions[i] != NULL)
		{
			LabSession_Free(&instance->sessions[i]);
		}
	}
}

/*****************************************************************************************************************************/

// CORE_Bool GameServer_InitNewSession(GameServer instance, uint32 *out_session_index)
// {
// 	LabSession 	new_session;
// 	CORE_Bool 	found_free_session;
// 	uint32  	new_session_index;


// 	found_free_session = FALSE;

// 	for (uint32 i = 0; i < SESSIONS_CAPACITY; i++)
// 	{
// 		if (instance->sessions[i] != NULL)
// 		{
// 			continue;
// 		}

// 		found_free_session = TRUE;
// 		new_session_index = i;
// 		break;
// 	}

// 	if (found_free_session == FALSE)
// 	{
// 		CORE_DebugError("No free session.\n");
// 		return FALSE;
// 	}

// 	LabSession_Create(&new_session);
// 	LabSession_Setup(new_session, SESSION_PLAYERS_COUNT);

// 	instance->sessions[new_session_index] = new_session;
// 	*out_session_index = new_session_index;
// 	return TRUE;
// }

// CORE_Bool GameServer_AddPlayerToSession(GameServer instance, uint32 session_index, uint32 player_id, uint32 *out_player_index)
// {
// 	CORE_AssertPointer(out_player_index);

// 	LabSession session;


// 	if (LabSession_HelperFindSession(instance->sessions, SESSIONS_CAPACITY, session_index, &session) == FALSE)
// 	{
// 		CORE_DebugError("Can't add player\n");
// 		return FALSE;
// 	}

// 	return LabSession_AddPlayer(session, player_id, out_player_index);
// }

// CORE_Bool GameServer_StartSession(GameServer instance, uint32 session_index)
// {
// 	LabSession session;


// 	if (LabSession_HelperFindSession(instance->sessions, SESSIONS_CAPACITY, session_index, &session) == FALSE)
// 	{
// 		CORE_DebugError("Can't start session\n");
// 		return FALSE;
// 	}

// 	LabSession_Start(session);
// 	return TRUE;
// }

void GameServer_Process(GameServer instance)
{
	// all processing in _OnIOSystemRead
}

void GameServer_Setup(GameServer instance)
{
	CommandsIOSystem_Setup(instance->commands_io_system, instance->sessions, SESSIONS_CAPACITY);
	CommandsIOSystem_Start(instance->commands_io_system);
}

/*****************************************************************************************************************************/

void GameServer_Create(GameServer *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, GameServer);

	CORE_DebugInfo("Init sessions\n");
	_PrepareSessions(*instance_ptr);

	CORE_DebugInfo("Init commands in-out system\n");
	CommandsIOSystem_Create(&(*instance_ptr)->commands_io_system);
}

void GameServer_Free(GameServer *instance_ptr)
{
	_FreeSessions(*instance_ptr);
	CommandsIOSystem_Free(&(*instance_ptr)->commands_io_system);

	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
