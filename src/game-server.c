#include "../include/game-server.h"
#include "../include/labyrinth/lab-session.h"
#include "../include/commands-listener.h"
#include "../include/in-out-system.h"

/*****************************************************************************************************************************/

#define SESSIONS_SIZE_CAPACITY 	(40)

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(GameServer,
	LabSession 				Sessions[SESSIONS_SIZE_CAPACITY];
	CommandsListener 		CommandsListenerObject;
	InOutSystem 			InOutSystemObject;
);

/*****************************************************************************************************************************/

static void GameServer_InitSessions(GameServer Instance)
{
	LabSession CreatedLabSession;


	for (uint32 i = 0; i < SESSIONS_SIZE_CAPACITY; i++)
	{
		LabSession_Create(&CreatedLabSession);
		// TODO: LabSession_Setup ? 
		Instance->Sessions[i] = CreatedLabSession;
	}
}

static void GameServer_FreeSessions(GameServer Instance)
{
	for (uint32 i = 0; i < SESSIONS_SIZE_CAPACITY; i++)
	{
		LabSession_Free(&Instance->Sessions[i]);
	}
}

void GameServer_Process(GameServer Instance)
{
	CORE_Bool command_process_result;


	command_process_result = CommandsListener_Process(Instance->CommandsListenerObject);
	if (command_process_result == FALSE)
	{
		CORE_DebugError("Command processing failed\n");
		return;
	}
}

void GameServer_Setup(GameServer Instance)
{
	CommandsListener_Setup(Instance->CommandsListenerObject, Instance->Sessions, SESSIONS_SIZE);
}

/*****************************************************************************************************************************/

void GameServer_Create(GameServer *InstancePtr)
{
	CORE_OBJECT_CREATE(InstancePtr, GameServer);

	CORE_DebugPrint("GameServer: Init sessions\n");
	GameServer_InitSessions(*InstancePtr);

	CORE_DebugPrint("GameServer: Init commands listener\n");
	CommandsListener_Create(&(*InstancePtr)->CommandsListenerObject);

	CORE_DebugPrint("GameServer: Init in-out system\n");
	InOutSystem_Create(&(*InstancePtr)->InOutSystemObject);
}

void GameServer_Free(GameServer *InstancePtr)
{
	CORE_OBJECT_FREE(InstancePtr);

	GameServer_FreeSessions(*InstancePtr);
	CommandsListener_Free(&(*InstancePtr)->CommandsListenerObject);
	InOutSystem_Free(&(*InstancePtr)->InOutSystemObject);
}

/*****************************************************************************************************************************/
