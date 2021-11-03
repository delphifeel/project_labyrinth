#include <string.h> 

#include "CORE.h"
#include "gameserver/player.h"

/*****************************************************************************************************************************/

#define MAX_DIRECTION_SIZE      (2)
#define SPEED                   (1)

CORE_OBJECT_INTERFACE(Player,
    uint32                      id;
    uint32                      speed_multiplier;
    uint32                      position_point_id;
    PositionStruct              position_inside_lab_point;
    LabPointsMapReader          points_reader;
    uint8                       token[TOKEN_SIZE];
);

/*****************************************************************************************************************************/

CORE_Bool Player_Move(Player instance, const MoveDirection *directions, uint32 directions_size)
{
    CORE_AssertPointer(directions);

    LabPointStruct      result_lab_point;
    uint32              result_point_id;


    if (directions_size > MAX_DIRECTION_SIZE) 
    {
        CORE_DebugError("directions max size is 2\n"); 
        return FALSE; 
    }

    result_point_id = instance->position_point_id;

    for (uint32 i = 0; i < directions_size; i++)
    {
        LabPointsMapReader_GetPointByID(instance->points_reader, result_point_id, &result_lab_point);

        switch (directions[i])
        {
            case kMoveDirection_Top: 
                result_point_id = result_lab_point.top_connection_id;
                break; 
            case kMoveDirection_Right:
                result_point_id = result_lab_point.right_connection_id;
                break;
            case kMoveDirection_Bottom:  
                result_point_id = result_lab_point.bottom_connection_id;
                break; 
            case kMoveDirection_Left:
                result_point_id = result_lab_point.left_connection_id;
                break; 
        }

        if (result_point_id == 0)
        {
            CORE_DebugError("Can't move to the [%u] - no connection\n", directions[i]);
            return FALSE;
        }
    }

    CORE_DebugInfo("Player %u moves to the point %u \n", instance->id, result_point_id);
    instance->position_point_id = result_point_id;
    return TRUE; 
}

void Player_SetToken(Player instance, const uint8 token[TOKEN_SIZE])
{
    memcpy(instance->token, token, TOKEN_SIZE);
}

void Player_GetTokenPtr(Player instance, const uint8 *out_token_ptr[TOKEN_SIZE])
{
    *out_token_ptr = instance->token;
}

void Player_SetId(Player instance, uint32 id)
{
    instance->id = id; 
}

void Player_GetId(Player instance, uint32 *id)
{
    *id = instance->id; 
}

void Player_GetPositionInsideLabPoint(Player instance, PositionStruct *position)
{
    *position = instance->position_inside_lab_point;
}

void Player_GetPositionPointId(Player instance, uint32 *out_position_point_id)
{
    *out_position_point_id = instance->position_point_id;
}

/*****************************************************************************************************************************/

void Player_Setup(Player instance, LabPointsMapReader points_reader, uint32 spawn_point_id)
{
    instance->position_inside_lab_point.x = 0;
    instance->position_inside_lab_point.y = 0;
    
    instance->speed_multiplier = 1;
    instance->position_point_id = spawn_point_id;
    instance->points_reader = points_reader;
}

/*****************************************************************************************************************************/

void Player_Create(Player *instance_ptr)
{
    CORE_OBJECT_CREATE(instance_ptr, Player);
}

void Player_Free(Player *instance_ptr) 
{
    CORE_OBJECT_FREE(instance_ptr); 
}
