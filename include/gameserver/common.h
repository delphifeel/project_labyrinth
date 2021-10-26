#ifndef _COMMON_H_
#define _COMMON_H_

#include "CORE/CORE-types.h"

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

/*****************************************************************************************************************************/

#endif
