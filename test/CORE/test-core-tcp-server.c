#include "CORE.h"

void OnReadCb(	CORE_TCPServer instance, void *context, CORE_TCPServer_ClientConnection client_connection,
				const uint8 data[], uint32 data_size)
{
	char response_string[100];


	CORE_DebugPrint("Read(%d bytes): %s\n", data_size, data);

	response_string[0] = 0;
	strcat(response_string, "Hello world\n");

	CORE_TCPServer_Write(instance, client_connection, (const uint8 *) response_string, strlen(response_string));
}

void OnErrorCb(CORE_TCPServer instance, void *context, const char *error_message)
{
	CORE_DebugPrint("%s\n", error_message);
}

void OnNewConnectionCb(CORE_TCPServer instance, void *context, CORE_TCPServer_ClientConnection client_connection)
{
	CORE_DebugPrint("New connection %lu\n", (uint32) client_connection);
}

void OnCloseConnectionCb(CORE_TCPServer instance, void *context, CORE_TCPServer_ClientConnection client_connection)
{
	CORE_DebugPrint("Close connection %lu\n", (uint32) client_connection);
}

void Test_CORE_TCPServer()
{
	CORE_TCPServer server;

	CORE_TCPServer_Create(&server);
	CORE_TCPServer_OnRead(server, OnReadCb);
	CORE_TCPServer_OnError(server, OnErrorCb);
	CORE_TCPServer_OnNewConnection(server, OnNewConnectionCb);
	CORE_TCPServer_OnCloseConnection(server, OnCloseConnectionCb);
	CORE_TCPServer_Setup(server, 7000);
	CORE_TCPServer_Start(server);
}
