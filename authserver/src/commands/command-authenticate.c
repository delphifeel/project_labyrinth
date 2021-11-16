#include "lib/commands-processor/command.h"
#include "authserver/auth-command-types.h"
#include "authserver/CONFIG.h"

#define MAX_LOGIN_SIZE         (36)
#define MAX_PASSWORD_SIZE      (24)

typedef struct AuthenticatePayload
{
    // TODO(dhatz): UTF-8 support. 
    char                login[MAX_LOGIN_SIZE];
    char                password[MAX_PASSWORD_SIZE]; 
} AuthenticatePayload; 

typedef struct AuthenticateResponsePayload
{
    uint8                token[TOKEN_SIZE];
} AuthenticateResponsePayload;

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
    CORE_Bool                       user_found;
    AuthenticateResponsePayload     response_payload;

    Command_GetPayloadPtr(command, &payload_raw, &payload_size); 

    if (payload_size != sizeof(AuthenticatePayload))
    {
        CORE_DebugError("payload_size != sizeof(AuthenticatePayload)\n");
        return FALSE; 
    }

    payload = (const AuthenticatePayload *) payload_raw; 

    user_found = FALSE;
    creds_array_size = sizeof(_mocked_creds) / sizeof(AuthenticatePayload);
    for (uint32 i = 0; i < creds_array_size; i++)
    {
        if ((CORE_StringEqual(payload->login, _mocked_creds[i].login) == TRUE) && 
            (CORE_StringEqual(payload->password, _mocked_creds[i].password) == TRUE))
        {
            CORE_DebugInfo("Found user: %s - return auth creds\n", payload->login);
            user_found = TRUE;
            break;
        }
    }

    if (user_found == FALSE)
    {
        CORE_DebugError("Can't login - user not found: %s\n", payload->login);
        return FALSE;
    }


    *out_is_have_response = TRUE;
    Command_SetType(out_response_command, kCommandResponseType_Authenticate);

    memcpy(response_payload.token, mocked_token, TOKEN_SIZE);
    if (Command_SetPayload( out_response_command, 
                            (const uint8 *) &response_payload, 
                            sizeof(AuthenticateResponsePayload)) == FALSE)
    {
        return FALSE;
    }

    return TRUE; 
}
