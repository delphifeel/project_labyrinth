#include "CORE.h"
#include "gameserver/commands-io-system.h"
#include "gameserver/game-server-config.h"

/*****************************************************************************************************************************/

#define CONNECTIONS_PER_SESSION  (SESSION_PLAYERS_COUNT)

/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(CommandsIOSystem,
    void                                   *context;
	CORE_TCPServer                         tcp_server;

    OnCommandGetFunc   on_command_get;          

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

static void _TCPServerOnRead(CORE_TCPServer tcp_server, void *context, 
                             CORE_TCPServer_ClientConnection client_connection,
                             const uint8 data[], uint32 data_size)
{
    CommandStruct       *command_from_client;
    CommandsIOSystem    instance;
    uint32              session_index;
    uint32              player_index;


    if (data_size != sizeof(CommandStruct))
    {
        CORE_DebugError("Data size != size of `CommandStruct`\n");
        return;
    }

    command_from_client = (CommandStruct *) data;
    if (Command_Verificate(command_from_client) == FALSE)
    {
        CORE_DebugError("Command verification failed: data is not a `CommandStruct`\n");
        return;
    }

    instance = (CommandsIOSystem) context;

    // add tcp_client to `tcp_clients_map` if its not in map already
    session_index = command_from_client->session_index;
    player_index = command_from_client->player_index;

    if ((session_index > SESSIONS_CAPACITY - 1) ||
        (player_index > CONNECTIONS_PER_SESSION - 1))
    {
        CORE_DebugError("Command session_index or player_index out of bounds\n");
        return;
    }

    if (instance->tcp_clients_map[session_index][player_index] == NULL)
    {
        instance->tcp_clients_map[session_index][player_index] = client_connection;
    }

    instance->on_command_get(instance, instance->context, command_from_client);
}

/*****************************************************************************************************************************/

void CommandsIOSystem_Send(CommandsIOSystem instance, const CommandStruct *command_to_client)
{
    CORE_AssertPointer(command_to_client);

    uint32                              session_index;
    uint32                              player_index;
    CORE_TCPServer_ClientConnection     client_connection;


    session_index = command_to_client->session_index;
    player_index = command_to_client->player_index;

    if ((session_index > SESSIONS_CAPACITY - 1) ||
        (player_index > CONNECTIONS_PER_SESSION - 1))
    {
        CORE_DebugError("Command session_index or player_index out of bounds\n");
        return;
    }

    client_connection = instance->tcp_clients_map[session_index][player_index];
    if (client_connection == NULL)
    {
        CORE_DebugError("No associated connection with this command\n");
        return;
    }

    CORE_TCPServer_Write(instance->tcp_server, client_connection, (const uint8 *) command_to_client, sizeof(CommandStruct));
}

/*****************************************************************************************************************************/

void CommandsIOSystem_OnGet(CommandsIOSystem instance, OnCommandGetFunc on_command_get)
{
    CORE_AssertPointer(on_command_get);

    instance->on_command_get = on_command_get;
}

/*****************************************************************************************************************************/

void CommandsIOSystem_SetContext(CommandsIOSystem instance, void *context)
{
    instance->context = context;
}

/*****************************************************************************************************************************/

void CommandsIOSystem_Setup(CommandsIOSystem instance)
{
    CORE_AssertPointer(instance->on_command_get);

    CORE_TCPServer_OnError(instance->tcp_server, _TCPServerOnError);
    CORE_TCPServer_OnNewConnection(instance->tcp_server, _TCPServerOnNewConnection);
    CORE_TCPServer_OnCloseConnection(instance->tcp_server, _TCPServerOnCloseConnection);
    CORE_TCPServer_OnRead(instance->tcp_server, _TCPServerOnRead);

    CORE_TCPServer_SetContext(instance->tcp_server, instance);

	CORE_TCPServer_Setup(instance->tcp_server, COMMANDS_IO_SYSTEM_DEFAULT_PORT);
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

    instance->context = NULL;

    CORE_MemZero(&instance->tcp_clients_map, sizeof(instance->tcp_clients_map));
    CORE_TCPServer_Create(&instance->tcp_server);
}

void CommandsIOSystem_Free(CommandsIOSystem *instance_ptr)
{
    CORE_TCPServer_Free(&(*instance_ptr)->tcp_server);

	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
