#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "CORE.h"
#include "common.h"
#include "labyrinth/lab-points-map-reader.h"

/*****************************************************************************************************************************/

typedef struct PositionStruct
{ 
	uint32	X; 
	uint32  Y; 
} PositionStruct; 

CORE_OBJECT_DEFINE(Player);

/*****************************************************************************************************************************/

CORE_Bool Player_Move(Player, MoveDirection *Directions, uint32 DirectionsSize);

void Player_SetId(Player,  uint32 Id);
void Player_GetId(Player, uint32 *Id);
void Player_SetName(Player, char *Name);
void Player_GetName(Player, char *Name, uint32 NameSize);
void Player_GetPositionInsideLabPoint(Player, PositionStruct *Position);
void Player_GetPositionPointId(Player, uint32 *OUT_PositionPointId);

void Player_Setup(Player, LabPointsMapReader PointsReader, uint32 SpawnPointId);

void Player_Create(Player*);
void Player_Free(Player*);

/*****************************************************************************************************************************/

#endif