#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "CORE.h"
#include "common.h"

/*****************************************************************************************************************************/

typedef enum CommandType
{
	kCommandType_PlayerMove,
} CommandType;

/*****************************************************************************************************************************/

typedef struct CommandPayload_PlayerMove
{
	uint32 			directions_size;
	MoveDirection 	*directions;
} CommandPayload_PlayerMove;

typedef struct CommandStruct 
{
	CommandType 	type;
	uint32			player_index;
	uint32			session_index;
	union
	{
		CommandPayload_PlayerMove	player_move_payload;
	};
} CommandStruct;

#endif
