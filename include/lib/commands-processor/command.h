#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <CCORE.h>

#define COMMAND_MAX_PAYLOAD_SIZE    (512)

struct Command
{
    uint32      type;
    uint32      payload_size;
    uint8       payload[COMMAND_MAX_PAYLOAD_SIZE];
};

void        Command_GetType(struct Command *instance, uint32 *out_command_type);
void        Command_GetPayloadPtr(struct Command *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size);
void        Command_SetType(struct Command *instance, uint32 command_type);
bool        Command_SetPayload(struct Command *instance, const uint8 payload[], uint32 payload_size);

void        Command_Init(struct Command *instance);

#endif
