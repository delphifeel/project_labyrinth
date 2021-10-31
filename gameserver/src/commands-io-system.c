#include "CORE.h"
#include "command.h"
#include "commands-processor.h"
#include "gameserver/commands-io-system.h"
#include "gameserver/game-server-config.h"
#include "gameserver/gameserver-command.h"
#include "gameserver/gameserver-command-types.h"

/*****************************************************************************************************************************/

#define _GAMESERVER_COMMAND_VALIDATION_ID    (0xDEADBEAF)

#define CONNECTIONS_PER_SESSION  (SESSION_PLAYERS_COUNT)

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(CommandsIOSystem,
    LabSession                             *sessions;
    uint32                                 sessions_size;

    CommandsProcessor                      commands_processor;
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

static CORE_Bool _ParseCommandFromBuffer(GameServerCommand *instance, const uint8 buffer[], uint32 buffer_size)
{
    CORE_AssertPointer(buffer);

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

static void _TCPServerOnRead(CORE_TCPServer tcp_server, void *context, 
                             CORE_TCPServer_ClientConnection client_connection,
                             const uint8 data[], uint32 data_size)
{
    GameServerCommand               command;
    GameServerCommand               response_command;
    CommandsIOSystem                instance;
    uint32                          session_index;
    uint32                          player_index;


    GameServerCommand_Init(&command);
    GameServerCommand_Init(&response_command);

    if (_ParseCommandFromBuffer(&command, data, data_size) == FALSE)
    {
        CORE_DebugError("Parse `data` for command error\n");
        return;
    }
    
    GameServerCommand_GetSessionIndex(&command, &session_index);
    GameServerCommand_GetPlayerIndex(&command, &player_index);

    // add tcp_client to `tcp_clients_map` if its not in map already

    if ((session_index > SESSIONS_CAPACITY - 1) ||
        (player_index > CONNECTIONS_PER_SESSION - 1))
    {
        CORE_DebugError("Command session_index or player_index out of bounds\n");
        return;
    }

    instance = (CommandsIOSystem) context;

    if (instance->tcp_clients_map[session_index][player_index] == NULL)
    {
        instance->tcp_clients_map[session_index][player_index] = client_connection;
    }

    GameServerCommand_SetSessionsPtr(&command, instance->sessions, instance->sessions_size);

    if (CommandsProcessor_Process(instance->commands_processor, (Command *) &command, (Command *) &response_command) == FALSE)
    {
        CORE_DebugError("Command processing error\n");
        return;
    }
}

/*****************************************************************************************************************************/

// void CommandsIOSystem_Send(CommandsIOSystem instance, const ServerCommandHeaderStruct *command_header, const uint8 *command_payload)
// {
//     CORE_AssertPointer(command_header);
//     CORE_AssertPointer(command_payload);

//     uint32                              session_index;
//     uint32                              player_index;
//     CORE_TCPServer_ClientConnection     client_connection;


//     session_index = command_header->session_index;
//     player_index = command_header->player_index;

//     if ((session_index > SESSIONS_CAPACITY - 1) ||
//         (player_index > CONNECTIONS_PER_SESSION - 1))
//     {
//         CORE_DebugError("Command session_index or player_index out of bounds\n");
//         return;
//     }

//     client_connection = instance->tcp_clients_map[session_index][player_index];
//     if (client_connection == NULL)
//     {
//         CORE_DebugError("No associated connection with this command\n");
//         return;
//     }

//     CORE_TCPServer_Write(instance->tcp_server, client_connection, (const uint8 *) command_to_client, sizeof(CommandStruct));
// }

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

    CommandsProcessor_Setup(instance->commands_processor, GetGameServerCommandToProcessFunc());
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
    CommandsProcessor_Create(&instance->commands_processor);
}

void CommandsIOSystem_Free(CommandsIOSystem *instance_ptr)
{
    CommandsProcessor_Free(&(*instance_ptr)->commands_processor);
    CORE_TCPServer_Free(&(*instance_ptr)->tcp_server);

	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
