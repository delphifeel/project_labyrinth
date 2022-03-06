#include "CORE.h"
#include <lib/commands-processor/command.h>
#include "authserver/auth-io-system.h"
#include "authserver/CONFIG.h"
#include "authserver/auth-command-types.h"
#include "authserver/auth-commands-processor.h"

#define _AUTH_COMMAND_VALIDATION_ID     (0xBADBEE)

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(AuthIOSystem, 
    CORE_TCPServer                      tcp_server;
    AuthCommandsProcessor               commands_processor;

    CORE_TCPServer_ClientConnection     tcp_clients_map[AUTH_IO_MAX_CONNECTIONS];
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

static CORE_Bool _ParseCommandFromBuffer(struct Command *instance, const uint8 buffer[], uint32 buffer_size)
{
    CORE_AssertPointer(buffer);

    if (buffer_size <= 8)
    {
        CORE_DebugError("Buffer is too small\n");
        return FALSE;
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

static CORE_Bool _ConvertCommandToBuffer(struct Command      *instance, 
                                         uint8               buffer[],
                                         uint32              buffer_max_size,  
                                         uint32              *out_buffer_size)
{
    CORE_AssertPointer(buffer);
    CORE_AssertPointer(out_buffer_size);
    if (buffer_max_size <= 8)
    {
        CORE_DebugError("Buffer is too small\n");
        return FALSE;
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
        return FALSE;
    }

    memcpy(buffer_ptr, payload, payload_size);

    *out_buffer_size = 8 + payload_size;

    return TRUE;
}

static CORE_Bool _IsSendBufferToAllConnections(struct Command *response_command)
{
    uint32          command_type;
    const uint8     *payload;
    uint32          payload_size;
    uint32          status_code;


    Command_GetType(response_command, &command_type);
    if (command_type != kCommandType_JoinLobby)
    {
        return FALSE;
    }

    Command_GetPayloadPtr(response_command, &payload, &payload_size);
    status_code = * (uint32 *) payload;
    if (status_code != 2)
    {
        return FALSE;
    }

    return TRUE;
}

static void _TCP_ServerOnRead(CORE_TCPServer tcp_server, void *context,
                             CORE_TCPServer_ClientConnection client_connection, 
                             const uint8 data[], uint32 data_size)
{
    uint32                command_type;
    struct Command        command;
    struct Command        response_command;
    AuthIOSystem          instance;
    CORE_Bool             is_have_response;
    uint8                 response_buffer[512];
    uint32                response_buffer_size;


    instance = (AuthIOSystem) context;
    Command_Init(&command);
    Command_Init(&response_command);

    if (_ParseCommandFromBuffer(&command, data, data_size) == FALSE)
    {
        CORE_DebugError("Parse `data` for command error\n");
        CORE_TCPServer_CloseConnection(tcp_server, client_connection);
        return; 
    }

    // add connection to tcp_clients_map if not already
    {
        uint32                empty_slot_index;
        CORE_Bool             empty_slot_found;
        CORE_Bool             connection_already_in_map;


        empty_slot_found = FALSE;
        connection_already_in_map = FALSE;
        for (uint32 i = 0; i < AUTH_IO_MAX_CONNECTIONS; i++)
        {
            if ((empty_slot_found == FALSE) && (instance->tcp_clients_map[i] == NULL))
            {
                empty_slot_index = i;
                empty_slot_found = TRUE;
                continue;
            }

            if (instance->tcp_clients_map[i] == client_connection)
            {
                connection_already_in_map = TRUE;
                break;
            }
        }

        if ((connection_already_in_map == FALSE) && (empty_slot_found == TRUE))
        {
            CORE_DebugInfo("Adding connection to `tcp_clients_map` (index %d)\n", empty_slot_index);
            instance->tcp_clients_map[empty_slot_index] = client_connection;
        }
    }

    is_have_response = FALSE;
    Command_GetType(&command, &command_type);
    AuthCommandsProcessor_Process(instance->commands_processor, command_type, &command, &response_command, &is_have_response);

    if (is_have_response == TRUE)
    {
        if (_ConvertCommandToBuffer(&response_command, 
                                    response_buffer, 
                                    sizeof(response_buffer), 
                                    &response_buffer_size) == FALSE)
        {
            CORE_TCPServer_CloseConnection(tcp_server, client_connection);
            return;
        }

        if (_IsSendBufferToAllConnections(&response_command) == TRUE)
        {
            for (uint32 i = 0; i < AUTH_IO_MAX_CONNECTIONS; i++)
            {
                if (instance->tcp_clients_map[i] == NULL)
                {
                    continue;
                }

                CORE_TCPServer_Write(tcp_server, instance->tcp_clients_map[i], response_buffer, response_buffer_size);
            }
        }
        else
        {
            CORE_TCPServer_Write(tcp_server, client_connection, response_buffer, response_buffer_size);
        }
    }
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

    AuthCommandsProcessor_Setup(instance->commands_processor, GetAuthCommandToProcessFunc(), GetAuthCommandToProcessFuncSize());
}

/*****************************************************************************************************************************/

void AuthIOSystem_Create(AuthIOSystem *instance_ptr)
{
    AuthIOSystem instance;
    CORE_OBJECT_CREATE(instance_ptr, AuthIOSystem); 

    instance = *instance_ptr; 

    CORE_MemZero(&instance->tcp_clients_map, sizeof(instance->tcp_clients_map));

    CORE_TCPServer_Create(&instance->tcp_server);
    AuthCommandsProcessor_Create(&instance->commands_processor);
}
/*****************************************************************************************************************************/


void AuthIOSystem_Free(AuthIOSystem *instance_ptr)
{
    AuthCommandsProcessor_Free(&(*instance_ptr)->commands_processor);
    CORE_TCPServer_Free(&(*instance_ptr)->tcp_server); 

    CORE_OBJECT_FREE(instance_ptr); 
}
