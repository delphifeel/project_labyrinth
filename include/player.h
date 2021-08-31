#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "./CORE.h"
#include "common.h"

/*****************************************************************************************************************************/

typedef struct PositionStruct
{ 
	uint32	X; 
	uint32  Y; 
} PositionStruct; 


CORE_OBJECT_INTERFACE(Player,
	uint32 				Id;
	uint32				SpeedMultiplier;
	uint32  			PositionPointId;
	PositionStruct		PositionInsideLabPoint;
	LabSession 			Session;
	char 				Name[40];
)

/*****************************************************************************************************************************/

CORE_Bool Player_Move(Player, MoveDirection *Directions, uint32 DirectionsSize);

void Player_SetId(Player,  uint32 Id);
void Player_GetId(Player, uint32 *Id);
void Player_SetName(Player, char *Name);
void Player_GetName(Player, char *Name, uint32 NameSize);
void Player_GetPositionInsideLabPoint(Player, PositionStruct *Position);

void Player_Setup(Player, LabSession Session, uint32 SpawnPointId);

void Player_Create(Player*);
void Player_Free(Player*);

/*****************************************************************************************************************************/

#endif