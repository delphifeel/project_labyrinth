#include "CORE/CORE-tcp-server.h"

CORE_OBJECT_INTERFACE(CORE_TCPServer,
	uint32 _mock;
);

void CORE_TCPServer_Setup(CORE_TCPServer instance)
{
	
}

void CORE_TCPServer_Create(CORE_TCPServer *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, CORE_TCPServer);
}

void CORE_TCPServer_Free(CORE_TCPServer *instance_ptr)
{
	CORE_OBJECT_FREE(instance_ptr);
}
