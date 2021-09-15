#include "../include/CORE.h"
#include "../include/labyrinth/lab-points-map.h"
#include "../include/labyrinth/lab-session.h"

void Test_LabSessionAddPlayers()
{
	LabSession Instance;
	char *Player1_Name = "Player1";	
	char *Player2_Name = "Player2";
	uint32 Player1_Index, Player2_Index;	
	Player Player1, Player2;
	char TempName[40];
	uint32 TempId;


	LabSession_Create(&Instance);
	LabSession_Setup(Instance, 2);

	LabSession_AddPlayer(Instance, Player1_Name, &Player1_Index);
	LabSession_AddPlayer(Instance, Player2_Name, &Player2_Index);

	assert(Player1_Index == 0);
	assert(Player2_Index == 1);

	LabSession_FindPlayer(Instance, Player1_Index, &Player1);
	LabSession_FindPlayer(Instance, Player2_Index, &Player2);

	Player_GetName(Player1, TempName, sizeof(TempName));
	assert(strcmp(TempName, Player1_Name) == 0);

	Player_GetId(Player1, &TempId);
	assert(TempId == Player1_Index);

	Player_GetName(Player2, TempName, sizeof(TempName));
	assert(strcmp(TempName, Player2_Name) == 0);

	Player_GetId(Player2, &TempId);
	assert(TempId == Player2_Index);

	LabSession_Free(&Instance);
}

// void Test_LabSessionLabyrinthToJSON()
// {
// 	LabSession Instance;
// 	char *JSON;
// 	CORE_FileHandle FileToWrite;

// 	LabSession_Create(&Instance);
// 	LabSession_Setup(Instance, 5);

// 	LabPointsMap_ToJSON(Instance->LabyrinthMap, &JSON);

// 	FileToWrite = CORE_FileOpen("data-prim.json", "w");
// 	CORE_FileWrite(JSON, sizeof(char), strlen(JSON), FileToWrite);
// 	CORE_FileClose(FileToWrite);

// 	LabSession_Free(&Instance);
// 	CORE_MemFree(JSON);
// }

int main()
{
	Test_LabSessionAddPlayers();
	// Test_LabSessionLabyrinthToJSON();
}