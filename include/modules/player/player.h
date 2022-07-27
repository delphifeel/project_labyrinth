#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "CCORE.h"
#include "player-token.h"

// TODO: tests
typedef struct Player_s Player;

/*****************************************************************************************************************************/

void Player_CopyToken(Player *player, const uint8 token[PLAYER_TOKEN_SIZE]);
void Player_GetToken(const Player *player, const uint8 *token_ptr[PLAYER_TOKEN_SIZE]);
uint Player_GetId(const Player *player);

Player *Player_Create(uint id);
void Player_Free(Player *player);

/*****************************************************************************************************************************/

#endif