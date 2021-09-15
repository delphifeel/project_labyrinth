#include "../include/CORE.h"
#include "../include/player.h"


static const PositionStruct Mock_Position   = {.X = 42, .Y = 71};
static const uint32         Mock_Id         = 999;

void Test_PlayerInit()
{
    Player          instance; 
    PositionStruct  PlayerPosition;

    Player_Create(&instance); 
    Player_Setup(instance, Mock_Position);

    Player_GetPosition(instance, &PlayerPosition);
    assert(PlayerPosition.X == Mock_Position.X); 
    assert(PlayerPosition.Y == Mock_Position.Y); 

    Player_Free(&instance); 
}


void Test_PlayerSetGetName()
{
    Player instance; 
    char Name[40] = "Labyrinth"; 
    char player_name[40];

    Player_Create(&instance);
    Player_Setup(instance, Mock_Position);

    Player_SetName(instance, Name); 
    Player_GetName(instance, player_name, sizeof(player_name));
    assert(strcmp(player_name, Name) == 0);

    strncpy(player_name, "Lorem ipsum dolor", sizeof(player_name));   
    Player_GetName(instance, player_name, sizeof(player_name));
    assert(strcmp(player_name, Name) == 0);

    Player_Free(&instance); 
}


void Test_PlayerSetGetId()
{
    Player instance; 
    uint32 Id; 

    Player_Create(&instance);
    Player_Setup(instance, Mock_Position);

    Player_SetId(instance, Mock_Id); 
    Player_GetId(instance, &Id);
    assert(Id == Mock_Id); 

    Player_Free(&instance); 
}


void Test_PlayerMove()
{
    Player instance; 
    uint32 Id; 

    MoveDirection Directions[3] = {kMoveDirection_Top, kMoveDirection_Bottom, kMoveDirection_Left};  

    Player_Create(&instance);
    Player_Setup(instance, Mock_Position);
    // exceed required count of params

    // TODO: implement Test_PlayerMove
}


int main()
{
    //TODO: FIX TEST
    Test_PlayerInit(); 
    Test_PlayerSetGetName();
    Test_PlayerSetGetId(); 
    Test_PlayerMove();
}