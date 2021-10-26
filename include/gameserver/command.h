#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "CORE.h"
#include "common.h"
#include "lab-session.h"

/*****************************************************************************************************************************/

typedef struct CommandHeader CommandHeader;

typedef struct Command
{
	CommandHeader 	*_header;
	uint8 			*_payload;
} Command;

/*****************************************************************************************************************************/

typedef enum CommandType
{
	kCommandType_PlayerMove,
	kCommandType_StartGame,
} CommandType;

/*****************************************************************************************************************************/

void Command_GetType(Command *instance, CommandType *out_command_type);
void Command_GetSessionIndex(Command *instance, uint32 *out_session_index);
void Command_GetPlayerIndex(Command *instance, uint32 *out_player_index);
void Command_GetPayload(Command *instance, const uint8 **out_payload);

CORE_Bool Command_ParseFromBuffer(Command *instance, const uint8 *buffer_ptr);
void Command_Process(Command *instance, LabSession sessions[], uint32 sessions_size);
void Command_Init(Command *instance);

#endif
