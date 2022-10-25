#ifndef _TURN_STATE_H_
#define _TURN_STATE_H_

#include "libs/core/core.h"
#include "modules/labyrinth/player/move-direction.h"

struct TurnState
{
    bool            IsPlayerMoving;
    MoveDirection   PlayerMoveDirection;
};

#endif