#include "../include/CORE.h"
#include "../include/labyrinth/lab-generation.h"
#include "../include/labyrinth/lab-session.h"


CORE_OBJECT_INTERFACE(LabSession,
	uint8  				SessionUID[UID_SIZE];

	/* hash map structure containing graph of LabPoint's */
	LabPointsMap 		LabyrinthMap;
	LabPointsMapReader 	LabyrinthMapReader;

	/* players that currently in this session */
	Player 				*PlayersMap;
	uint32  			PlayersMapSize;
	uint32 				PlayersMapCapacity;

	uint32  			*SpawnPoints;
	uint32  			SpawnPointsSize;
	uint32  			SpawnPointsAssocWithPlayerCount;

	/* etc */
);

/*****************************************************************************************************************************/

CORE_Bool LabSession_FindPlayer(LabSession Instance, uint32 PlayerIndex, Player *OUT_Player)
{
	if (PlayerIndex + 1 > Instance->PlayersMapSize)
	{
		CORE_DebugError("Player index out of bounds\n");
		return FALSE;
	}

	*OUT_Player = Instance->PlayersMap[PlayerIndex];
	return TRUE;
}

void LabSession_AddPlayer(LabSession Instance, char *PlayerName, uint32 *OUT_AddedPlayerId)
{
	Player NewPlayer;
	uint32 PlayerSpawnPointId;


	if (Instance->PlayersMapSize == Instance->PlayersMapCapacity)
	{
		CORE_DebugError("PlayersMapSize == PlayersMapCapacity\n");
		return;
	}

	*OUT_AddedPlayerId = Instance->PlayersMapSize;

	if (Instance->SpawnPointsAssocWithPlayerCount == Instance->SpawnPointsSize)
	{
		CORE_DebugError("No more spawn points available\n");
		return;
	}

	PlayerSpawnPointId = Instance->SpawnPoints[Instance->SpawnPointsAssocWithPlayerCount++];

	Player_Create(&NewPlayer);
	Player_Setup(NewPlayer, Instance->LabyrinthMapReader, PlayerSpawnPointId);
	Player_SetId(NewPlayer, *OUT_AddedPlayerId);
	Player_SetName(NewPlayer, PlayerName);

	Instance->PlayersMap[Instance->PlayersMapSize] = NewPlayer;
	Instance->PlayersMapSize++;
}

void LabSession_GetLabPointsReader(LabSession Instance, LabPointsMapReader *OUT_LabPointsReader)
{
	*OUT_LabPointsReader = Instance->LabyrinthMapReader;
}

/*****************************************************************************************************************************/

void LabSession_Setup(LabSession Instance, uint32 PlayersCount)
{
	if (CORE_CreateUID(Instance->SessionUID) == FALSE)
	{
		CORE_DebugAbort("Can't create UID\n");
		return;
	}

	Instance->PlayersMap = CORE_MemAlloc(sizeof(Player) * PlayersCount);
	Instance->PlayersMapSize = 0;
	Instance->PlayersMapCapacity = PlayersCount;

	LabPointsMap_Create(&Instance->LabyrinthMap);
	LabPointsMapReader_Create(&Instance->LabyrinthMapReader);
	LabPointsMapReader_Setup(Instance->LabyrinthMapReader, Instance->LabyrinthMap);

	Instance->SpawnPointsAssocWithPlayerCount = 0;
	LabGeneration_Execute(Instance->LabyrinthMap, &Instance->SpawnPoints, &Instance->SpawnPointsSize);
}

/*****************************************************************************************************************************/

void LabSession_Create(LabSession* InstancePtr)
{
	CORE_OBJECT_CREATE(InstancePtr, LabSession);
}

void LabSession_Free(LabSession* InstancePtr)
{
	CORE_MemFree((*InstancePtr)->SpawnPoints);

	LabPointsMapReader_Free(&(*InstancePtr)->LabyrinthMapReader);
	LabPointsMap_Free(&(*InstancePtr)->LabyrinthMap);

	for (uint32 i = 0; i < (*InstancePtr)->PlayersMapSize; i++)
	{
		CORE_MemFree((*InstancePtr)->PlayersMap[i]);
	}

	CORE_MemFree((*InstancePtr)->PlayersMap);
	CORE_OBJECT_FREE(InstancePtr);
}

/*****************************************************************************************************************************/
