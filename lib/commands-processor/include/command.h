#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "CORE.h"
#include "command-private.h"

typedef struct _Command Command;

void 		Command_GetType(Command *instance, uint32 *out_command_type);
void 		Command_GetPayloadPtr(Command *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size);
void 		Command_SetType(Command *instance, uint32 command_type);
CORE_Bool 	Command_SetPayload(Command *instance, const uint8 payload[], uint32 payload_size);

void 		Command_Init(Command *instance);

#endif
