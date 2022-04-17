#ifndef _GS_COMMON_H_
#define _GS_COMMON_H_

#include "CCORE.h"

/*****************************************************************************************************************************/

#define ID_HASH(id) 	((id) - 1)
#define TOKEN_SIZE 		(32)

/*****************************************************************************************************************************/

typedef enum MoveDirection
{
	kMoveDirection_Top = 1,
	kMoveDirection_Right,
	kMoveDirection_Bottom,
	kMoveDirection_Left,
} MoveDirection;

typedef struct PositionStruct
{ 
	float32		x; 
	float32  	y; 
} PositionStruct; 

typedef struct LabPointStruct 
{
    uint32          Id; 
    uint32          top_connection_id; 
    uint32          right_connection_id; 
    uint32          bottom_connection_id; 
    uint32          left_connection_id; 
    bool            is_exit; 
    bool            is_spawn;
} LabPointStruct;

/*****************************************************************************************************************************/

#endif
