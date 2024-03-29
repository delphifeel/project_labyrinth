#include "CCORE.h"
#include <lib/commands-processor/command.h>
#include <lib/tcp/tcp-server.h>
#include "authserver/auth-io-system.h"
#include "authserver/config.h"
#include "authserver/auth-command-types.h"
#include "authserver/auth-commands-processor.h"

#define _AUTH_COMMAND_VALIDATION_ID     (0xBADBEE)

/*****************************************************************************************************************************/

typedef struct AuthIOSystem_s
{ 
    TCPServer                      *tcp_server;
    AuthCommandsProcessor          commands_processor;

    TCPServer_ClientConnection     tcp_clients_map[AUTH_IO_MAX_CONNECTIONS];
} *AuthIOSystem;

/*****************************************************************************************************************************/

static void _TCP_ServerOnError(TCPServer *tcp_server, void *context, const char *error_message)
{
    CORE_DebugError("TCP Server error: %s\n", error_message); 
}


static void _TCP_ServerOnNewConnection(TCPServer *tcp_server, void *context, 
                                      TCPServer_ClientConnection client_connection)
{
    CORE_DebugInfo("TCP Server - new connection\n"); 
}


static void _TCP_ServerOnCloseConnection(TCPServer *tcp_server, void *context, 
                                        TCPServer_ClientConnection client_connection)
{
    CORE_DebugInfo("TCP Server - close connection\n");
}

static bool _ParseCommandFromBuffer(struct Command *instance, const uint8 buffer[], uint32 buffer_size)
{
    CORE_AssertPointer(buffer);

    if (buffer_size <= 8)
    {
        CORE_DebugError("Buffer is too small\n");
        return false;
    }

    uint32          validation_header;
    const uint8     *buffer_ptr;
    uint32          buffer_size_left;


    CORE_DebugInfo("Buffer size: %d\n", buffer_size);

    buffer_ptr = buffer;
    buffer_size_left = buffer_size;

    // 0...4    (4 bytes)   - validation header
    validation_header = *((uint32 *) buffer_ptr);
    if (validation_header != _AUTH_COMMAND_VALIDATION_ID)
    {
        CORE_DebugError("No validation header - `buffer` is not a `Command`\n");
        CORE_DebugInfo("Expected %x, got %x\n", _AUTH_COMMAND_VALIDATION_ID, validation_header);
        return false;
    }
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 4...8   (4 bytes)  - command type
    Command_SetType(instance, *((uint32 *) buffer_ptr));
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 8...~               - command payload
    if (Command_SetPayload(instance, buffer_ptr, buffer_size_left) == false)
    {
        return false;
    }

    return true;
}

static bool _ConvertCommandToBuffer(struct Command      *instance, 
                                         uint8               buffer[],
                                         uint32              buffer_max_size,  
                                         uint32              *out_buffer_size)
{
    CORE_AssertPointer(buffer);
    CORE_AssertPointer(out_buffer_size);
    if (buffer_max_size <= 8)
    {
        CORE_DebugError("Buffer is too small\n");
        return false;
    }

    uint8           *buffer_ptr;
    uint32          buffer_size_left;
    const uint8     *payload;
    uint32          payload_size;


    buffer_ptr = buffer;
    buffer_size_left = buffer_max_size;

    // 0...4    (4 bytes)   - validation header
    *((uint32 *) buffer_ptr) = _AUTH_COMMAND_VALIDATION_ID;
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 4...8   (4 bytes)  - command type
    Command_GetType(instance, (uint32 *) buffer_ptr);
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 8...~               - command payload
    Command_GetPayloadPtr(instance, &payload, &payload_size);

    if (payload_size > buffer_size_left)
    {
        CORE_DebugError("Command payload size (%u) > buffer size left (%u)\n", payload_size, buffer_size_left);
        return false;
    }

    memcpy(buffer_ptr, payload, payload_size);

    *out_buffer_size = 8 + payload_size;

    return true;
}

static bool _IsSendBufferToAllConnections(struct Command *response_command)
{
    uint32          command_type;
    const uint8     *payload;
    uint32          payload_size;
    uint32          status_code;


    Command_GetType(response_command, &command_type);
    Command_GetPayloadPtr(response_command, &payload, &payload_size);
    status_code = * (uint32 *) payload;
    return ((command_type == kCommandType_JoinLobby) && (status_code == 2));
}

static void _TCP_ServerOnRead(TCPServer *tcp_server, void *context,
                             TCPServer_ClientConnection client_connection, 
                             const uint8 data[], uint32 data_size)
{
    uint32                command_type;
    struct Command        command;
    struct Command        response_command;
    AuthIOSystem          instance;
    bool             is_have_response;
    uint8                 response_buffer[512];
    uint32                response_buffer_size;


    instance = (AuthIOSystem) context;
    Command_Init(&command);
    Command_Init(&response_command);

    if (_ParseCommandFromBuffer(&command, data, data_size) == false)
    {
        CORE_DebugError("Parse `data` for command error\n");
        TCPServer_CloseConnection(tcp_server, client_connection);
        return; 
    }

    // add connection to tcp_clients_map if not already
    {
        uint32                empty_slot_index;
        bool             empty_slot_found;
        bool             connection_already_in_map;


        empty_slot_found = false;
        connection_already_in_map = false;
        for (uint32 i = 0; i < AUTH_IO_MAX_CONNECTIONS; i++)
        {
            if ((empty_slot_found == false) && (instance->tcp_clients_map[i] == NULL))
            {
                empty_slot_index = i;
                empty_slot_found = true;
                continue;
            }

            if (instance->tcp_clients_map[i] == client_connection)
            {
                connection_already_in_map = true;
                break;
            }
        }

        if ((connection_already_in_map == false) && (empty_slot_found == true))
        {
            CORE_DebugInfo("Adding connection to `tcp_clients_map` (index %d)\n", empty_slot_index);
            instance->tcp_clients_map[empty_slot_index] = client_connection;
        }
    }

    is_have_response = false;
    Command_GetType(&command, &command_type);
    AuthCommandsProcessor_Process(instance->commands_processor, command_type, &command, &response_command, &is_have_response);

    if (is_have_response == true)
    {
        if (_ConvertCommandToBuffer(&response_command, 
                                    response_buffer, 
                                    sizeof(response_buffer), 
                                    &response_buffer_size) == false)
        {
            TCPServer_CloseConnection(tcp_server, client_connection);
            return;
        }

        if (_IsSendBufferToAllConnections(&response_command) == true)
        {
            for (uint32 i = 0; i < AUTH_IO_MAX_CONNECTIONS; i++)
            {
                if (instance->tcp_clients_map[i] == NULL)
                {
                    continue;
                }

                TCPServer_Write(tcp_server, instance->tcp_clients_map[i], response_buffer, response_buffer_size);
            }
        }
        else
        {
            TCPServer_Write(tcp_server, client_connection, response_buffer, response_buffer_size);
        }
    }
}

/*****************************************************************************************************************************/

void AuthIOSystem_Start(AuthIOSystem instance)
{
    TCPServer_Start(instance->tcp_server);
} 

/*****************************************************************************************************************************/


void AuthIOSystem_Setup(AuthIOSystem instance)
{
    TCPServer_OnError(instance->tcp_server, _TCP_ServerOnError); 
    TCPServer_OnNewConnection(instance->tcp_server, _TCP_ServerOnNewConnection); 
    TCPServer_OnCloseConnection(instance->tcp_server, _TCP_ServerOnCloseConnection);
    TCPServer_OnRead(instance->tcp_server, _TCP_ServerOnRead);
    TCPServer_SetContext(instance->tcp_server, instance);
    TCPServer_Setup(instance->tcp_server, AUTH_IO_SYSTEM_DEFAULT_PORT); 

    AuthCommandsProcessor_Setup(instance->commands_processor, GetAuthCommandToProcessFunc(), GetAuthCommandToProcessFuncSize());
}

/*****************************************************************************************************************************/

void AuthIOSystem_Create(AuthIOSystem *instance_ptr)
{
    AuthIOSystem instance;
    *instance_ptr = CORE_MemAlloc(sizeof(struct AuthIOSystem_s), 1);

    instance = *instance_ptr; 

    CORE_MemZero(&instance->tcp_clients_map, sizeof(instance->tcp_clients_map));

    instance->tcp_server = TCPServer_Create();
    AuthCommandsProcessor_Create(&instance->commands_processor);
}
/*****************************************************************************************************************************/


void AuthIOSystem_Free(AuthIOSystem *instance_ptr)
{
    AuthCommandsProcessor_Free(&(*instance_ptr)->commands_processor);
    TCPServer_Free((*instance_ptr)->tcp_server); 

    CORE_MemFree(*instance_ptr); 
}
