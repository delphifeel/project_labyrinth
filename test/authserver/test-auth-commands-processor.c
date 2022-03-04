#include "authserver/auth-command-types.h"
#include "authserver/auth-commands-processor.h"
#include "authserver/CONFIG.h"

#define INIT_PAYLOAD(_size)                         \
    const uint32    _payload_max_size = (_size);    \
    uint8           _payload[_payload_max_size];    \
    uint8           *_payload_ptr;                  \
    _payload_ptr = _payload;

#define WRITE_TO_PAYLOAD(_data_ptr, _data_size)                     \
    if (_payload_ptr + _data_size > _payload + _payload_max_size)   \
    {                                                               \
        CORE_DebugError("Payload reached max size\n");              \
        return;                                                     \
    }                                                               \
    memcpy(_payload_ptr, _data_ptr, _data_size);                    \
    _payload_ptr += _data_size;

#define GET_PAYLOAD     _payload

/**
 *      command `JoinLobby` contains:
 *          - 0..4      (4 bytes)       -> command type
 *          - 4..36     (32 bytes)      -> token
 * 
 *      response contains:
 *          - 0..4      (4 bytes)       -> command response type
 *          - 4..8     (4 bytes)        -> is ok
 */

void Test_CommandJoinLobby(AuthCommandsProcessor commands_processor)
{
    CORE_Bool                is_have_response;
    struct Command           command;
    struct Command           response_command;
    uint32                   command_type;
    const uint8              *response_payload_ptr;
    uint32                   response_payload_size;

    
    command_type = kCommandType_JoinLobby;
    INIT_PAYLOAD(32);
    WRITE_TO_PAYLOAD(mocked_token, TOKEN_SIZE);

    Command_Init(&response_command);

    Command_Init(&command); 
    Command_SetType(&command, command_type);
    Command_SetPayload(&command, GET_PAYLOAD, sizeof(GET_PAYLOAD));
    
    CORE_Assert(AuthCommandsProcessor_Process(commands_processor, command_type, &command, &response_command, &is_have_response) == TRUE);
    CORE_Assert(is_have_response == TRUE);

    Command_GetType(&response_command, &command_type);
    CORE_Assert(command_type == kCommandResponseType_JoinLobby);
    Command_GetPayloadPtr(&response_command, &response_payload_ptr, &response_payload_size);
    // CORE_Assert(response_payload_size == sizeof(uint32));
    // CORE_Assert(*((uint32 *) response_payload_ptr) == TRUE);
}

/**
 *		command `Authenticate` contains:
 *			- 0..4 	    (4 bytes)	    -> command type
 *			- 4..40	    (36 bytes)	    -> login	
 *			- 40..64	(24 bytes)	    -> password
 * 
 *      response contains:
 *          - 0..4      (4 bytes)       -> command response type
 *          - 4..36     (32 bytes)      -> token
 */

void Test_CommandAuthenticate(AuthCommandsProcessor commands_processor)
{
    CORE_Bool                is_have_response;
    struct Command           command;
    struct Command           response_command;
    uint32                   command_type;
    const uint8              *response_payload_ptr;
    uint32                   response_payload_size;

    const uint8              login[36]      = "delphifeel"; 
    const uint8              password[24]   = "1234"; 

    
    command_type = kCommandType_Authenticate;

    INIT_PAYLOAD(60);
    WRITE_TO_PAYLOAD(login, sizeof(login));
    WRITE_TO_PAYLOAD(password, sizeof(password));

    Command_Init(&response_command);

    Command_Init(&command); 
    Command_SetType(&command, command_type);
    Command_SetPayload(&command, GET_PAYLOAD, sizeof(GET_PAYLOAD));
    
    CORE_Assert(AuthCommandsProcessor_Process(commands_processor, command_type, &command, &response_command, &is_have_response) == TRUE);
    CORE_Assert(is_have_response == TRUE);

    Command_GetType(&response_command, &command_type);
    CORE_Assert(command_type == kCommandResponseType_Authenticate);
    Command_GetPayloadPtr(&response_command, &response_payload_ptr, &response_payload_size);
    CORE_Assert(response_payload_size == TOKEN_SIZE);
    CORE_Assert(memcmp(response_payload_ptr, mocked_token, TOKEN_SIZE) == 0);
}

void Test_AuthCommandsProcessor()
{
    AuthCommandsProcessor commands_processor;


    AuthCommandsProcessor_Create(&commands_processor);
    AuthCommandsProcessor_Setup(commands_processor, GetAuthCommandToProcessFunc(), GetAuthCommandToProcessFuncSize());

    Test_CommandJoinLobby(commands_processor);
    Test_CommandAuthenticate(commands_processor);

    AuthCommandsProcessor_Free(&commands_processor);
}
