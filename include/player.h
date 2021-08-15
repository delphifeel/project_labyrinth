#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "../CORE.h"

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(Player,
	uint32 	Id;
	char 	Name[40];
	// TODO: Position
)

typedef enum MoveDirection
{
	kMoveDirection_Top,
	kMoveDirection_Right,
	kMoveDirection_Bottom,
	kMoveDirection_Left,
} MoveDirection;

/*****************************************************************************************************************************/

void Player_Move(Player, MoveDirection *Directions);

void Player_SetId(Player,  uint32 Id);
void Player_GetId(Player, uint32 *Id);
void Player_SetName(Player, char *Name);
void Player_GetName(Player, char **Name);

void Player_Create(Player*);
void Player_Free(Player*);

/*****************************************************************************************************************************/

#endif