#include <string.h> 

#include "CCORE.h"
#include "gameserver/player.h"

/*****************************************************************************************************************************/

#define MAX_DIRECTION_SIZE      (2)
#define SPEED                   (1)

CORE_OBJECT_INTERFACE(Player,
    uint32                      id;
    uint32                      speed_multiplier;
    uint32                      position_point_id;
    PositionStruct              position_coords;
    LabPointsMapReader          points_reader;
    uint8                       token[TOKEN_SIZE];
);

/*****************************************************************************************************************************/

static PositionStruct _min_possible_position;
static PositionStruct _max_possible_position;
static bool      _min_max_set = false;

static void _GetPossibleMinMaxPosition(const PositionStruct **out_min_possible_position_ptr,
                                       const PositionStruct **out_max_possible_position_ptr)
{
    if (_min_max_set == false)
    {
        _min_max_set = true;
        int32 LABPOINT_SIZE_HALF = LABPOINT_SIZE >> 1;
        _min_possible_position.x = -LABPOINT_SIZE_HALF;
        _min_possible_position.y = -LABPOINT_SIZE_HALF;
        _max_possible_position.x = LABPOINT_SIZE_HALF;
        _max_possible_position.y = LABPOINT_SIZE_HALF;
    }

    *out_min_possible_position_ptr = &_min_possible_position;
    *out_max_possible_position_ptr = &_max_possible_position;
}

/*****************************************************************************************************************************/

bool Player_Move(Player instance, const MoveDirection *directions, uint32 directions_size)
{
    CORE_AssertPointer(directions);

    LabPointStruct          result_lab_point;
    uint32                  result_point_id;
    int32                   new_position_x;
    int32                   new_position_y;
    const PositionStruct    *min_possible_position;
    const PositionStruct    *max_possible_position;
    bool               can_move;


    if (directions_size > MAX_DIRECTION_SIZE) 
    {
        CORE_DebugError("directions max size is 2\n"); 
        return false; 
    }

    new_position_x = instance->position_coords.x;
    new_position_y = instance->position_coords.y;
    result_point_id = instance->position_point_id;

    for (uint32 i = 0; i < directions_size; i++)
    {
        switch (directions[i])
        {
            case kMoveDirection_Top: 
                new_position_y++;
                break; 
            case kMoveDirection_Right:
                new_position_x++;
                break;
            case kMoveDirection_Bottom:  
                new_position_y--;
                break; 
            case kMoveDirection_Left:
                new_position_x--;
                break; 
            default:
                CORE_DebugError("Unknown direction\n");
                return false;
        }
    }

    _GetPossibleMinMaxPosition(&min_possible_position, &max_possible_position);
    Player_GetPositionPoint(instance, &result_lab_point);
    can_move = true;
    do
    {
        if (new_position_x < min_possible_position->x)
        {
            if (result_lab_point.left_connection_id != 0)
            {
                result_point_id = result_lab_point.left_connection_id;
                new_position_x = max_possible_position->x;
                break;
            }
            can_move = false;
        }
        if (new_position_y < min_possible_position->y)
        {
            if (result_lab_point.bottom_connection_id != 0)
            {
                result_point_id = result_lab_point.bottom_connection_id;
                new_position_y = max_possible_position->y;
                break;
            }
            can_move = false;
        }
        if (new_position_x > max_possible_position->x)
        {
            if (result_lab_point.right_connection_id != 0)
            {
                result_point_id = result_lab_point.right_connection_id;
                new_position_x = min_possible_position->x;
                break;
            }
            can_move = false;
        }
        if (new_position_y > max_possible_position->y)
        {
            if (result_lab_point.top_connection_id != 0)
            {
                result_point_id = result_lab_point.top_connection_id;
                new_position_y = min_possible_position->y;
                break;
            }
            can_move = false;
        }
    } while (0);

    if (can_move == false)
    {
        CORE_DebugError("Can't move that side - there is a wall\n");
        return false;
    }

    if (result_point_id != instance->position_point_id)
    {
        CORE_DebugInfo("Player %u moves to the point %u \n", instance->id, result_point_id);
        instance->position_point_id = result_point_id;
    }

    CORE_DebugInfo("Player %u moves to the coords [%d:%d]\n", instance->id, new_position_x, new_position_y);
    instance->position_coords.x = new_position_x;
    instance->position_coords.y = new_position_y;
    return true; 
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

void Player_GetPositionCoords(Player instance, PositionStruct *out_position)
{
    *out_position = instance->position_coords;
}

void Player_GetPositionPoint(Player instance, LabPointStruct *out_point)
{
    LabPointsMapReader_GetPointByID(instance->points_reader, instance->position_point_id, out_point);
}

/*****************************************************************************************************************************/

void Player_Setup(Player instance, LabPointsMapReader points_reader, uint32 spawn_point_id)
{
    instance->position_coords.x = 0;
    instance->position_coords.y = 0;
    
    instance->speed_multiplier = 1;
    instance->position_point_id = spawn_point_id;
    instance->points_reader = points_reader;
}

/*****************************************************************************************************************************/

void Player_Create(Player *instance_ptr)
{
    CORE_OBJECT_CREATE(instance_ptr, Player);
    Player instance = *instance_ptr;

    CORE_MemZero(instance->token, sizeof(instance->token));
}

void Player_Free(Player *instance_ptr) 
{
    CORE_OBJECT_FREE(instance_ptr); 
}
