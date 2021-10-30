#include "authserver/message-processor.h"


void _FillBuffer(uint32 message_type, const uint8 payload[], uint32 payload_size, uint8 buffer[])
{
    uint8               *buffer_ptr; 

    buffer_ptr = buffer; 
    memcpy(buffer_ptr, &message_type, sizeof(message_type)); 
    buffer_ptr += sizeof(message_type); 
    memcpy(buffer_ptr, payload, payload_size);
}

/* 
 *		command `Authenticate` contains:
 *			- 0..4 	    (4 bytes)	    -> command header
 *			- 4..40	    (36 bytes)	    -> login	
 *			- 40..64	(24 bytes)	    -> password
 */

void Test_MessageAuthenticate()
{
    Message                     message;
    uint8                       buffer[64];
    uint8                       payload[60]; 
    uint8                       *payload_ptr; 

    uint8 login[] = "admin"; 
    uint8 password[] = "admin"; 

    CORE_MemZero(&payload, sizeof(payload)); 
    payload_ptr = payload; 

    memcpy(payload_ptr, login, sizeof(login));
    payload_ptr += 36;

    memcpy(payload_ptr, password, sizeof(password)); 

    _FillBuffer(kMessageAuthenticate, payload, sizeof(payload), buffer); 

    Message_Init(&message); 
    Message_ParseFromBuffer(&message, buffer, sizeof(buffer));

    assert(MessageProcessor_Process(&message) == TRUE);

}


void Test_MessageProcessor()
{
    Test_MessageAuthenticate();
}
