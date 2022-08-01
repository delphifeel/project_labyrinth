#include "CCORE.h"
#include "modules/labyrinth/lab-session.h"

static void Test_LabSessionAddPlayers(void)
{
    LabSession       *instance;
    uint              player1_id;
    uint              player2_id;  
    Player           *player1;
    Player           *player2;

    instance = LabSession_Create();
    LabSession_Setup(instance, 2);

    LabSession_AddPlayer(instance, &player1_id);
    LabSession_AddPlayer(instance, &player2_id);

    CORE_Assert(player1_id == 1);
    CORE_Assert(player2_id == 2);

    player1 = LabSession_FindPlayer(instance, player1_id);
    player2 = LabSession_FindPlayer(instance, player2_id);

    CORE_Assert(Player_GetId(player1) == player1_id);
    CORE_Assert(Player_GetId(player2) == player2_id);

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