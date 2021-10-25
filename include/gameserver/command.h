#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "CORE.h"
#include "common.h"
#include "lab-session.h"

#include "command-private.h"

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
} CommandType;

typedef CORE_Bool (*CommandProcessFunc)(Command *command, LabSession sessions[], uint32 sessions_size);
typedef struct CommandProcessorStruct
{
	CommandType 		command_type;
	CommandProcessFunc 	process_cb;
} CommandProcessorStruct;

extern CommandProcessorStruct CommandsProcessors[];

/*****************************************************************************************************************************/

CORE_Bool Command_ValidateBytes(const uint8 data[]);

void Command_GetType(Command *instance, CommandType *out_command_type);
void Command_GetSessionIndex(Command *instance, uint32 *out_session_index);
void Command_GetPlayerIndex(Command *instance, uint32 *out_player_index);
void Command_GetPayload(Command *instance, uint8 **out_payload);

CORE_Bool Command_ParseFromBuffer(Command *instance, const uint8 buffer[]);
void Command_Init(Command *instance);

#endif
