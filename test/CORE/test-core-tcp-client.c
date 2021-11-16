#include "CORE.h"

static void OnReadCb(	CORE_TCPClient instance, void *context,
				const uint8 data[], uint32 data_size)
{
	// char response_string[100];


	// CORE_DebugPrint("Read(%d bytes): %s\n", data_size, data);

	// response_string[0] = 0;
	// strcat(response_string, "Hello world\n");

	// CORE_TCPClient_Write(instance, client_connection, (const uint8 *) response_string, strlen(response_string));
}

static void OnConnectedCb(CORE_TCPClient instance, void *context)
{
	char data[5];

	memcpy(data, "DAD\n", sizeof(data));
	CORE_TCPClient_Write(instance, (const uint8 *) data, sizeof(data));

	// CORE_TCPClient_Disconnect(instance);
}

static void OnCloseConnectionCb(CORE_TCPClient instance, void *context)
{
}

void Test_CORE_TCPClient()
{
	CORE_TCPClient client;

	CORE_TCPClient_Create(&client);
	CORE_TCPClient_OnRead(client, OnReadCb);
	CORE_TCPClient_OnConnected(client, OnConnectedCb);
	CORE_TCPClient_OnCloseConnection(client, OnCloseConnectionCb);

	if (CORE_TCPClient_Connect(client, "127.0.0.1", 7000) == FALSE)
	{
		CORE_DebugError("ERROR 1\n");
	}

	if (CORE_TCPClient_Connect(client, "127.0.0.1", 7000) == FALSE)
	{
		CORE_DebugError("ERROR 2\n");
	}

	CORE_TCPClient_Free(&client);
}
