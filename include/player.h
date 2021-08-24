#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "./CORE.h"

/*****************************************************************************************************************************/

typedef struct PositionStruct
{ 
	uint32	X; 
	uint32  Y; 
} PositionStruct; 


CORE_OBJECT_INTERFACE(Player,
	uint32 				Id;
	char 				Name[40];
	PositionStruct		Position; 
	uint32				SpeedMultiplier;
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

void Player_SetId(Player,  uint32 Id);
void Player_GetId(Player, uint32 *Id);
void Player_SetName(Player, char *Name);
void Player_GetName(Player, char *Name, uint32 NameSize);
void Player_GetPosition(Player, PositionStruct *Position);

void Player_Setup(Player, PositionStruct Position);

void Player_Create(Player*);
void Player_Free(Player*);

/*****************************************************************************************************************************/

#endif