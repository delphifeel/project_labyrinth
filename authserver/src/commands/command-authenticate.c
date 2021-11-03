#include "command.h"

#define MAX_LOGIN_SIZE         (36)
#define MAX_PASSWORD_SIZE      (24)


typedef struct AuthenticatePayload
{
    // TODO(dhatz): UTF-8 support. 
    char                login[MAX_LOGIN_SIZE];
    char                password[MAX_PASSWORD_SIZE]; 
} AuthenticatePayload; 

static const AuthenticatePayload _mocked_creds[] = 
{
    {"delphifeel", "1234"},
    {"dhatz", "1234"},
    {"cherniki", "12345"},
};

CORE_Bool CommandAuthenticate_Process(  struct Command 	*command, 
                                        struct Command 	*out_response_command, 
                                        CORE_Bool     	*out_is_have_response)
{
    const AuthenticatePayload       *payload;
    const uint8                     *payload_raw;
    uint32                          payload_size; 
    uint32                          creds_array_size;

    Command_GetPayloadPtr(command, &payload_raw, &payload_size); 

    if (payload_size != sizeof(AuthenticatePayload))
    {
        CORE_DebugError("payload_size != sizeof(AuthenticatePayload)\n");
        return FALSE; 
    }

    payload = (const AuthenticatePayload *) payload_raw; 

    creds_array_size = sizeof(_mocked_creds) / sizeof(AuthenticatePayload);
    for (uint32 i = 0; i < creds_array_size; i++)
    {
        if ((CORE_StringEqual(payload->login, _mocked_creds[i].login) == TRUE) && 
            (CORE_StringEqual(payload->password, _mocked_creds[i].password) == TRUE))
        {
            CORE_DebugInfo("Found user: %s - return auth creds\n", payload->login);
            return TRUE;
        }
    }

    return FALSE; 
}
