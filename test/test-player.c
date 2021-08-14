#include "../include/CORE.h"
#include "../include/player.h"


void Test_PlayerInit()
{
    Player player; 
    Position position; 

    CORE_OBJECT_CREATE(&position, Position); 

    (*position).positionX = 42; 
    (*position).positionY = 71; 

    Player_Create(&player, position); 

    assert((*player).position->positionX == 42); 
    assert((*player).position->positionY == 71); 

    Player_Free(&player); 
}


void Test_PlayerSetGetName()
{
    Player player; 
    Position position; 
    char name[40] = "Labyrinth"; 

    CORE_OBJECT_CREATE(&position, Position); 

    (*position).positionX = 42; 
    (*position).positionY = 71; 

    Player_Create(&player, position); 
    Player_SetName(player, name); 

    assert(strcmp((*player).Name, name) == 0);

    strcpy(name, "Lorem ipsum dolor");   
    Player_GetName(player, name); 

    assert(strcmp((*player).Name, name) == 0); 

    Player_Free(&player); 
}


void Test_PlayerSetGetId()
{
    Player player; 
    Position position; 
    uint32 ID; 

    CORE_OBJECT_CREATE(&position, Position); 

    (*position).positionX = 42; 
    (*position).positionY = 71; 
    Player_Create(&player, position);

    Player_SetId(player, 42); 
    assert((*player).Id == 42); 

    Player_GetId(player, &ID); 

    assert(ID == 42); 

    Player_Free(&player); 
}


int main()
{
    Test_PlayerInit(); 
    Test_PlayerSetGetName();
    Test_PlayerSetGetId(); 
}