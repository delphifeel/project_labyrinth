#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "CCORE.h"
#include "player-token.h"

// TODO: tests
typedef struct Player_s Player;

/*****************************************************************************************************************************/

uint Player_GetId(const Player *player);

Player *Player_Create(uint id);
void Player_Free(Player *player);

/*****************************************************************************************************************************/

#endif