#include "CCORE.h"
#include "modules/labyrinth/lab-session.h"

static void Test_LabSessionAddPlayers(void)
{
    LabSession       *instance;
    uint              player1_id;
    uint              player2_id;  
    Player           *player1;
    Player           *player2;
    uint8             player1_token[PLAYER_TOKEN_SIZE];
    uint8             player2_token[PLAYER_TOKEN_SIZE];
    uint              player1_token_value;
    uint              player2_token_value;
    const uint8      *temp_token_ptr;


    player1_token_value = 0xBADBEE;
    player2_token_value = 0xDEADBEE;
    CORE_MemZero(player1_token, sizeof(player1_token));
    CORE_MemZero(player2_token, sizeof(player2_token));
    CORE_MemCpy(player1_token, &player1_token_value, sizeof(player1_token_value));
    CORE_MemCpy(player2_token, &player2_token_value, sizeof(player2_token_value));

    instance = LabSession_Create();
    LabSession_Setup(instance, 2);

    LabSession_AddPlayer(instance, player1_token, &player1_id);
    LabSession_AddPlayer(instance, player2_token, &player2_id);

    CORE_Assert(player1_id == 1);
    CORE_Assert(player2_id == 2);

    player1 = LabSession_FindPlayer(instance, player1_id);
    player2 = LabSession_FindPlayer(instance, player2_id);

    CORE_Assert(Player_GetId(player1) == player1_id);
    CORE_Assert(Player_GetId(player2) == player2_id);

    Player_GetToken(player1, &temp_token_ptr);
    CORE_Assert(CORE_MemCmp(temp_token_ptr, player1_token, PLAYER_TOKEN_SIZE));

    Player_GetToken(player2, &temp_token_ptr);
    CORE_Assert(CORE_MemCmp(temp_token_ptr, player2_token, PLAYER_TOKEN_SIZE));

    LabSession_Free(instance);
}

// void Test_LabSessionLabyrinthToJSON(void)
// {
//     LabSession instance;
//     char *json;
//     CFile FileToWrite;

//     LabSession_Create(&instance);
//     LabSession_Setup(instance, 5);

//     LabSession_MapToJSON(instance, &json);

//     FileToWrite = CFile_Open("data-prim.json", "w");
//     CFile_Write(json, sizeof(char), strlen(json), FileToWrite);
//     CFile_Close(FileToWrite);

//     LabSession_Free(&instance);
//     CORE_MemFree(json);
// }

void LabSession_TestsRun(void)
{
    Test_LabSessionAddPlayers();
    // Test_LabSessionLabyrinthToJSON();
}