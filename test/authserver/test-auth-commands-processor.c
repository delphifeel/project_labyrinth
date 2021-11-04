#include "commands-processor.h"
#include "authserver/auth-command-types.h"
#include "authserver/auth-commands-processor.h"

/**
 *		command `Authenticate` contains:
 *			- 0..4 	    (4 bytes)	    -> command type
 *			- 4..40	    (36 bytes)	    -> login	
 *			- 40..64	(24 bytes)	    -> password
 */

void Test_CommandAuthenticate()
{
    CORE_Bool                is_have_response;
    AuthCommandsProcessor    commands_processor;
    struct Command           command;
    struct Command           response_command;
    uint32                   command_type;
    uint8                    payload[60];
    uint8                    *payload_ptr;
    uint8                    login[36]      = "delphifeel"; 
    uint8                    password[24]   = "1234"; 

    
    command_type = kCommandType_Authenticate;
    payload_ptr = payload;

    memcpy(payload_ptr, login, sizeof(login)); 
    payload_ptr += sizeof(login);

    memcpy(payload_ptr, password, sizeof(password)); 

    Command_Init(&response_command);

    Command_Init(&command); 
    Command_SetType(&command, command_type);
    Command_SetPayload(&command, payload, sizeof(payload));

    AuthCommandsProcessor_Create(&commands_processor);
    AuthCommandsProcessor_Setup(commands_processor, GetAuthCommandToProcessFunc(), GetAuthCommandToProcessFuncSize());
    CORE_Assert(AuthCommandsProcessor_Process(commands_processor, command_type, &command, &response_command, &is_have_response) == TRUE);

    AuthCommandsProcessor_Free(&commands_processor);
}


void Test_AuthCommandsProcessor()
{
    Test_CommandAuthenticate();
}
