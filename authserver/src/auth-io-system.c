#include "CORE.h"
#include "authserver/auth-io-system.h"
#include "authserver/config.h"


/*****************************************************************************************************************************/

CORE_OBJECT_INTERFACE(AuthIOSystem, 
    CORE_TCPServer              tcp_server;
);


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


static void _TCP_ServerOnRead(CORE_TCPServer tcp_server, void *context,
                             CORE_TCPServer_ClientConnection client_connection, 
                             const uint8 data[], uint32 data_size)
{

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
    CORE_TCPServer_Setup(instance->tcp_server, AUTH_IO_SYSTEM_DEFAULT_PORT); 
}

/*****************************************************************************************************************************/

void AuthIOSystem_Create(AuthIOSystem *instance_ptr)
{
    AuthIOSystem instance; 
    CORE_OBJECT_CREATE(instance_ptr, AuthIOSystem); 

    instance = *instance_ptr; 
    CORE_TCPServer_Create(&instance->tcp_server);
}
/*****************************************************************************************************************************/


void AuthIOSystem_Free(AuthIOSystem *instance_ptr)
{
    CORE_TCPServer_Free(&(*instance_ptr)->tcp_server); 
    CORE_OBJECT_FREE(instance_ptr); 
}