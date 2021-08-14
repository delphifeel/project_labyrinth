#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "./CORE.h"

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(Position, 
	uint32	positionX; 
	uint32  positionY; 
)


CORE_OBJECT_INTERFACE(Player,
	uint32 		Id;
	char 		Name[40];
	Position	position; 
	uint32		SpeedMultiplier;
)

typedef enum MoveDirection
{
	kMoveDirection_Top,
	kMoveDirection_Right,
	kMoveDirection_Bottom,
	kMoveDirection_Left,
} MoveDirection;

/*****************************************************************************************************************************/

CORE_Bool Player_Move(Player, MoveDirection *Directions, uint32 DirectionsSize);

void Player_SetId(Player, uint32 Id);
void Player_GetId(Player, uint32 *Id);
void Player_SetName(Player, char *Name);
void Player_GetName(Player, char *Name);

void Player_Create(Player*, Position);
void Player_Free(Player*);

/*****************************************************************************************************************************/

#endif