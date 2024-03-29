#include <string.h> 

#include "CCORE.h"
#include "gameserver/player.h"

/*****************************************************************************************************************************/

#define MAX_DIRECTIONS      (2)

typedef struct Player_s
{
    LabPointsMap                lab_map;
    uint32                      id;
    float32                     speed;
    uint32                      position_point_id;
    PositionStruct              position_coords;
    uint8                       token[TOKEN_SIZE];
} *Player;

/*****************************************************************************************************************************/

static PositionStruct   _min_possible_position;
static PositionStruct   _max_possible_position;
static bool             _min_max_set = false;

static void _GetPossibleMinMaxPosition(const PositionStruct **out_min_possible_position_ptr,
                                       const PositionStruct **out_max_possible_position_ptr)
{
    if (_min_max_set == false)
    {
        _min_max_set = true;
        int32 LABPOINT_SIZE_HALF = ROOM_SIZE >> 1;
        _min_possible_position.x = -LABPOINT_SIZE_HALF;
        _min_possible_position.y = -LABPOINT_SIZE_HALF;
        _max_possible_position.x = LABPOINT_SIZE_HALF;
        _max_possible_position.y = LABPOINT_SIZE_HALF;
    }

    *out_min_possible_position_ptr = &_min_possible_position;
    *out_max_possible_position_ptr = &_max_possible_position;
}

static void _ChangePosPoint(Player player, uint32 new_point_id)
{
    player->position_point_id = new_point_id;
    LabPointsMap_AssignPlayerToPoint(player->lab_map, player, new_point_id);
}

/*****************************************************************************************************************************/

bool Player_Move(Player player, const MoveDirection *directions, uint32 directions_size)
{
    CORE_AssertPointer(directions);
    CORE_Assert(directions_size <= MAX_DIRECTIONS);

    LabPointStruct          result_lab_point;
    float32                 new_position_x;
    float32                 new_position_y;
    const PositionStruct    *min_possible_position;
    const PositionStruct    *max_possible_position;


    new_position_x = player->position_coords.x;
    new_position_y = player->position_coords.y;

    for (uint32 i = 0; i < directions_size; i++)
    {
        switch (directions[i])
        {
            case kMoveDirection_Top: 
                new_position_y += player->speed;
                break; 
            case kMoveDirection_Right:
                new_position_x += player->speed;
                break;
            case kMoveDirection_Bottom:  
                new_position_y -= player->speed;
                break; 
            case kMoveDirection_Left:
                new_position_x -= player->speed;
                break; 
            default:
                CORE_DebugError("Unknown direction\n");
                return false;
        }
    }

    _GetPossibleMinMaxPosition(&min_possible_position, &max_possible_position);
    Player_GetPositionPoint(player, &result_lab_point);
    bool blocked = false;
    if (new_position_x <= min_possible_position->x) {
        if (result_lab_point.left_connection_id == 0) {
            blocked = true;
        } else {
            _ChangePosPoint(player, result_lab_point.left_connection_id);
            new_position_x = max_possible_position->x;
        }
    }
    else if (new_position_y <= min_possible_position->y) {
        if (result_lab_point.bottom_connection_id == 0) {
            blocked = true;
        } else {
            _ChangePosPoint(player, result_lab_point.bottom_connection_id);
            new_position_y = max_possible_position->y;
        }
    }
    else if (new_position_x >= max_possible_position->x) {
        if (result_lab_point.right_connection_id == 0) {
            blocked = true;
        } else {
            _ChangePosPoint(player, result_lab_point.right_connection_id);
            new_position_x = min_possible_position->x;
        }
    }
    else if (new_position_y >= max_possible_position->y) {
        if (result_lab_point.top_connection_id == 0) {
            blocked = true;
        } else {
            _ChangePosPoint(player, result_lab_point.top_connection_id);
            new_position_y = min_possible_position->y;
        }
    }

    if (blocked) {
        // can't move there - there is a wall
        return false;
    }

    player->position_coords.x = new_position_x;
    player->position_coords.y = new_position_y;
    return true; 
}

void Player_SetToken(Player player, const uint8 token[TOKEN_SIZE])
{
    memcpy(player->token, token, TOKEN_SIZE);
}

void Player_GetTokenPtr(Player player, const uint8 *out_token_ptr[TOKEN_SIZE])
{
    *out_token_ptr = player->token;
}

void Player_SetId(Player player, uint32 id)
{
    player->id = id; 
}

void Player_GetId(Player player, uint32 *id)
{
    *id = player->id; 
}

void Player_GetPositionCoords(Player player, PositionStruct *out_position)
{
    *out_position = player->position_coords;
}

void Player_GetPositionPoint(Player player, LabPointStruct *out_point)
{
    LabPointsMap_GetPointByID(player->lab_map, player->position_point_id, out_point);
}

float32 Player_GetSpeed(Player player)
{
    return player->speed;
}

/*****************************************************************************************************************************/

void Player_Setup(Player player, LabPointsMap lab_map, uint32 spawn_point_id)
{
    player->position_coords.x = 0;
    player->position_coords.y = 0;
    
    player->speed = 0.3;
    player->position_point_id = spawn_point_id;
    player->lab_map = lab_map;
}

/*****************************************************************************************************************************/

void Player_Create(Player *player_ptr)
{
    *player_ptr = CORE_MemAlloc(sizeof(struct Player_s), 1);
    Player player = *player_ptr;

    CORE_MemZero(player->token, sizeof(player->token));
}

void Player_Free(Player *player_ptr) 
{
    CORE_MemFree(*player_ptr); 
}
