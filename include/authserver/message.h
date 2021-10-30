#ifndef _MESSAGE_H_
#define _MESSAGE_H_


#include "CORE.h"

#include "message-private.h"

/*****************************************************************************************************************************/

typedef struct _Message Message; 


/*****************************************************************************************************************************/

typedef enum MessageType
{
    kMessageAuthenticate,
} MessageType; 

/*****************************************************************************************************************************/

void Message_GetType(Message *instance, MessageType *out_message_type);
void Message_GetPayloadPtr(Message *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size); 

CORE_Bool Message_ParseFromBuffer(Message *instance, const uint8 buffer[], uint32 buffer_size); 
void Message_Init(Message *instance); 

#endif
