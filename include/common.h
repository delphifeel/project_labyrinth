#ifndef __COMMON_H__
#define __COMMON_H__

#include "CORE/CORE-types.h"

/*****************************************************************************************************************************/

#define ID_HASH(id) 	(id-1)

/*****************************************************************************************************************************/

typedef enum MoveDirection
{
	kMoveDirection_Top,
	kMoveDirection_Right,
	kMoveDirection_Bottom,
	kMoveDirection_Left,
} MoveDirection;

/*****************************************************************************************************************************/

#endif
