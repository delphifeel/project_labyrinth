#include "CORE.h"
#include "lib/commands-processor/command.h"
#include "lib/commands-processor/commands-processor.h"
#include "gameserver/commands-io-system.h"
#include "gameserver/CONFIG.h"
#include "lib/commands-processor/command.h"
#include "gameserver/gameserver-command-response.h"
#include "gameserver/gameserver-command-types.h"
#include "gameserver/gameserver-commands-processor.h"

/*****************************************************************************************************************************/

#define _GAMESERVER_COMMAND_VALIDATION_ID    (0xDEADBEAF)

#define CONNECTIONS_PER_SESSION  (SESSION_PLAYERS_COUNT)

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(CommandsIOSystem,
    LabSession                             *sessions;
    uint32                                 sessions_size;

    GameServerCommandsProcessor            commands_processor;
	CORE_TCPServer                         tcp_server;

    // OnCommandGetFunc   on_command_get;          

    /*
     *      All TCP clients connected to game server.
     *      This is 2-dimensional array (hash map) where
     *      1st index - session index
     *      2nd index - player index
     */
    CORE_TCPServer_ClientConnection        tcp_clients_map[SESSIONS_CAPACITY][CONNECTIONS_PER_SESSION];
);

/*****************************************************************************************************************************/

static void _TCPServerOnError(CORE_TCPServer tcp_server, void *context, const char *error_message)
{
    CORE_DebugError("TCP Server error: %s\n", error_message);
}

static void _TCPServerOnNewConnection(CORE_TCPServer tcp_server, void *context, 
                                      CORE_TCPServer_ClientConnection client_connection)
{
    CORE_DebugInfo("TCP Server - new connection\n");
}

static void _TCPServerOnCloseConnection(CORE_TCPServer tcp_server, void *context, 
                                        CORE_TCPServer_ClientConnection client_connection)
{
    CORE_DebugInfo("TCP Server - close connection\n");
}

#define _MIN_COMMAND_SIZE   (48)

static CORE_Bool _ParseCommandFromBuffer(struct GameServerCommand *instance, const uint8 buffer[], uint32 buffer_size)
{
    CORE_AssertPointer(buffer);
    if (buffer_size <= _MIN_COMMAND_SIZE)
    {
        CORE_DebugError("Buffer is too small\n");
        return FALSE;

    }

    uint32          validation_header;
    const uint8     *buffer_ptr;
    uint32          buffer_size_left;


    buffer_ptr = buffer;
    buffer_size_left = buffer_size;

    // 0...4    (4 bytes)   - validation header
    validation_header = *((uint32 *) buffer_ptr);
    if (validation_header != _GAMESERVER_COMMAND_VALIDATION_ID)
    {
        CORE_DebugError("no validation header - `buffer` is not a command\n");
        return FALSE;
    }
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 4...8   (4 bytes)  - command type
    GameServerCommand_SetType(instance, *((uint32 *) buffer_ptr));
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 8...12   (4 bytes) - command session index
    GameServerCommand_SetSessionIndex(instance, *((uint32 *) buffer_ptr));
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 12...16   (4 bytes) - command player index
    GameServerCommand_SetPlayerIndex(instance, *((uint32 *) buffer_ptr));
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 16...48   (32 bytes) - command player token
    GameServerCommand_SetPlayerToken(instance, buffer_ptr);
    buffer_ptr += 32;
    buffer_size_left -= 32;

    // 48...~   (~ bytes) - command payload
    if (GameServerCommand_SetPayload(instance, buffer_ptr, buffer_size_left) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

#define _MIN_COMMAND_RESPONSE_SIZE   (8)

static CORE_Bool _ConvertCommandResponseToBuffer(struct GameServerCommandResponse       *instance, 
                                                 uint8                                  buffer[], 
                                                 uint32                                 buffer_max_size,
                                                 uint32                                 *out_buffer_size)
{
    CORE_AssertPointer(buffer);
    CORE_AssertPointer(out_buffer_size);
    if (buffer_max_size <= _MIN_COMMAND_RESPONSE_SIZE)
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
    *((uint32 *) buffer_ptr) = _GAMESERVER_COMMAND_VALIDATION_ID;
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 4...8   (4 bytes)  - command type
    GameServerCommandResponse_GetType(instance, (uint32 *) buffer_ptr);
    buffer_ptr += 4;
    buffer_size_left -= 4;

    // 8...~   (~ bytes) - command payload
    GameServerCommandResponse_GetPayloadPtr(instance, &payload, &payload_size);

    if (payload_size > buffer_size_left)
    {
        CORE_DebugError("Command response payload size (%u) > buffer size left (%u)\n", payload_size, buffer_size_left);
        return FALSE;
    }

    memcpy(buffer_ptr, payload, payload_size);

    *out_buffer_size = _MIN_COMMAND_RESPONSE_SIZE + payload_size;

    return TRUE;
}

static CORE_Bool _CommandFromAuthServer(uint32 command_type)
{
    return command_type == kCommandType_StartGame;
}

static CORE_Bool _ProcessCommand(CommandsIOSystem                   instance, 
                                 struct GameServerCommand           *command_ptr,
                                 uint32                             command_type,
                                 struct GameServerCommandResponse   *response_command_ptr,
                                 CORE_Bool                          *out_is_have_response,
                                 uint8                              response_buffer[],
                                 uint32                             response_buffer_max_size,
                                 uint32                             *out_response_buffer_size)
{
    GameServerCommand_SetSessionsPtr(command_ptr, instance->sessions, instance->sessions_size);

    *out_is_have_response = FALSE;
    if (GameServerCommandsProcessor_Process(instance->commands_processor, 
                                            command_type,
                                            command_ptr, 
                                            response_command_ptr,
                                            out_is_have_response) == FALSE)
    {
        return FALSE;
    }

    if (*out_is_have_response == TRUE)
    {
        if (_ConvertCommandResponseToBuffer(response_command_ptr, 
                                            response_buffer, 
                                            response_buffer_max_size, 
                                            out_response_buffer_size) == FALSE)
        {
            return FALSE;
        }
    }

    return TRUE;
}

static void _TCPServerOnRead(CORE_TCPServer tcp_server, void *context, 
                             CORE_TCPServer_ClientConnection client_connection,
                             const uint8 data[], uint32 data_size)
{
    uint32                                  command_type;
    struct GameServerCommand                command;
    struct GameServerCommandResponse        response_command;
    CommandsIOSystem                        instance;
    uint32                                  session_index;
    uint32                                  player_index;
    CORE_Bool                               is_have_response;
    uint8                                   response_buffer[512];
    uint32                                  response_buffer_size;
    const uint32                            *response_receivers_indexes;
    uint32                                  response_receivers_indexes_size;
    CORE_TCPServer_ClientConnection         iter_client_connection;
    uint32                                  iter_player_index;


    instance = (CommandsIOSystem) context;
    
    GameServerCommand_Init(&command);
    GameServerCommandResponse_Init(&response_command);

    if (_ParseCommandFromBuffer(&command, data, data_size) == FALSE)
    {
        CORE_DebugError("Parse `data` for command error\n");
        CORE_TCPServer_CloseConnection(tcp_server, client_connection);
        return;
    }

    GameServerCommand_GetType(&command, &command_type);
    if (_CommandFromAuthServer(command_type) == TRUE)
    {
        CORE_DebugInfo("Got command from auth server\n");

        if (_ProcessCommand(instance,
                            &command,
                            command_type,
                            &response_command,
                            &is_have_response,
                            response_buffer,
                            sizeof(response_buffer),
                            &response_buffer_size) == FALSE)
        {
            CORE_DebugError("Auth server command processing error\n");
            CORE_TCPServer_CloseConnection(tcp_server, client_connection);
            return;
        }

        CORE_TCPServer_Write(tcp_server, client_connection, response_buffer, response_buffer_size);

        return;
    }
    
    GameServerCommand_GetSessionIndex(&command, &session_index);
    GameServerCommand_GetPlayerIndex(&command, &player_index);

    if ((session_index > SESSIONS_CAPACITY - 1) ||
        (player_index > CONNECTIONS_PER_SESSION - 1))
    {
        CORE_DebugError("Command session_index or player_index out of bounds\n");
        CORE_TCPServer_CloseConnection(tcp_server, client_connection);
        return;
    }

    if (instance->tcp_clients_map[session_index][player_index] == NULL)
    {
        CORE_DebugInfo(
            "Add new client to clients map (session index: %u, player index: %u)\n",
            session_index,
            player_index
        );
        instance->tcp_clients_map[session_index][player_index] = client_connection;
    }

    if (_ProcessCommand(instance,
                        &command,
                        command_type,
                        &response_command,
                        &is_have_response,
                        response_buffer,
                        sizeof(response_buffer),
                        &response_buffer_size) == FALSE)
    {
        CORE_DebugError("Command processing error\n");
        CORE_TCPServer_CloseConnection(tcp_server, client_connection);
        return;
    }

    GameServerCommandResponse_GetPlayerIndexesPtr(&response_command, 
                                                  &response_receivers_indexes, 
                                                  &response_receivers_indexes_size);

    if (response_receivers_indexes_size == 0)
    {
        CORE_DebugInfo("Send response to all players in session\n");

        for (uint32 i = 0; i < CONNECTIONS_PER_SESSION; i++)
        {
            iter_client_connection = instance->tcp_clients_map[session_index][i];
            if (iter_client_connection == NULL)
            {
                continue;
            }

            CORE_TCPServer_Write(tcp_server, iter_client_connection, response_buffer, response_buffer_size);
        }

        return;
    }

    CORE_DebugInfo("Send response to specific players in session ...\n");
    for (uint32 i = 0; i < response_receivers_indexes_size; i++)
    {
        iter_player_index = response_receivers_indexes[i];
        iter_client_connection = instance->tcp_clients_map[session_index][iter_player_index];
        if (iter_client_connection == NULL)
        {
            continue;
        }

        CORE_DebugInfo("Send to player #%u ...\n", iter_player_index);
        CORE_TCPServer_Write(tcp_server, iter_client_connection, response_buffer, response_buffer_size);
    }
}

/*****************************************************************************************************************************/

void CommandsIOSystem_Setup(CommandsIOSystem instance, LabSession sessions[], uint32 sessions_size)
{
    CORE_AssertPointer(sessions);

    instance->sessions = sessions;
    instance->sessions_size = sessions_size;

    CORE_TCPServer_OnError(instance->tcp_server, _TCPServerOnError);
    CORE_TCPServer_OnNewConnection(instance->tcp_server, _TCPServerOnNewConnection);
    CORE_TCPServer_OnCloseConnection(instance->tcp_server, _TCPServerOnCloseConnection);
    CORE_TCPServer_OnRead(instance->tcp_server, _TCPServerOnRead);

    CORE_TCPServer_SetContext(instance->tcp_server, instance);
	CORE_TCPServer_Setup(instance->tcp_server, COMMANDS_IO_SYSTEM_DEFAULT_PORT);

    GameServerCommandsProcessor_Setup(instance->commands_processor, 
                                      GetGameServerCommandToProcessFunc(), 
                                      GetGameServerCommandToProcessFuncSize());
}

void CommandsIOSystem_Start(CommandsIOSystem instance)
{
    CORE_TCPServer_Start(instance->tcp_server);
}

/*****************************************************************************************************************************/

void CommandsIOSystem_Create(CommandsIOSystem *instance_ptr)
{
    CommandsIOSystem instance;


	CORE_OBJECT_CREATE(instance_ptr, CommandsIOSystem);
    instance = *instance_ptr;

    CORE_MemZero(&instance->tcp_clients_map, sizeof(instance->tcp_clients_map));
    CORE_TCPServer_Create(&instance->tcp_server);
    GameServerCommandsProcessor_Create(&instance->commands_processor);
}

void CommandsIOSystem_Free(CommandsIOSystem *instance_ptr)
{
    GameServerCommandsProcessor_Free(&(*instance_ptr)->commands_processor);
    CORE_TCPServer_Free(&(*instance_ptr)->tcp_server);

	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
