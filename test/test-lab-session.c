#include "../include/CORE.h"
#include "../include/labyrinth/lab-points-map.h"
#include "../include/labyrinth/lab-session.h"

CORE_OBJECT_INTERFACE(LabSession,
	uint8  			SessionUID[UID_SIZE];

	/* hash map structure containing graph of LabPoint's */
	LabPointsMap 	LabyrinthMap;

	/* players that currently in this session */
	Player 			*PlayersMap;
	uint32  		PlayersMapSize;
	uint32 			PlayersMapCapacity;

	/* etc */
);

void Test_LabSessionAddPlayers()
{
	LabSession Instance;
	char *Player1_Name = "Player1";	
	char *Player2_Name = "Player2";
	uint32 Player1_Id, Player2_Id;	
	Player Player1, Player2;
	char TempName[40];
	uint32 TempId;


	LabSession_Create(&Instance);
	LabSession_Setup(Instance, 2);

	LabSession_AddPlayer(Instance, Player1_Name, &Player1_Id);
	LabSession_AddPlayer(Instance, Player2_Name, &Player2_Id);

	assert(Player1_Id == 1);
	assert(Player2_Id == 2);

	assert(Instance->PlayersMapSize == 2);
	assert(Instance->PlayersMapCapacity == 2);

	Player1 = Instance->PlayersMap[ID_HASH(Player1_Id)];
	Player2 = Instance->PlayersMap[ID_HASH(Player2_Id)];

	Player_GetName(Player1, TempName, sizeof(TempName));
	assert(strcmp(TempName, Player1_Name) == 0);

	Player_GetId(Player1, &TempId);
	assert(TempId == Player1_Id);

	Player_GetName(Player2, TempName, sizeof(TempName));
	assert(strcmp(TempName, Player2_Name) == 0);

	Player_GetId(Player2, &TempId);
	assert(TempId == Player2_Id);

	LabSession_Free(&Instance);
}

void Test_LabSessionLabyrinthToJSON()
{
	LabSession Instance;
	char *JSON;
	CORE_FileHandle FileToWrite;

	LabSession_Create(&Instance);
	LabSession_Setup(Instance, 5);

	LabPointsMap_ToJSON(Instance->LabyrinthMap, &JSON);

	FileToWrite = CORE_FileOpen("data-prim.json", "w");
	CORE_FileWrite(JSON, sizeof(char), strlen(JSON), FileToWrite);
	CORE_FileClose(FileToWrite);

	LabSession_Free(&Instance);
	CORE_MemFree(JSON);
}

int main()
{
	Test_LabSessionAddPlayers();
	// Test_LabSessionLabyrinthToJSON();
}