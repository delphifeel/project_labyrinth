#include <string.h> 

#include "../include/CORE.h"
#include "../include/player.h"

/*****************************************************************************************************************************/

#define MAX_DIRECTION_SIZE      (2)
#define SPEED                   (1)

/*****************************************************************************************************************************/

CORE_Bool Player_Move(Player Instance, MoveDirection *Directions, uint32 DirectionsSize)
{
    LabPointStruct      ResultLabPoint;
    uint32              ResultPointId;


    if (DirectionsSize > MAX_DIRECTION_SIZE) 
    {
        CORE_DebugError("Directions max size is 2\n"); 
        return FALSE; 
    }

    ResultPointId = Instance->PositionPointId;

    for (uint32 i = 0; i < DirectionsSize; i++)
    {
        LabSession_GetLabPointById(Instance->Session, ResultPointId, &ResultLabPoint);

        switch (Directions[i])
        {
            case kMoveDirection_Top: 
                ResultPointId = ResultLabPoint.TopConnectionId;
                break; 
            case kMoveDirection_Right:
                ResultPointId = ResultLabPoint.RightConnectionId;
                break;
            case kMoveDirection_Bottom:  
                ResultPointId = ResultLabPoint.BottomConnectionId;
                break; 
            case kMoveDirection_Left:
                ResultPointId = ResultLabPoint.LeftConnectionId;
                break; 
        }
    }

    Instance->PositionPointId = ResultPointId;

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

void Player_GetPositionInsideLabPoint(Player Instance, PositionStruct *Position)
{
    *Position = Instance->PositionInsideLabPoint;
}

/*****************************************************************************************************************************/

void Player_Setup(Player Instance, LabSession Session, uint32 SpawnPointId)
{
    Instance->PositionInsideLabPoint = {0, 0};
    Instance->SpeedMultiplier = 1;
    Instance->Session = Session;
    Instance->PositionPointId = SpawnPointId;
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