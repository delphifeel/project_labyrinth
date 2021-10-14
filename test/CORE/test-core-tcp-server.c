#include "CORE.h"

void OnReadCb(	CORE_TCPServer instance, const uint8 data[], uint32 data_size, 
				uint8 **out_response_data, uint32 *out_response_data_size)
{
	char *response_string;


	CORE_DebugPrint("Read(%d bytes): %s\n", data_size, data);

	response_string = CORE_MemAlloc(100);
	response_string[0] = 0;
	strcat(response_string, "Hello world\n");

	*out_response_data = response_string;
	*out_response_data_size = strlen(response_string);
}

void OnErrorCb(CORE_TCPServer instance, const char *error_message)
{
	CORE_DebugPrint("%s\n", error_message);
}

void OnNewConnectionCb(CORE_TCPServer instance)
{
	CORE_DebugPrint("New connection\n");
}

void OnCloseConnectionCb(CORE_TCPServer instance)
{
	CORE_DebugPrint("Close connection\n");
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
