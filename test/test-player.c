#include "../include/CORE.h"
#include "../include/player.h"


static const PositionStruct Mock_Position   = {.X = 42, .Y = 71};
static const uint32         Mock_Id         = 999;

void Test_PlayerInit()
{
    Player          Instance; 
    PositionStruct  PlayerPosition;

    Player_Create(&Instance); 
    Player_Setup(Instance, Mock_Position);

    Player_GetPosition(Instance, &PlayerPosition);
    assert(PlayerPosition.X == Mock_Position.X); 
    assert(PlayerPosition.Y == Mock_Position.Y); 

    Player_Free(&Instance); 
}


void Test_PlayerSetGetName()
{
    Player Instance; 
    char Name[40] = "Labyrinth"; 
    char PlayerName[40];

    Player_Create(&Instance);
    Player_Setup(Instance, Mock_Position);

    Player_SetName(Instance, Name); 
    Player_GetName(Instance, PlayerName, sizeof(PlayerName));
    assert(strcmp(PlayerName, Name) == 0);

    strncpy(PlayerName, "Lorem ipsum dolor", sizeof(PlayerName));   
    Player_GetName(Instance, PlayerName, sizeof(PlayerName));
    assert(strcmp(PlayerName, Name) == 0);

    Player_Free(&Instance); 
}


void Test_PlayerSetGetId()
{
    Player Instance; 
    uint32 Id; 

    Player_Create(&Instance);
    Player_Setup(Instance, Mock_Position);

    Player_SetId(Instance, Mock_Id); 
    Player_GetId(Instance, &Id);
    assert(Id == Mock_Id); 

    Player_Free(&Instance); 
}


void Test_PlayerMove()
{
    Player Instance; 
    uint32 Id; 

    MoveDirection Directions[3] = {kMoveDirection_Top, kMoveDirection_Bottom, kMoveDirection_Left};  

    Player_Create(&Instance);
    Player_Setup(Instance, Mock_Position);
    // exceed required count of params

    // TODO: implement Test_PlayerMove
}


int main()
{
    Test_PlayerInit(); 
    Test_PlayerSetGetName();
    Test_PlayerSetGetId(); 
    Test_PlayerMove();
}