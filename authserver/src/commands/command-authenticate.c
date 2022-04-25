#include "lib/commands-processor/command.h"
#include "authserver/auth-command-types.h"
#include "authserver/config.h"
#include "authserver/account.h"




typedef struct AuthenticateResponsePayload
{
    uint8                token[TOKEN_SIZE];
} AuthenticateResponsePayload;



bool CommandAuthenticate_Process(  struct Command 	*command, 
                                        struct Command 	*out_response_command, 
                                        bool     	*out_is_have_response)
{
    const AuthenticatePayload       *payload;
    const uint8                     *payload_raw;
    uint32                          payload_size; 
    uint32                          creds_array_size;
    AuthenticateResponsePayload     response_payload;

    Command_GetPayloadPtr(command, &payload_raw, &payload_size); 

    if (payload_size != sizeof(AuthenticatePayload))
    {
        CORE_DebugError("payload_size != sizeof(AuthenticatePayload)\n");
        return false; 
    }

    payload = (const AuthenticatePayload *) payload_raw; 

    if(!Account_LogIn(payload->login, payload->password)){
        return false; 
    }
    
    *out_is_have_response = true;
    Command_SetType(out_response_command, kCommandResponseType_Authenticate);

    memcpy(response_payload.token, mocked_token, TOKEN_SIZE);
    if (Command_SetPayload( out_response_command, 
                            (const uint8 *) &response_payload, 
                            sizeof(AuthenticateResponsePayload)) == false)
    {
        return false;
    }

    return true; 
}
