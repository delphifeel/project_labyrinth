#include "authserver/message.h"

#define MAX_LOGIN_SIZE         (36)
#define MAX_PASSWORD_SIZE      (24)


typedef struct AuthenticatePayload
{
    // TODO: UTF-8 support. 
    uint8                login[MAX_LOGIN_SIZE];
    uint8                password[MAX_PASSWORD_SIZE]; 
} AuthenticatePayload; 


CORE_Bool MessageAuthenticate_Process(Message *message)
{
    const AuthenticatePayload       *payload;
    const uint8                     *payload_raw;
    uint32                          payload_size; 

    Message_GetPayloadPtr(message, &payload_raw, &payload_size); 

    if (payload_size != sizeof(AuthenticatePayload))
    {
        CORE_DebugError("payload_size != sizeof(AuthenticatePayload)\n");
        return FALSE; 
    }

    payload = (const AuthenticatePayload *) payload_raw; 

    // TODO: Search for user. 

    return TRUE; 
}