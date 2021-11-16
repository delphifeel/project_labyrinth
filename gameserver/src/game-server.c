#include "gameserver/game-server.h"
#include "gameserver/lab-session.h"
#include "gameserver/commands-io-system.h"
#include "gameserver/CONFIG.h"

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
	CommandsIOSystem_Free(&(*instance_ptr)->commands_io_system);
	_FreeSessions(*instance_ptr);

	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
