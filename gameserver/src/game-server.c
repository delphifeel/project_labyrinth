#include "game-server/game-server.h"
#include "game-server/lab-session.h"
#include "game-server/commands-listener.h"
#include "game-server/in-out-system.h"

/*****************************************************************************************************************************/

#define SESSIONS_SIZE_CAPACITY 	(40)

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(GameServer,
	LabSession 				sessions[SESSIONS_SIZE_CAPACITY];
	CommandsListener 		commands_listener;
	InOutSystem 			in_out_system;
);

/*****************************************************************************************************************************/

static void GameServer_InitSessions(GameServer instance)
{
	LabSession created_lab_session;


	for (uint32 i = 0; i < SESSIONS_SIZE_CAPACITY; i++)
	{
		LabSession_Create(&created_lab_session);
		// TODO: LabSession_Setup ? 
		instance->sessions[i] = created_lab_session;
	}
}

static void GameServer_FreeSessions(GameServer instance)
{
	for (uint32 i = 0; i < SESSIONS_SIZE_CAPACITY; i++)
	{
		LabSession_Free(&instance->sessions[i]);
	}
}

// TODO(delphifeel): Add processing
// void GameServer_Process(GameServer instance)
// {
// 	CORE_Bool command_process_result;


// 	command_process_result = CommandsListener_Process(instance->commands_listener);
// 	if (command_process_result == FALSE)
// 	{
// 		CORE_DebugError("Command processing failed\n");
// 		return;
// 	}
// }

void GameServer_Setup(GameServer instance)
{
	CommandsListener_Setup(instance->commands_listener, instance->sessions, SESSIONS_SIZE_CAPACITY);
}

/*****************************************************************************************************************************/

void GameServer_Create(GameServer *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, GameServer);

	CORE_DebugPrint("GameServer: Init sessions\n");
	GameServer_InitSessions(*instance_ptr);

	CORE_DebugPrint("GameServer: Init commands listener\n");
	CommandsListener_Create(&(*instance_ptr)->commands_listener);

	CORE_DebugPrint("GameServer: Init in-out system\n");
	InOutSystem_Create(&(*instance_ptr)->in_out_system);
}

void GameServer_Free(GameServer *instance_ptr)
{
	CORE_OBJECT_FREE(instance_ptr);

	GameServer_FreeSessions(*instance_ptr);
	CommandsListener_Free(&(*instance_ptr)->commands_listener);
	InOutSystem_Free(&(*instance_ptr)->in_out_system);
}

/*****************************************************************************************************************************/
