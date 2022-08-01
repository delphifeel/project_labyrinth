#include "CCORE.h"
#include "modules/player/player.h"

/*****************************************************************************************************************************/

typedef struct Player_s
{
    uint      id;
} Player;

/*****************************************************************************************************************************/

// bool Player_Move(Player *player, MoveDirection direction)
// {
//     CORE_AssertPointer(player);

//     LabPoint  result_lab_point;
//     uint      new_point_id;


//     Player_GetPositionPoint(player, &result_lab_point);
//     switch (direction)
//     {
//     case kMoveDirection_Top:
//         new_point_id = result_lab_point.top_connection_id;
//         break;
//     case kMoveDirection_Right:
//         new_point_id = result_lab_point.right_connection_id;
//         break;
//     case kMoveDirection_Bottom:
//         new_point_id = result_lab_point.bottom_connection_id;
//         break;
//     case kMoveDirection_Left:
//         new_point_id = result_lab_point.left_connection_id;
//         break;
//     default:
//         // unknown direction
//         return false;
//     }

//     if (new_point_id == 0) {
//         // no connection there
//         return false;
//     }

//     _ChangePosPoint(player, new_point_id);
//     return true; 
// }

uint Player_GetId(const Player *player)
{
    CORE_AssertPointer(player);


    return player->id;
}

/*****************************************************************************************************************************/

Player *Player_Create(uint id)
{
    Player *player = CORE_MemAlloc(sizeof(Player), 1);
    player->id = id;
    return player;
}

void Player_Free(Player *player) 
{
    CORE_AssertPointer(player);


    CORE_MemFree(player); 
}