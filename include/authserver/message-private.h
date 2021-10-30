#ifndef _MESSAGE_PRIVATE_H_
#define _MESSAGE_PRIVATE_H_


#define _MESSAGE_MAX_PAYLOAD_SIZE           (256)


typedef struct _MessageHeader
{
    uint32                  type; 
} _MessageHeader; 


struct _Message
{
    CORE_Bool               is_header_set; 
    _MessageHeader          header; 
    uint32                  payload_size; 
    uint8                   payload[_MESSAGE_MAX_PAYLOAD_SIZE];
};


#endif
