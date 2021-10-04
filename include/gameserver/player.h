#ifndef _PLAYER_H_
#define _PLAYER_H_

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

CORE_Bool Player_Move(Player instance, const MoveDirection *directions, uint32 directions_size);

void Player_SetId(Player instance,  uint32 id);
void Player_GetId(Player instance, uint32 *id);
void Player_SetName(Player instance, char *name);
void Player_GetName(Player instance, char *name, uint32 name_size);
void Player_GetPositionInsideLabPoint(Player instance, PositionStruct *position);
void Player_GetPositionPointId(Player instance, uint32 *out_position_point_id);

void Player_Setup(Player instance, LabPointsMapReader points_reader, uint32 spawn_point_id);

void Player_Create(Player* instance_ptr);
void Player_Free(Player* instance_ptr);

/*****************************************************************************************************************************/

#endif
