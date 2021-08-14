#include <string.h> 

#include "../include/CORE.h"
#include "../include/player.h"

#define MAX_DIRECTON_SIZE 2
#define SPEED 1; 



CORE_Bool Player_Move(Player Instance, MoveDirection *Directions, uint32 DirectionsSize)
{
    uint32 speed; 

    if(DirectionsSize > MAX_DIRECTON_SIZE) 
    {
        CORE_DebugError("Player_Move: Directions applies %d params got %ld", MAX_DIRECTON_SIZE, DirectionsSize); 
        return FALSE; 
    }

    speed = Instance->SpeedMultiplier * SPEED; 

    for (uint32 i = 0; i < DirectionsSize; i++)
    {
        switch(Directions[i])
        {
            case kMoveDirection_Top: 
                Instance->position->positionY += speed; 
                break; 
            case kMoveDirection_Right:
                Instance->position->positionX += speed;
                break;
            case kMoveDirection_Bottom:  
                Instance->position->positionY -= speed;
                break; 
            case kMoveDirection_Left:
                Instance->position->positionX -= speed; 
                break; 
        }
    }

    return TRUE; 
}


void Player_SetId(Player Instance, uint32 ID)
{
    Instance->Id = ID; 
}


void Player_GetId(Player Instance, uint32 *ID)
{
    *ID = Instance->Id; 
}


void Player_SetName(Player Instance, char *Name)
{
    strncpy(Instance->Name, Name, strlen(Name)); 
}


void Player_GetName(Player Instance, char *Name)
{
    strncpy(Name, Instance->Name, strlen(Name)); 
}


void Player_Create(Player *InstancePtr, Position position)
{
    CORE_OBJECT_CREATE(InstancePtr, Player);
    (*InstancePtr)->position = position; 
}


void Player_Free(Player *InstancePtr) 
{
    CORE_OBJECT_FREE(InstancePtr); 
}