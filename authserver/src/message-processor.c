#include "authserver/message-processor.h"

/*****************************************************************************************************************************/


#define _DEFINE_PROCESS_FUNC(_NAME)                                                     \
    extern CORE_Bool _NAME(Message *message)                                            \

/*****************************************************************************************************************************/


typedef CORE_Bool (*MessageProcessorFunc)(Message *message);


typedef struct MessageProcessorStruct
{
    MessageType             message_type; 
    MessageProcessorFunc    process_cb; 
} MessageProcessorStruct;

/*****************************************************************************************************************************/


_DEFINE_PROCESS_FUNC(MessageAuthenticate_Process);


static MessageProcessorStruct _message_processors[] = 
{
    {   kMessageAuthenticate,      MessageAuthenticate_Process   }
};


CORE_Bool MessageProcessor_Process(Message *message)
{
    MessageType                     message_type; 
    MessageProcessorStruct          message_processor; 

    Message_GetType(message, &message_type); 

    message_processor = _message_processors[message_type]; 
    if (message_processor.message_type != message_type)
    {
        CORE_DebugError("Wrong association in `_message_processors`. Fix `_message_processors` order to resolve\n");
        return FALSE;
    }

    if (message_processor.process_cb(message) == FALSE)
    {

    }

    return TRUE; 
}
