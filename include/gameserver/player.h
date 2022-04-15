#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "CCORE.h"
#include "common.h"
#include "lab-points-map-reader.h"

/*****************************************************************************************************************************/

typedef struct PositionStruct
{ 
	float32		x; 
	float32  	y; 
} PositionStruct; 

CORE_OBJECT_DEFINE(Player);

/*****************************************************************************************************************************/

bool Player_Move(Player player, const MoveDirection *directions, uint32 directions_size);

void Player_SetToken(Player player, const uint8 token[TOKEN_SIZE]);
void Player_GetTokenPtr(Player player, const uint8 *out_token_ptr[TOKEN_SIZE]);
void Player_SetId(Player player, uint32 id);
void Player_GetId(Player player, uint32 *id);
void Player_GetPositionCoords(Player player, PositionStruct *out_position);
void Player_GetPositionPoint(Player player, LabPointStruct *out_point);
float32 Player_GetSpeed(Player player);

void Player_Setup(Player player, LabPointsMapReader points_reader, uint32 spawn_point_id);

void Player_Create(Player* player_ptr);
void Player_Free(Player* player_ptr);

/*****************************************************************************************************************************/

#endif
