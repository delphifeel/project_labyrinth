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
	uint32 PlayerId;
	uint32 Direction;
} CommandPayload_PlayerMove;

typedef struct CommandStruct 
{
	CommandType 	Type;
	
	union 
	{
		CommandPayload_PlayerMove 	PlayerMovePayload;
	};
} CommandStruct;

/*****************************************************************************************************************************/

#endif