#include "CORE.h"

void Test_CORE_TCPServer()
{
	CORE_TCPServer server;


	CORE_TCPServer_Create(&server);
	CORE_TCPServer_Setup(server);

	CORE_TCPServer_Free(&server);
}