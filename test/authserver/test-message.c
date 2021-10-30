#include "authserver/message.h"


void Test_Message()
{
    Message             message; 
    uint8               buffer[_MESSAGE_MAX_PAYLOAD_SIZE];
    uint8               *buffer_ptr; 
    uint32              message_type; 
    const   uint8       *payload;
    uint32              payload_size; 
    uint32              temp; 

    message_type = kMessageAuthenticate; 
    buffer_ptr = buffer;
    CORE_MemZero(&buffer, sizeof(buffer)); 
    memcpy(buffer, &message_type, sizeof(message_type));
    buffer_ptr += sizeof(message_type); 

    Message_Init(&message); 
    assert(Message_ParseFromBuffer(&message, buffer, sizeof(buffer)) == TRUE); 

    Message_GetType(&message, &temp); 
    assert(temp == message_type); 

    Message_GetPayloadPtr(&message, &payload, &payload_size); 
    assert(payload_size == sizeof(buffer) - sizeof(uint32)); 
}
