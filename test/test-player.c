#include "../include/CORE.h"
#include "../include/player.h"


void Test_PlayerInit()
{
    Player player; 
    Position position; 

    position = CORE_MemAlloc(sizeof(Position)); 

    (*position).positionX = 42; 
    (*position).positionY = 71; 

    Player_Create(&player, position); 

    assert((*player).position->positionX == 42); 
    assert((*player).position->positionY == 71); 

    Player_Free(&player); 
    CORE_MemFree(position); 
}


void Test_PlayerSetGetName()
{
    Player player; 
    Position position; 
    char name[40] = "Labyrinth"; 

    position = CORE_MemAlloc(sizeof(Position)); 

    position->positionX = 42; 
    position->positionY = 71; 

    Player_Create(&player, position); 
    Player_SetName(player, name); 

    assert(strcmp((*player).Name, name) == 0);

    strcpy(name, "Lorem ipsum dolor");   
    Player_GetName(player, name); 

    assert(strcmp((*player).Name, name) == 0); 

    Player_Free(&player); 
    CORE_MemFree(position);
}


void Test_PlayerSetGetId()
{
    Player player; 
    Position position; 
    uint32 ID; 

    position = CORE_MemAlloc(sizeof(Position)); 

    (*position).positionX = 42; 
    (*position).positionY = 71; 
    Player_Create(&player, position);

    Player_SetId(player, 42); 
    assert((*player).Id == 42); 

    Player_GetId(player, &ID); 

    assert(ID == 42); 

    Player_Free(&player); 
    CORE_MemFree(position)
}


void Test_PlayerMove()
{
    Player player; 
    Position position; 
    uint32 ID; 

    MoveDirection Directions[3] = {kMoveDirection_Top, kMoveDirection_Bottom, kMoveDirection_Left};  

    position = CORE_MemAlloc(sizeof(Position)); 

    (*position).positionX = 42; 
    (*position).positionY = 71; 
    Player_Create(&player, position);
    CORE_MemFree(position)

    // exceed required count of params


}


int main()
{
    Test_PlayerInit(); 
    Test_PlayerSetGetName();
    Test_PlayerSetGetId(); 
    Test_PlayerMove();
}