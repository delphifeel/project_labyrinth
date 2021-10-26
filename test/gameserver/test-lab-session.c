#include "CORE.h"
#include "gameserver/lab-points-map.h"
#include "gameserver/lab-session.h"

static void Test_LabSessionAddPlayers()
{
	LabSession 			instance;
	uint32 				player1_index;
	uint32 				player2_index;	
	Player 				player1;
	Player 				player2;
	uint32 				temp_id;
	uint32 				player1_id;
	uint32 				player2_id;
	uint8 				player1_token[TOKEN_SIZE];
	uint8 				player2_token[TOKEN_SIZE];
	uint32  			player1_token_value;
	uint32  			player2_token_value;
	const uint8 		*temp_token_ptr;


	player1_id = 666;
	player2_id = 669;

	player1_token_value = 0xBADBEE;
	player2_token_value = 0xDEADBEE;
	memcpy(player1_token, &player1_token_value, sizeof(player1_token_value));
	memcpy(player2_token, &player2_token_value, sizeof(player2_token_value));

	LabSession_Create(&instance);
	LabSession_Setup(instance, 2);

	LabSession_AddPlayer(instance, player1_id, player1_token, &player1_index);
	LabSession_AddPlayer(instance, player2_id, player2_token, &player2_index);

	assert(player1_index == 0);
	assert(player2_index == 1);

	LabSession_FindPlayer(instance, player1_index, &player1);
	LabSession_FindPlayer(instance, player2_index, &player2);

	Player_GetId(player1, &temp_id);
	assert(temp_id == player1_id);

	Player_GetId(player2, &temp_id);
	assert(temp_id == player2_id);

	Player_GetTokenPtr(player1, &temp_token_ptr);
	assert(memcmp(temp_token_ptr, player1_token, TOKEN_SIZE) == 0);

	Player_GetTokenPtr(player2, &temp_token_ptr);
	assert(memcmp(temp_token_ptr, player2_token, TOKEN_SIZE) == 0);

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
