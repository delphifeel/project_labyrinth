#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "CORE.h"
#include "common.h"
#include "lab-session.h"

/*****************************************************************************************************************************/

#include "command-private.h"

typedef struct _Command Command;

/*****************************************************************************************************************************/

typedef enum CommandType
{
	kCommandType_PlayerMove,
	kCommandType_StartGame,
} CommandType;

/*****************************************************************************************************************************/

void 		Command_GetType(Command *instance, CommandType *out_command_type);
void 		Command_GetSessionIndex(Command *instance, uint32 *out_session_index);
void 		Command_GetPlayerIndex(Command *instance, uint32 *out_player_index);
void 		Command_GetPayloadPtr(Command *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size);

CORE_Bool 	Command_ParseFromBuffer(Command *instance, const uint8 buffer[], uint32 buffer_size);
void 		Command_Init(Command *instance);

#endif
