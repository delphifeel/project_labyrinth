#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "../CORE.h"

/*****************************************************************************************************************************/

typedef enum CommandType
{
	kCommandType_PlayerMove,
} CommandType;

/*****************************************************************************************************************************/

typedef struct CommandPayload_PlayerMove
{
	uint32 DirectionsSize;
	uint32 *Directions;
} CommandPayload_PlayerMove;

typedef struct CommandStruct 
{
	CommandType 	Type;
	uint32			PlayerId;
	union
	{
		CommandPayload_PlayerMove	PlayerMovePayload;
	};
} CommandStruct;

#endif