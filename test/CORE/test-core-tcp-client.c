#include "CORE.h"
#include "authserver/CONFIG.h"
#include "gameserver/gameserver-command-types.h"

static void OnReadCb(	CORE_TCPClient instance, void *context,
				const uint8 data[], uint32 data_size)
{
	// char response_string[100];


	// CORE_DebugPrint("Read(%d bytes): %s\n", data_size, data);

	// response_string[0] = 0;
	// strcat(response_string, "Hello world\n");

	// CORE_TCPClient_Write(instance, client_connection, (const uint8 *) response_string, strlen(response_string));
}


typedef struct StartGamePayload 
{
    struct 
    {
        uint32  player_id;
        uint8   player_token[32];   
    } players[4];
} StartGamePayload;

static void OnConnectedCb(CORE_TCPClient instance, void *context)
{
	uint8 				buffer[512];
	uint8 				*buffer_ptr;
	StartGamePayload 	payload;


    payload.players[0].player_id = 1;
    memcpy(payload.players[0].player_token, mocked_token, TOKEN_SIZE);

    payload.players[1].player_id = 2;
    memcpy(payload.players[1].player_token, mocked_token, TOKEN_SIZE);

    payload.players[2].player_id = 3;
    memcpy(payload.players[2].player_token, mocked_token, TOKEN_SIZE);

    payload.players[3].player_id = 4;
    memcpy(payload.players[3].player_token, mocked_token, TOKEN_SIZE);


	buffer_ptr = buffer;

	*(uint32 *) buffer_ptr = 0xDEADBEAF;
	buffer_ptr += 4;
	*(uint32 *) buffer_ptr = kCommandType_StartGame;
	buffer_ptr += 4;
	CORE_MemZero(buffer_ptr, 40);
	buffer_ptr += 40;
	memcpy(buffer_ptr, &payload, sizeof(payload));

	CORE_TCPClient_Write(instance, (const uint8 *) buffer, 48 + sizeof(payload));

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

	CORE_TCPClient_Free(&client);
}
