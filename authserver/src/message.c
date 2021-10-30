#include "authserver/message.h"


/*****************************************************************************************************************************/

void Message_GetType(Message *instance, MessageType *out_message_type)
{
    CORE_Assert(instance->is_header_set == TRUE); 
    CORE_AssertPointer(out_message_type); 

    *out_message_type = (MessageType) instance->header.type; 
}


void Message_GetPayloadPtr(Message *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size)
{
    CORE_AssertPointer(instance->payload); 
    CORE_AssertPointer(out_payload_ptr); 
    CORE_AssertPointer(out_payload_size);

    *out_payload_ptr = instance->payload; 
    *out_payload_size = instance->payload_size; 
}


/*****************************************************************************************************************************/

CORE_Bool Message_ParseFromBuffer(Message *instance, const uint8 buffer[], uint32 buffer_size)
{
    const   uint8       *buffer_ptr; 
    uint32              buffer_size_left; 

    buffer_ptr = buffer; 
    buffer_size_left = buffer_size; 

    // 0...4 (uint32) - Message Header 
    memcpy(&instance->header, buffer_ptr, sizeof(_MessageHeader)); 
    buffer_ptr += sizeof(_MessageHeader); 
    buffer_size_left -= sizeof(_MessageHeader); 
    instance->is_header_set = TRUE;

    if (buffer_size_left > _MESSAGE_MAX_PAYLOAD_SIZE)
    {
        CORE_DebugError("buffer size > message max payload size\n"); 
        return FALSE; 
    }

    instance->payload_size = buffer_size_left; 
    memcpy(&instance->payload, buffer_ptr, instance->payload_size); 

    return TRUE; 
}


void Message_Init(Message *instance)
{
    CORE_MemZero(instance, sizeof(Message)); 
}
