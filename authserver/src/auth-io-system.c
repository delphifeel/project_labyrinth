#include "CORE.h"
#include "commands-processor.h"
#include "authserver/auth-io-system.h"
#include "authserver/config.h"
#include "authserver/auth-command-types.h"

#define _AUTH_COMMAND_VALIDATION_ID     (0xBADBEE)

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(AuthIOSystem, 
    CORE_TCPServer              tcp_server;
    CommandsProcessor           commands_processor;
);

/*****************************************************************************************************************************/

static void _TCP_ServerOnError(CORE_TCPServer tcp_server, void *context, const char *error_message)
{
    CORE_DebugError("TCP Server error: %s\n", error_message); 
}


static void _TCP_ServerOnNewConnection(CORE_TCPServer tcp_server, void *context, 
                                      CORE_TCPServer_ClientConnection client_connection)
{
    CORE_DebugInfo("TCP Server - new connection\n"); 
}


static void _TCP_ServerOnCloseConnection(CORE_TCPServer tcp_server, void *context, 
                                        CORE_TCPServer_ClientConnection client_connection)
{
    CORE_DebugInfo("TCP Server - close connection\n");
}

static CORE_Bool _ParseCommandFromBuffer(Command *instance, const uint8 buffer[], uint32 buffer_size)
{
    CORE_AssertPointer(buffer);

    uint32          validation_header;
    const uint8     *buffer_ptr;
    uint32          buffer_size_left;


    buffer_ptr = buffer;
    buffer_size_left = buffer_size;

    // 0...4    (4 bytes)   - validation header
    validation_header = *((uint32 *) buffer_ptr);
    if (validation_header != _AUTH_COMMAND_VALIDATION_ID)
    {
        CORE_DebugError("no validation header - `buffer` is not a command\n");
        return FALSE;
    }
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 4...8   (4 bytes)  - command type
    Command_SetType(instance, *((uint32 *) buffer_ptr));
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 8...~               - command payload
    if (Command_SetPayload(instance, buffer_ptr, buffer_size_left) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

static void _TCP_ServerOnRead(CORE_TCPServer tcp_server, void *context,
                             CORE_TCPServer_ClientConnection client_connection, 
                             const uint8 data[], uint32 data_size)
{
    Command             command;
    Command             command_to_send;
    AuthIOSystem        instance;


    instance = (AuthIOSystem) context;
    Command_Init(&command);
    Command_Init(&command_to_send);

    if (_ParseCommandFromBuffer(&command, data, data_size) == FALSE)
    {
        CORE_DebugError("Parse `data` for command error\n"); 
        return; 
    }

    CommandsProcessor_Process(instance->commands_processor, &command, &command_to_send);
}

/*****************************************************************************************************************************/

void AuthIOSystem_Start(AuthIOSystem instance)
{
    CORE_TCPServer_Start(instance->tcp_server);
} 

/*****************************************************************************************************************************/


void AuthIOSystem_Setup(AuthIOSystem instance)
{
    CORE_TCPServer_OnError(instance->tcp_server, _TCP_ServerOnError); 
    CORE_TCPServer_OnNewConnection(instance->tcp_server, _TCP_ServerOnNewConnection); 
    CORE_TCPServer_OnCloseConnection(instance->tcp_server, _TCP_ServerOnCloseConnection);
    CORE_TCPServer_OnRead(instance->tcp_server, _TCP_ServerOnRead);
    CORE_TCPServer_SetContext(instance->tcp_server, instance);
    CORE_TCPServer_Setup(instance->tcp_server, AUTH_IO_SYSTEM_DEFAULT_PORT); 

    CommandsProcessor_Setup(instance->commands_processor, GetAuthCommandToProcessFunc());
}

/*****************************************************************************************************************************/

void AuthIOSystem_Create(AuthIOSystem *instance_ptr)
{
    AuthIOSystem instance; 
    CORE_OBJECT_CREATE(instance_ptr, AuthIOSystem); 

    instance = *instance_ptr; 
    CORE_TCPServer_Create(&instance->tcp_server);
    CommandsProcessor_Create(&instance->commands_processor);
}
/*****************************************************************************************************************************/


void AuthIOSystem_Free(AuthIOSystem *instance_ptr)
{
    CommandsProcessor_Free(&(*instance_ptr)->commands_processor);
    CORE_TCPServer_Free(&(*instance_ptr)->tcp_server); 

    CORE_OBJECT_FREE(instance_ptr); 
}
