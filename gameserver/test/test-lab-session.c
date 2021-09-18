#include "CORE.h"
#include "game-server/lab-points-map.h"
#include "game-server/lab-session.h"

void Test_LabSessionAddPlayers()
{
	LabSession instance;
	char *player1_name = "player1";	
	char *player2_name = "player2";
	uint32 player1_index, player2_index;	
	Player player1, player2;
	char temp_name[40];
	uint32 temp_id;


	LabSession_Create(&instance);
	LabSession_Setup(instance, 2);

	LabSession_AddPlayer(instance, player1_name, &player1_index);
	LabSession_AddPlayer(instance, player2_name, &player2_index);

	assert(player1_index == 0);
	assert(player2_index == 1);

	LabSession_FindPlayer(instance, player1_index, &player1);
	LabSession_FindPlayer(instance, player2_index, &player2);

	Player_GetName(player1, temp_name, sizeof(temp_name));
	assert(strcmp(temp_name, player1_name) == 0);

	Player_GetId(player1, &temp_id);
	assert(temp_id - 1 == player1_index);

	Player_GetName(player2, temp_name, sizeof(temp_name));
	assert(strcmp(temp_name, player2_name) == 0);

	Player_GetId(player2, &temp_id);
	assert(temp_id - 1 == player2_index);

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

int main()
{
	Test_LabSessionAddPlayers();
	// Test_LabSessionLabyrinthToJSON();
}
