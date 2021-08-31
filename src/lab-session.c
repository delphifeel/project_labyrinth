#include "../include/CORE.h"
#include "../include/labyrinth/lab-point.h"

/*****************************************************************************************************************************/

void LabSession_GetLabPointById(LabSession Instance, uint32 Id, LabPointStruct *OUT_LabPoint)
{
	LabPointsMap_GetPointByID(Instance->LabyrinthMap, Id, OUT_LabPoint);
}

void LabSession_AddPlayer(LabSession Instance, char *PlayerName, uint32 *OUT_AddedPlayerId)
{
	Player NewPlayer;
	uint32 PlayerSpawnPointId;


	if (Instance->PlayersMapSize == Instance->PlayersMapCapacity)
	{
		CODE_DebugError("PlayersMapSize == PlayersMapCapacity\n");
		return;
	}

	*OUT_AddedPlayerId = Instance->PlayersMapSize + 1;

	Player_Create(&NewPlayer);
	Player_Setup(NewPlayer, Instance);
	Player_SetId(NewPlayer, *OUT_AddedPlayerId);
	Player_SetName(NewPlayer, PlayerName);
	// TODO: Player_SetSpawnLabPointId(NewPlayer, PlayerSpawnPointId)

	Instance->PlayersMap[Instance->PlayersMapSize] = NewPlayer;
	Instance->PlayersMapSize++;
}

/*****************************************************************************************************************************/

void LabSession_Setup(LabSession Instance, uint32 PlayersCount);
{
	if (CORE_CreateUID(Instance->SessionUID) == FALSE)
	{
		CORE_DebugAbort("Can't create UID\n");
		return;
	}

	Instance->PlayersMap = CORE_MemAlloc(sizeof(Player) * PlayersCount);
	Instance->PlayersMapSize = 0;
	Instance->PlayersMapCapacity = PlayersCount;
}

/*****************************************************************************************************************************/

void LabSession_Create(LabSession* InstancePtr)
{
	CORE_OBJECT_CREATE(InstancePtr, LabSession);
}

void LabSession_Free(LabSession* InstancePtr)
{
	for (uint32 i = 0; i < (*InstancePtr->PlayersMapSize); i++)
	{
		CORE_MemFree((*InstancePtr)->PlayersMap[i]);
	}

	CORE_MemFree((*InstancePtr)->PlayersMap);
	CORE_OBJECT_FREE(InstancePtr);
}

/*****************************************************************************************************************************/
