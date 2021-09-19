#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "CORE.h"
#include "common.h"
#include "lab-points-map-reader.h"

/*****************************************************************************************************************************/

typedef struct PositionStruct
{ 
	uint32	x; 
	uint32  y; 
} PositionStruct; 

CORE_OBJECT_DEFINE(Player);

/*****************************************************************************************************************************/

CORE_Bool Player_Move(Player, MoveDirection *directions, uint32 directions_size);

void Player_SetId(Player,  uint32 id);
void Player_GetId(Player, uint32 *id);
void Player_SetName(Player, char *name);
void Player_GetName(Player, char *name, uint32 name_size);
void Player_GetPositionInsideLabPoint(Player, PositionStruct *position);
void Player_GetPositionPointId(Player, uint32 *out_position_point_id);

void Player_Setup(Player, LabPointsMapReader points_reader, uint32 spawn_point_id);

void Player_Create(Player*);
void Player_Free(Player*);

/*****************************************************************************************************************************/

#endif
