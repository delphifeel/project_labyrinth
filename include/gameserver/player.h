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

void Player_SetToken(Player instance, const uint8 token[TOKEN_SIZE]);
void Player_GetTokenPtr(Player instance, const uint8 *out_token_ptr[TOKEN_SIZE]);
void Player_SetId(Player instance, uint32 id);
void Player_GetId(Player instance, uint32 *id);
void Player_GetPositionInsideLabPoint(Player instance, PositionStruct *position);
void Player_GetPositionPointId(Player instance, uint32 *out_position_point_id);

void Player_Setup(Player instance, LabPointsMapReader points_reader, uint32 spawn_point_id);

void Player_Create(Player* instance_ptr);
void Player_Free(Player* instance_ptr);

/*****************************************************************************************************************************/

#endif
