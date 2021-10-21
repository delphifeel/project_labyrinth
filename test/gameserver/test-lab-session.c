#include "CORE.h"
#include "gameserver/lab-points-map.h"
#include "gameserver/lab-session.h"

static void Test_LabSessionAddPlayers()
{
	LabSession instance;
	uint32 player1_index, player2_index;	
	Player player1, player2;
	uint32 temp_id;
	uint32 player1_id;
	uint32 player2_id;


	player1_id = 666;
	player2_id = 669;

	LabSession_Create(&instance);
	LabSession_Setup(instance, 2);

	LabSession_AddPlayer(instance, player1_id, &player1_index);
	LabSession_AddPlayer(instance, player2_id, &player2_index);

	assert(player1_index == 0);
	assert(player2_index == 1);

	LabSession_FindPlayer(instance, player1_index, &player1);
	LabSession_FindPlayer(instance, player2_index, &player2);

	Player_GetId(player1, &temp_id);
	assert(temp_id == player1_id);

	Player_GetId(player2, &temp_id);
	assert(temp_id == player2_id);

	LabSession_Free(&instance);
}

// void Test_LabSessionLabyrinthToJSON()
// {
// 	LabSession instance;
// 	char *json;
// 	CORE_FileHandle FileToWrite;

// 	LabSession_Create(&instance);
// 	LabSession_Setup(instance, 5);

// 	LabPointsMap_ToJSON(instance->labyrinth_map, &json);

// 	FileToWrite = CORE_FileOpen("data-prim.json", "w");
// 	CORE_FileWrite(json, sizeof(char), strlen(json), FileToWrite);
// 	CORE_FileClose(FileToWrite);

// 	LabSession_Free(&instance);
// 	CORE_MemFree(json);
// }

void Test_LabSession()
{
	Test_LabSessionAddPlayers();
	// Test_LabSessionLabyrinthToJSON();
}
