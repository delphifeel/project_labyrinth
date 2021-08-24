#include <string.h> 

#include "../include/CORE.h"
#include "../include/player.h"

/*****************************************************************************************************************************/

#define MAX_DIRECTON_SIZE   (2)
#define SPEED               (1)

/*****************************************************************************************************************************/

CORE_Bool Player_Move(Player Instance, MoveDirection *Directions, uint32 DirectionsSize)
{
    uint32          Speed; 
    PositionStruct  *Position;


    if (DirectionsSize > MAX_DIRECTON_SIZE) 
    {
        CORE_DebugError("Player_Move: Directions applies %d params got %ld", MAX_DIRECTON_SIZE, DirectionsSize); 
        return FALSE; 
    }

    Speed = Instance->SpeedMultiplier * SPEED; 
    Position = &Instance->Position;

    for (uint32 i = 0; i < DirectionsSize; i++)
    {
        switch(Directions[i])
        {
            case kMoveDirection_Top: 
                Position->Y += Speed; 
                break; 
            case kMoveDirection_Right:
                Position->X += Speed;
                break;
            case kMoveDirection_Bottom:  
                Position->Y -= Speed;
                break; 
            case kMoveDirection_Left:
                Position->X -= Speed; 
                break; 
        }
    }

    return TRUE; 
}

void Player_SetId(Player Instance, uint32 Id)
{
    Instance->Id = Id; 
}

void Player_GetId(Player Instance, uint32 *Id)
{
    *Id = Instance->Id; 
}

void Player_SetName(Player Instance, char *Name)
{
    strncpy(Instance->Name, Name, sizeof(Instance->Name)); 
}

void Player_GetName(Player Instance, char *Name, uint32 NameSize)
{
    strncpy(Name, Instance->Name, NameSize); 
}

void Player_GetPosition(Player Instance, PositionStruct *Position)
{
    Position->X = Instance->Position.X; 
    Position->Y = Instance->Position.Y; 
}

void Player_Setup(Player Instance, PositionStruct Position)
{
    Instance->Position = Position;
}

/*****************************************************************************************************************************/

void Player_Create(Player *InstancePtr)
{
    CORE_OBJECT_CREATE(InstancePtr, Player);
}

void Player_Free(Player *InstancePtr) 
{
    CORE_OBJECT_FREE(InstancePtr); 
}