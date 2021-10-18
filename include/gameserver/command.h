#ifndef _COMMAND_H_
#define _COMMAND_H_

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

typedef union 
{
	CommandPayload_PlayerMove	player_move;
} CommandPayload;

typedef struct CommandStruct 
{
	uint32 			verification_id;
	CommandType 	type;
	uint32			player_index;
	uint32			session_index;
	CommandPayload 	payload;
} CommandStruct;

#define _VERIFICATION_ID  					(0xDEAD)

#define Command_Verificate(COMMAND_PTR)		((COMMAND_PTR)->verification_id == _VERIFICATION_ID)

#endif
